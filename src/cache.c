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

void metal_dcache_l1_flush(int hartid)
{
   if (metal_dcache_l1_available(hartid)) {
        __asm__ __volatile__ (".word 0xfc000073" : : : "memory"); // CFLUSH.D.L1
        __asm__ __volatile__ ("fence w ,r" ::: "memory");         // FENCE
    }
}

