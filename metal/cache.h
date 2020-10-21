/* Copyright 2018 SiFive, Inc */
/* SPDX-License-Identifier: Apache-2.0 */

#ifndef METAL__CACHE_H
#define METAL__CACHE_H

/*!
 * @file cache.h
 *
 * @brief API for configuring caches
 */
#include <stdbool.h>
#include <stdint.h>

/*!
 * @brief Check if dcache is supported on the core
 * @return 1 if dcache is present
 */
bool metal_dcache_l1_available(void);

/*!
 * @brief Flush dcache for L1 on the current core with write back
 * @param address The virtual address of cacheline to invalidate
 * @return None
 */
static __inline__ void metal_dcache_l1_flush(uintptr_t address) {
    /*!
     * CFlush.D.L1 instruction is a custom instruction implemented as a
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
     * rs1 != x0, CFLUSH.D.L1 writes back and invalidates the L1 D$ line
     * containing the virtual address in integer register rs1.
     */
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

/*!
 * @brief Discard dcache for L1 on the current core with no write back
 * @param address The virtual address of cacheline to invalidate
 * @return None
 */
static __inline__ void metal_dcache_l1_discard(uintptr_t address) {
    /*!
     * CDiscard.D.L1 instruction is a custom instruction implemented as a
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

/*!
 * @brief Check if icache is supported on the core
 * @return 1 if icache is present
 */
bool metal_icache_l1_available(void);

#endif
