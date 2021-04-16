/* Copyright (c) 2017-2018  SiFive Inc. All rights reserved.

   This copyrighted material is made available to anyone wishing to use,
   modify, copy, or redistribute it subject to the terms and conditions
   of the FreeBSD License.   This program is distributed in the hope that
   it will be useful, but WITHOUT ANY WARRANTY expressed or implied,
   including the implied warranties of MERCHANTABILITY or FITNESS FOR
   A PARTICULAR PURPOSE.  A copy of this license is available at
   http://www.opensource.org/licenses.
*/

/* crt0.S: Entry point for RISC-V METAL programs. */

.section .text.libgloss.start
.global _start
.type   _start, @function

  /* _start is defined by the METAL to have been called with the following
   * arguments:
   *   a0: the hart ID of the currently executing hart.  Harts can start at
   *       any arbitrary point, it's the C library's job to ensure the code is
   *       safe.
   *   a1: a pointer to a description of the machine on which this code is
   *       currently executing.  This is probably 0 on an embedded system
   *       because they tend to not be dynamically portable.  As such, newlib
   *       ignores this argument.
   *   a2: a pointer to a function that must be run after the envirnoment has
   *       been initialized, but before user code can be expected to be run.
   *       If this is 0 then there is no function to be run. */
_start:
.cfi_startproc
.cfi_undefined ra

  /* This is a bit funky: it's not usually sane for _start to return, but in
   * this case we actually want to in order to signal an error to the METAL. */
  mv s0, ra

  /* Before doing anything we must initialize the global pointer, as we cannot
   * safely perform any access that may be relaxed without GP being set.  This
   * is done with relaxation disabled to avoid relaxing the address calculation
   * to just "addi gp, gp, 0". */
.option push
.option norelax
  la gp, __global_pointer$
.option pop

  /* Stack pointer is expected to be initialized before _start */

  /* Disable and clear all interrupt sources */
  li   a3, -1
  csrc mie, a3
  csrc mip, a3

  /* The delegation CSRs exist if user mode interrupts (N extension) or
   * supervisor mode (S extension) are supported */
  csrr a5, misa
  lui  a4, 0x42
  and  a4, a4, a5
  beqz a4, 1f
  csrc mideleg, a3
  csrc medeleg, a3
1:

  /* The satp CSR exists if supervisor mode (S extension) is supported */
  lui  a4, 0x40
  and  a4, a4, a5
  beqz a4, 1f
  csrc satp, a3
1:

  /* Check RISC-V isa and enable FS bits if Floating Point architecture. */
  li   a4, 0x10028
  and  a5, a5, a4
  beqz a5, 1f
  csrr a5, mstatus
  lui  a4, 0x2
  or   a5, a5, a4
  csrw mstatus, a5
  csrwi fcsr, 0
1:

  li a6, 0
  /* Check for vector extension support and enable it if found */
  csrr a5, misa
  li a4, 0x200000
  and a5, a5, a4
  beqz a5, 1f
  csrr a5, mstatus
  ori a5, a5, 0x200
  csrw mstatus, a5
  li a6, 1
1:

  /* If we're not hart 0, skip the initialization work */
  la t0, __metal_boot_hart
  bne a0, t0, _skip_init

  /* Embedded systems frequently require relocating the data segment before C
   * code can be run -- for example, the data segment may exist in flash upon
   * boot and then need to get relocated into a non-persistant writable memory
   * before C code can execute.  If this is the case we do so here.  This step
   * is optional: if the METAL provides an environment in which this relocation
   * is not necessary then it must simply set metal_segment_data_source_start to
   * be equal to metal_segment_data_target_start. */
  la t0, metal_segment_data_source_start
  la t1, metal_segment_data_target_start
  la t2, metal_segment_data_target_end

  beq t0, t1, 2f
  bge t1, t2, 2f

1:
#if __riscv_xlen == 32
  lw   a0, 0(t0)
  addi t0, t0, 4
  sw   a0, 0(t1)
  addi t1, t1, 4
  blt  t1, t2, 1b
#else
  ld   a0, 0(t0)
  addi t0, t0, 8
  sd   a0, 0(t1)
  addi t1, t1, 8
  blt  t1, t2, 1b
#endif
2:

  /* Copy the ITIM section */
  la t0, metal_segment_itim_source_start
  la t1, metal_segment_itim_target_start
  la t2, metal_segment_itim_target_end

  beq t0, t1, 2f
  bge t1, t2, 2f

1:
#if __riscv_xlen == 32
  lw   a0, 0(t0)
  addi t0, t0, 4
  sw   a0, 0(t1)
  addi t1, t1, 4
  blt  t1, t2, 1b
#else
  ld   a0, 0(t0)
  addi t0, t0, 8
  sd   a0, 0(t1)
  addi t1, t1, 8
  blt  t1, t2, 1b
#endif
2:

  /* Fence all subsequent instruction fetches until after the ITIM writes
     complete */
  fence.i

2:

  /* Copy the LIM section */
  la t0, metal_segment_lim_source_start
  la t1, metal_segment_lim_target_start
  la t2, metal_segment_lim_target_end

  beq t0, t1, 2f
  bge t1, t2, 2f

1:
#if __riscv_xlen == 32
  lw   a0, 0(t0)
  addi t0, t0, 4
  sw   a0, 0(t1)
  addi t1, t1, 4
  blt  t1, t2, 1b
#else
  ld   a0, 0(t0)
  addi t0, t0, 8
  sd   a0, 0(t1)
  addi t1, t1, 8
  blt  t1, t2, 1b
