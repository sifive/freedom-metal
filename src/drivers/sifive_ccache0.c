/* Copyright 2019 SiFive, Inc */
/* SPDX-License-Identifier: Apache-2.0 */

#ifdef METAL_SIFIVE_CCACHE0

#include <metal/generated/sifive_ccache0.h>
#include <metal/init.h>
#include <metal/io.h>

#define L2_CONFIG_WAYS_SHIFT 8
#define L2_CONFIG_WAYS_MASK (0xFF << L2_CONFIG_WAYS_SHIFT)

static inline get_index(struct metal_cache cache) {
    return cache.__cache.index;
}

METAL_CONSTRUCTOR(metal_driver_sifive_ccache0_init) {
    for(uint32_t i = 0; i < __METAL_DT_NUM_CACHES; i++) {
        /* Get the handle for the L2 cache controller */
        struct metal_cache cache = (struct metal_cache) { i };

        /* Get the number of available ways per bank */
        unsigned long base = dt_cache_data[get_index(cache)].base_addr;
        uint32_t ways = __METAL_ACCESS_ONCE(
            (__metal_io_u32 *)(base + METAL_SIFIVE_CCACHE0_CONFIG));
        ways = ((ways & cache_CONFIG_WAYS_MASK) >> L2_CONFIG_WAYS_SHIFT);

        /* Enable all the ways */
        metal_cache_init(l2, ways);
    }
}

void metal_cache_init(struct metal_cache l2, int ways) {
    metal_cache_set_enabled_ways(l2, ways);
}

int metal_cache_get_enabled_ways(struct metal_cache cache) {
    unsigned long base =
        dt_cache_data[get_index(cache)].base_addr;

    uint32_t way_enable = __METAL_ACCESS_ONCE(
        (__metal_io_u32 *)(base + METAL_SIFIVE_CCACHE0_WAYENABLE));

    /* The stored number is the index, so add one */
    return (0xFF & way_enable) + 1;
}

int metal_cache_set_enabled_ways(struct metal_cache cache, int ways) {
    unsigned long base =
        dt_cache_data[get_index(cache)].base_addr;

    /* We can't decrease the number of enabled ways */
    if (metal_cache_get_enabled_ways(cache) > ways) {
        return -2;
    }

    /* The stored value is the index, so subtract one */
    uint32_t value = 0xFF & (ways - 1);

    /* Set the number of enabled ways */
    __METAL_ACCESS_ONCE(
        (__metal_io_u32 *)(base + METAL_SIFIVE_CCACHE0_WAYENABLE)) =
        value;

    /* Make sure the number of ways was set correctly */
    if (metal_cache_get_enabled_ways(cache) != ways) {
        return -3;
    }

    return 0;
}

#endif

typedef int no_empty_translation_units;
