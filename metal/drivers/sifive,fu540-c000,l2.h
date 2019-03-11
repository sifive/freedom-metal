/* Copyright 2018 SiFive, Inc */
/* SPDX-License-Identifier: Apache-2.0 */

#ifndef METAL__DRIVERS__SIFIVE_FU540_C000_L2_H
#define METAL__DRIVERS__SIFIVE_FU540_C000_L2_H

struct __metal_driver_sifive_fu540_c000_l2;

#include <stdint.h>
#include <metal/cache.h>

void __metal_driver_sifive_fu540_c000_l2_init(struct metal_cache *l2, int ways);
int __metal_driver_sifive_fu540_c000_l2_get_enabled_ways(struct metal_cache *l2);
int __metal_driver_sifive_fu540_c000_l2_set_enabled_ways(struct metal_cache *l2, int ways);

struct __metal_driver_vtable_sifive_fu540_c000_l2 {
	struct __metal_cache_vtable cache;
};

__METAL_DECLARE_VTABLE(__metal_driver_vtable_sifive_fu540_c000_l2) = {
	.cache.init = __metal_driver_sifive_fu540_c000_l2_init,
	.cache.get_enabled_ways = __metal_driver_sifive_fu540_c000_l2_get_enabled_ways,
	.cache.set_enabled_ways = __metal_driver_sifive_fu540_c000_l2_set_enabled_ways,
};

struct __metal_driver_sifive_fu540_c000_l2 {
	struct metal_cache cache;
	const struct __metal_driver_vtable_sifive_fu540_c000_l2 *vtable;
	const uintptr_t control_base;
};

#endif

