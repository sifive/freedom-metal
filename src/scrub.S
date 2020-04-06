/* Copyright 2020 SiFive, Inc */
/* SPDX-License-Identifier: Apache-2.0 */

/*
 * Scrub memory with zero
 */
.type _metal_memory_scrub, @function
_metal_memory_scrub:
    /* Zero out memory if exist. */
    bge t1, t2, 2f
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
    mv s0, ra

    /* Zero out itim memory. */
    .weak metal_itim_memory_start
    .weak metal_itim_memory_end
    la t1, metal_itim_memory_start
    la t2, metal_itim_memory_end
    jal _metal_memory_scrub

    /* Zero out dtim memory. */
    .weak metal_dtim_memory_start
    .weak metal_dtim_memory_end
    la t1, metal_dtim_memory_start
    la t2, metal_dtim_memory_end
    jal _metal_memory_scrub

    /* Zero out ils memory. */
    .weak metal_ils_memory_start
    .weak metal_ils_memory_end
    /* Zero out dls memory. */
    .weak metal_dls_memory_start
    .weak metal_dls_memory_end

    /* Zero out main memory. */
    .weak metal_main_memory_start
    .weak metal_main_memory_end
    la t1, metal_main_memory_start
    la t2, metal_main_memory_end
    jal _metal_memory_scrub

    /* Restore caller ra */
    mv ra, s0
    ret
