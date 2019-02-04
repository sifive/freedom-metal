/* Copyright 2018 SiFive, Inc */
/* SPDX-License-Identifier: Apache-2.0 */

#ifndef METAL__DRIVERS__SIFIVE_FU540_C000_L2_H
#define METAL__DRIVERS__SIFIVE_FU540_C000_L2_H

struct __metal_driver_sifive_fu540_c000_l2;

#include <stdint.h>
#include <metal/cache.h>

void __metal_driver_sifive_fu540_c000_l2_init(struct __metal_driver_sifive_fu540_c000_l2 *l2, int ways);
void __metal_driver_sifive_fu540_c000_l2_get_enabled_ways(struct __metal_driver_sifive_fu540_c000_l2 *l2);
void __metal_driver_sifive_fu540_c000_l2_set_enabled_ways(struct __metal_driver_sifive_fu540_c000_l2 *l2, int ways);

struct __metal_driver_vtable_sifive_fu540_c000_l2 {
	void (*init)(struct __metal_driver_sifive_fu540_c000_l2 *l2, int ways);
	int (*get_enabled_ways)(struct __metal_driver_sifive_fu540_c000_l2 *cache);
	int (*set_enabled_ways)(struct __metal_driver_sifive_fu540_c000_l2 *cache, int ways);
}

__METAL_DECLARE_VTABLE(__metal_driver_vtable_sifive_fu540_c000_l2) = {
	.init = __metal_driver_sifive_fu540_c000_l2_init;
	.get_enabled_ways = __metal_driver_sifive_fu540_c000_l2_get_enabled_ways;
	.set_enabled_ways = __metal_driver_sifive_fu540_c000_l2_set_enabled_ways;
};

struct __metal_driver_sifive_fu540_c000_l2 {
	const struct __metal_driver_vtable_sifive_fu540_c000_l2 *vtable;
	const uintptr_t control_base;
};

#endif

