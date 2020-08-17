/* Copyright 2018 SiFive, Inc */
/* SPDX-License-Identifier: Apache-2.0 */

#include <metal/cache.h>
#include <metal/cpu.h>
#include <metal/private/metal_private_cache.h>

bool metal_dcache_l1_available(void) {
    return HART_HAS_L1_DCACHE(metal_cpu_get_current_hartid());
}

bool metal_icache_l1_available(void) {
    return HART_HAS_L1_ICACHE(metal_cpu_get_current_hartid());
}

/*!
 * @brief CFlush.D.L1 instruction is a custom instruction implemented as a
 * state machine in L1 Data Cache (D$) with funct3=0, (for core with data
 * caches) It is an I type: .insn i opcode, func3, rd, rs1, simm12(signed
 * immediate 12bs)
 *  31     28 27    24 23    20 19   16 15    12 11     8 7      4 3       0
 * |--------|--------|--------|--------|--------|--------|--------|--------|
 * +-------------+------------+----------+------+--------+-----------------+
 * |sign immediate12b (simm12)|   rs1    | func3|    rd  |      opcode     |
 * |-1-1-1-1 -1-1-0-0 -0-0-0-0|-x-x-x-x-x|0-0-0-|-0-0-0-0|-0-1-1-1 -0-0-1-1|
 * +--------------------------+----------+------+--------+-----------------+
 * 31     -0x40              20        15   0 12   x0     7      0x73      0
 * +--------+--------+--------+----------+------+--------+--------+--------+
 * where,
 * rs1 =  x0, CFLUSH.D.L1 writes back and invalidates all lines in the L1 D$
 * rs1 != x0, CFLUSH.D.L1 writes back and invalidates the L1 D$ line containing
 *            the virtual address in integer register rs1.
 */
void metal_dcache_l1_flush(uintptr_t address) {
    if (metal_dcache_l1_available()) {
        if (address) {
            uintptr_t ms1 = 0, ms2 = 0;
            __asm__ __volatile__("csrr %0, mtvec \n\t"
                                 "la %1, 1f \n\t"
                                 "csrw mtvec, %1 \n\t"
                                 ".insn i 0x73, 0, x0, %2, -0x40 \n\t"
                                 ".align 2\n\t"
                                 "1: \n\t"
                                 "csrw mtvec, %0 \n\t"
                                 : "+r"(ms1), "+r"(ms2)
                                 : "r"(address));
            // Using ‘.insn’ pseudo directive:
            //       '.insn i opcode, func3, rd, rs1, simm12'
        } else {
            __asm__ __volatile__(".word 0xfc000073" : : : "memory");
        }
    }
}

/*!
 * @brief CDiscard.D.L1 instruction is a custom instruction implemented as a
 * state machine in L1 Data Cache (D$) with funct3=0, (for core with data
 * caches) It is an I type: .insn i opcode, func3, rd, rs1, simm12(signed
 * immediate 12bs)
 * 31     28 27    24 23    20 19    16 15    12 11     8 7      4 3       0
 * |--------|--------|--------|--------|--------|--------|--------|--------|
 * +-------------+------------+----------+------+--------+-----------------+
 * |sign immediate12b (simm12)|   rs1    | func3|    rd  |      opcode     |
 * |-1-1-1-1 -1-1-0-0 -0-0-1-0|-x-x-x-x-x|0-0-0-|-0-0-0-0|-0-1-1-1 -0-0-1-1|
 * +--------------------------+----------+------+--------+-----------------+
 * 31     -0x3E              20        15   0 12    x0    7      0x73      0
 * +--------+--------+--------+----------+------+--------+--------+--------+
 * where,
 * rs1 = x0, CDISCARD.D.L1 invalidates all lines in the L1 D$ with no writes
 * back. rs1 != x0, CDISCARD.D.L1 invalidates the L1 D$ line containing the
 * virtual address in integer register rs1, with no writes back.
 */
void metal_dcache_l1_discard(uintptr_t address) {
    if (metal_dcache_l1_available()) {
        if (address) {
            uintptr_t ms1 = 0, ms2 = 0;
            __asm__ __volatile__("csrr %0, mtvec \n\t"
                                 "la %1, 1f \n\t"
                                 "csrw mtvec, %1 \n\t"
                                 ".insn i 0x73, 0, x0, %2, -0x3E \n\t"
                                 ".align 2\n\t"
                                 "1: \n\t"
                                 "csrw mtvec, %0 \n\t"
                                 : "+r"(ms1), "+r"(ms2)
                                 : "r"(address));
            // Using ‘.insn’ pseudo directive:
            //       '.insn i opcode, func3, rd, rs1, simm12'
        } else {
            __asm__ __volatile__(".word 0xfc200073" : : : "memory");
        }
    }
}
