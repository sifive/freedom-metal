/* Copyright 2018 SiFive, Inc */
/* SPDX-License-Identifier: Apache-2.0 */

#include <metal/cache.h>
#include <metal/machine.h>

extern inline void metal_cache_init(struct metal_cache *cache, int ways);
extern inline int metal_cache_get_enabled_ways(struct metal_cache *cache);
extern inline int metal_cache_set_enabled_ways(struct metal_cache *cache, int ways);

int metal_dcache_l1_available(int hartid) {
    switch (hartid) {
    case 0:
#ifdef __METAL_CPU_0_DCACHE_HANDLE
        return __METAL_CPU_0_DCACHE_HANDLE;
#endif  
        break;
    case 1:
#ifdef __METAL_CPU_1_DCACHE_HANDLE
        return __METAL_CPU_1_DCACHE_HANDLE;
#endif
        break;
    case 2:
#ifdef __METAL_CPU_2_DCACHE_HANDLE
        return __METAL_CPU_2_DCACHE_HANDLE;
#endif
        break;
    case 3:
#ifdef __METAL_CPU_3_DCACHE_HANDLE
        return __METAL_CPU_3_DCACHE_HANDLE;
#endif
        break;
    case 4:
#ifdef __METAL_CPU_4_DCACHE_HANDLE
        return __METAL_CPU_4_DCACHE_HANDLE;
#endif
        break;
    case 5:
#ifdef __METAL_CPU_5_DCACHE_HANDLE
        return __METAL_CPU_5_DCACHE_HANDLE;
#endif
        break;
    case 6:
#ifdef __METAL_CPU_6_DCACHE_HANDLE
        return __METAL_CPU_6_DCACHE_HANDLE;
#endif
        break;
    case 7:
#ifdef __METAL_CPU_7_DCACHE_HANDLE
        return __METAL_CPU_7_DCACHE_HANDLE;
#endif
        break;
    case 8:
#ifdef __METAL_CPU_8_DCACHE_HANDLE
        return __METAL_CPU_8_DCACHE_HANDLE;
#endif
        break;
    }
    return 0;
}

/*!
 * @brief CFlush instruction is a custom instruction implemented as a
 * state machine in L1 Data Cache (D$) with funct3=0, (for core with data caches)
 * It is an I type: .insn i opcode, func3, rd, rs1, simm12(signed immediate 12bs)
 * 31     28 27    24 23    20 19     16 15   12 11     8 7      4 3      0
 * |--------|--------|--------|--------|--------|--------|--------|--------|
 * +-------------+------------+----------+------+--------+-----------------+
 * |sign immediate12b (simm12)|   rs1    | func3|    rd  |      opcode     |
 * |-1-1-1-1 -1-1-0-0 -0-0-0-0|-x-x-x-x-x|0-0-0-|-0-0-0-0|-0-1-1-1 -0-0-1-1|
 * +--------------------------+----------+------+--------+-----------------+
 * 31     -0x40              20          15  0  12   x0  7      0x73       0
 * +--------+--------+--------+----------+------+--------+--------+--------+
 * where,
 * rs1 = 0x0, CFLUSH writes back and invalidates all lines in the L1 D$
 * rs1 != x0, CFLUSH writes back and invalidates the L1 D$ line containing the
 *            virtual address in integer register rs1.
 */
void metal_dcache_l1_flush(int hartid, uintptr_t address)
{
   if (metal_dcache_l1_available(hartid)) {
        // Using ‘.insn’ pseudo directive: '.insn i opcode, func3, rd, rs1, simm12'
        __asm__ __volatile__ (".insn i 0x73, 0, x0, %0, -0x40" : : "r" (address));
        __asm__ __volatile__ ("fence.i");         // FENCE
    }
}

