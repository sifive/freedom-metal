/* Copyright 2020 SiFive, Inc */
/* SPDX-License-Identifier: Apache-2.0 */

/*
 * Scrub memory with zero
 */

/* Keep it in metal.init section with _enter */
.section .text.metal.init.scrub
/* Disable linker relaxation */
.option push
.option norelax

.type _metal_memory_scrub, @function
_metal_memory_scrub:
    /* Zero out memory */
1:
#if __riscv_xlen == 32
    sw      x0, 0(t1)
    addi    t1, t1, 4
    blt     t1, t2, 1b
#else
    sd      x0, 0(t1)
    addi    t1, t1, 8
    blt     t1, t2, 1b
#endif
    ret

/*
 * Initialize all memories available to zero
 * This must be call before seting up any stack(s)
 */
.weak __metal_before_start
.global __metal_before_start
.type __metal_before_start, @function
__metal_before_start:
    /* Save caller ra */
    mv      s0, ra

    la      t0, __metal_eccscrub_bit
    beqz    t0, skip_scrub

    la      t0, __metal_boot_hart
    csrr    a5, mhartid
    /* Disable machine interrupts to be safe */
    li      a3, 8
    csrc    mstatus, a3
    bne     a5, t0, wait_scrub

    /* Zero out itim memory. */
    .weak metal_itim_0_memory_start
    .weak metal_itim_0_memory_end
    la      t1, metal_itim_0_memory_start
    la      t2, metal_itim_0_memory_end
    beq     t1, t2, dtim_scrub
    jal     _metal_memory_scrub
    .weak metal_itim_1_memory_start
    .weak metal_itim_1_memory_end
    la      t1, metal_itim_1_memory_start
    la      t2, metal_itim_1_memory_end
    beq     t1, t2, dtim_scrub
    jal     _metal_memory_scrub

dtim_scrub:
    /* Zero out dtim memory. */
    .weak metal_dtim_0_memory_start
    .weak metal_dtim_0_memory_end
    la      t1, metal_dtim_0_memory_start
    la      t2, metal_dtim_0_memory_end
    beq     t1, t2, ils_scrub
    jal     _metal_memory_scrub

ils_scrub:
    /* Zero out ils memory. */
    .weak metal_ils_0_memory_start
    .weak metal_ils_0_memory_end
    la      t1, metal_ils_0_memory_start
    la      t2, metal_ils_0_memory_end
    beq     t1, t2, dls_scrub
    jal     _metal_memory_scrub

dls_scrub:
    /* Zero out dls memory. */
    .weak metal_dls_0_memory_start
    .weak metal_dls_0_memory_end
    la      t1, metal_dls_0_memory_start
    la      t2, metal_dls_0_memory_end
    beq     t1, t2, sram_scrub
    jal     _metal_memory_scrub

sram_scrub:
    /* Zero out sram memory. */
    .weak metal_sys_sram_0_memory_start
    .weak metal_sys_sram_0_memory_end
    la      t1, metal_sys_sram_0_memory_start
    la      t2, metal_sys_sram_0_memory_end
    beq     t1, t2, memory_scrub
    jal     _metal_memory_scrub

memory_scrub:
    /* Zero out main memory. */
    .weak metal_memory_0_memory_start
    .weak metal_memory_0_memory_end
    la      t1, metal_memory_0_memory_start
    la      t2, metal_memory_0_memory_end
    beq     t1, t2, done_scrub
    jal     _metal_memory_scrub

done_scrub:
    lui     a4, 0x2000
    li      a5,1
    sw      a5,0(a4)
    fence   w,rw
    j skip_scrub

wait_scrub:
    lui     a4, 0x2000
    lw      a5, 0(a4)
    beqz    a5, wait_scrub

skip_scrub:
    /* Restore caller ra */
    mv      ra, s0
    ret

.option pop
