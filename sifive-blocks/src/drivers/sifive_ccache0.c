/* Copyright 2019 SiFive, Inc */
/* SPDX-License-Identifier: Apache-2.0 */

#include <metal/platform.h>

#ifdef METAL_SIFIVE_CCACHE0

#include <metal/drivers/sifive_ccache0.h>
#include <metal/init.h>
#include <metal/io.h>

#define L2_CONFIG_WAYS_SHIFT 8
#define L2_CONFIG_WAYS_MASK (0xFF << L2_CONFIG_WAYS_SHIFT)

#define L2_REGW(offset)                                                        \
    __METAL_ACCESS_ONCE(                                                       \
        (__metal_io_u32 *)(METAL_SIFIVE_CCACHE0_0_BASE_ADDRESS + (offset)))

void sifive_ccache0_init(uint32_t ways) {
    sifive_ccache0_set_enabled_ways(ways);
}

METAL_CONSTRUCTOR(init_sifive_ccache0) {
    /* Get the number of available ways per bank */
    const uint32_t ways = __METAL_GET_FIELD(
        L2_REGW(METAL_SIFIVE_CCACHE0_CONFIG), L2_CONFIG_WAYS_MASK);

    /* Enable all the ways */
    sifive_ccache0_init(ways);
}

uint32_t sifive_ccache0_get_enabled_ways(void) {
    const uint32_t way_enable = L2_REGW(METAL_SIFIVE_CCACHE0_WAYENABLE);

    /* The stored number is the index, so add one */
    return (0xFF & way_enable) + 1;
}

uint32_t sifive_ccache0_set_enabled_ways(uint32_t ways) {
    /* We can't decrease the number of enabled ways */
    if (sifive_ccache0_get_enabled_ways() > ways) {
        return sifive_ccache0_get_enabled_ways();
    }

    /* The stored value is the index, so subtract one */
    const uint32_t value = 0xFF & (ways - 1);

    /* Set the number of enabled ways */
    L2_REGW(METAL_SIFIVE_CCACHE0_WAYENABLE) = value;

    return sifive_ccache0_get_enabled_ways();
}

#endif

typedef int no_empty_translation_units;
