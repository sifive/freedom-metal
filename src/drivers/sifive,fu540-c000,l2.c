/* Copyright 2018 SiFive, Inc */
/* SPDX-License-Identifier: Apache-2.0 */

#include <metal/drivers/__metal_driver_sifive_fu540_c000_l2.h>

#define L2_REG_CONFIG           0x000

#define L2_CONFIG_WAYS_SHIFT    8
#define L2_CONFIG_WAYS_MASK     (0xFF << L2_CONFIG_WAYS_SHIFT)

#define L2_REG_WAYENABLE        0x008

static int fu540_c000_l2_get_max_ways(struct __metal_driver_sifive_fu540_c000_l2 *l2);

void __metal_driver_sifive_fu540_c000_l2_init(struct metal_cache *l2, int ways)
{
    __metal_driver_sifive_fu540_c000_l2_set_enabled_ways(l2, ways);
}

static int fu540_c000_l2_get_max_ways(struct __metal_driver_sifive_fu540_c000_l2 *l2)
{
    return 0;
}

int __metal_driver_sifive_fu540_c000_l2_get_enabled_ways(struct metal_cache *cache)
{
    struct __metal_driver_sifive_fu540_c000_l2 *l2 = (struct __metal_driver_sifive_fu540_c000_l2 *) cache;
    if(!l2) {
        return -1;
    }

    uint32_t way_enable = __METAL_ACCESS_ONCE((__metal_io_u32 *)(l2->control_base + L2_REG_WAYENABLE));

    /* The stored number is the index, so add one */
    return (0xFF & way_enable) + 1;
}

int __metal_driver_sifive_fu540_c000_l2_set_enabled_ways(struct metal_cache *cache, int ways)
{
    struct __metal_driver_sifive_fu540_c000_l2 *l2 = (struct __metal_driver_sifive_fu540_c000_l2 *) cache;
    if(!l2) {
        return -1;
    }

    /* We can't decrease the number of enabled ways */
    if(__metal_driver_sifive_fu540_c000_l2_get_enabled_ways(l2) > ways) {
        return -2;
    }

    /* The stored value is the index, so subtract one */
    uint32_t value = 0xFF & (ways - 1);

    /* Set the number of enabled ways */
    __METAL_ACCESS_ONCE((__metal_io_u32 *)(l2->control_base + L2_REG_WAYENABLE)) = value;

    /* Make sure the number of ways was set correctly */
    if(__metal_driver_sifive_fu540_c000_l2_get_enabled_ways(l2) != ways) {
        return -3;
    }

    return 0;
}
