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

extern __inline__ void metal_dcache_l1_flush(uintptr_t address);
extern __inline__ void metal_dcache_l1_discard(uintptr_t address);