#endif
2:

  /* Fence all subsequent instruction fetches until after the LIM writes
     complete */
  fence.i

  /* Zero the BSS segment. */
  la t1, metal_segment_bss_target_start
  la t2, metal_segment_bss_target_end

  bge t1, t2, 2f

  # use rvv for zeroing
  beqz a6, 1f
  jal t3, rvv_memset_zero
  j 2f

1:
#if __riscv_xlen == 32
  sw   x0, 0(t1)
  addi t1, t1, 4
  blt  t1, t2, 1b
#else
  sd   x0, 0(t1)
  addi t1, t1, 8
  blt  t1, t2, 1b
#endif
2:

  /* Set TLS pointer */
  .weak __tls_base	
  la tp, __tls_base

  /* At this point we're in an environment that can execute C code.  The first
   * thing to do is to make the callback to the parent environment if it's been
   * requested to do so. */
  beqz a2, 1f
  jalr a2
1:

  /* The RISC-V port only uses new-style constructors and destructors. */
  la a0, __libc_fini_array
  call atexit
  call __libc_init_array

  /* Register metal_fini_run as a destructor and call metal_init_run to
   * run and setup Metal constructors */
  la a0, metal_fini_run
  call atexit
  call metal_init_run

_skip_init:

  /* Synchronize harts so that secondary harts wait until hart 0 finishes
     initializing */
  call __metal_synchronize_harts

  /* This is a C runtime, so main() is defined to have some arguments.  Since
   * there's nothing sane the METAL can pass we don't bother with that but
   * instead just setup as close to a NOP as we can. */
  li a0, 1     /* argc=1 */
  la a1, argv  /* argv = {"libgloss", NULL} */
  la a2, envp  /* envp = {NULL} */
  call secondary_main

  /* And here's where we return.  Again, it's a bit odd but the METAL defines
   * this as a bad idea (ie, as opposed to leaving it undefined) and at this
   * point it's really the only thing left to do. */
  mv ra, s0
  ret

  # dst=t1, len=t2-t1
  # next pc: t3
rvv_memset_zero:
    # the copy length
    sub t4, t2, t1
    blez t4, 1f

    vsetvli x0, t4, e8, m8
    vmv.v.x v0, x0
rvv_memset_zero_loop:
    vsetvli t5, t4, e8, m8
    vse8.v v0, (t1)
    sub t4, t4, t5
    add t1, t1, t5
    bnez t4, rvv_memset_zero_loop
1:
  jalr x0, t3, 0


.cfi_endproc

/* RISC-V systems always use __libc_{init,fini}_array, but for compatibility we
 * define _{init,fini} to do nothing. */
.global _init
.type   _init, @function
.global _fini
.type   _fini, @function
_init:
_fini:
  ret
.size _init, .-_init
.size _fini, .-_fini

/* By default, secondary_main will cause secondary harts to spin forever.
 * Users can redefine secondary_main themselves to run code on secondary harts */
.weak   secondary_main
.global secondary_main
.type   secondary_main, @function

secondary_main:
  addi sp, sp, -16
  csrw mscratch, sp
#if __riscv_xlen == 32
  sw ra, 4(sp)
#else
  sd ra, 8(sp)
#endif
  csrr t0, mhartid
  la t1, __metal_boot_hart
  beq t0, t1, 2f
1:
  wfi
  j 1b
2:
  la a0, user_program
  /* If wanna enable mmu, then vm_boot will not return */
  jal vm_boot
user_program:
  call main

  /* Call exit to handle libc's cleanup routines.  Under normal contains this
   * shouldn't even get called, but I'm still not using a tail call here
   * because returning to the METAL is the right thing to do in pathological
   * situations. */
  call exit

#if __riscv_xlen == 32
  lw ra, 4(sp)
#else
  ld ra, 8(sp)
#endif
  addi sp, sp, 16
  ret

#define REGBYTES 8
.global  pop_tf
.type   pop_tf, @function
pop_tf:
  ld  t0,33*REGBYTES(a0)
  csrw  sepc,t0
  ld  x1,1*REGBYTES(a0)
  ld  x2,2*REGBYTES(a0)
  ld  x3,3*REGBYTES(a0)
  ld  x4,4*REGBYTES(a0)
  ld  x5,5*REGBYTES(a0)
  ld  x6,6*REGBYTES(a0)
  ld  x7,7*REGBYTES(a0)
  ld  x8,8*REGBYTES(a0)
  ld  x9,9*REGBYTES(a0)
  ld  x11,11*REGBYTES(a0)
  ld  x12,12*REGBYTES(a0)
  ld  x13,13*REGBYTES(a0)
  ld  x14,14*REGBYTES(a0)
  ld  x15,15*REGBYTES(a0)
  ld  x16,16*REGBYTES(a0)
  ld  x17,17*REGBYTES(a0)
  ld  x18,18*REGBYTES(a0)
  ld  x19,19*REGBYTES(a0)
  ld  x20,20*REGBYTES(a0)
  ld  x21,21*REGBYTES(a0)
  ld  x22,22*REGBYTES(a0)
  ld  x23,23*REGBYTES(a0)
  ld  x24,24*REGBYTES(a0)
  ld  x25,25*REGBYTES(a0)
  ld  x26,26*REGBYTES(a0)
  ld  x27,27*REGBYTES(a0)
  ld  x28,28*REGBYTES(a0)
  ld  x29,29*REGBYTES(a0)
  ld  x30,30*REGBYTES(a0)
  ld  x31,31*REGBYTES(a0)
  ld  a0,10*REGBYTES(a0)
  sret

/* This shim allows main() to be passed a set of arguments that can satisfy the
 * requirements of the C API. */
.section .rodata.libgloss.start
argv:
.dc.a name
envp:
.dc.a 0
name:
.asciz "libgloss"

