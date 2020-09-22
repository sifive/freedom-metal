/* Copyright 2019 SiFive, Inc */
/* SPDX-License-Identifier: Apache-2.0 */

#include <metal/platform.h>

#ifdef METAL_SIFIVE_CCACHE0

#include <assert.h>
#include <metal/drivers/sifive_ccache0.h>
#include <metal/init.h>
#include <metal/io.h>
#include <stddef.h>

#define L2_CONFIG_WAYS_SHIFT 8
#define L2_CONFIG_WAYS_MASK (0xFF << L2_CONFIG_WAYS_SHIFT)

#define L2_REGW(offset)                                                        \
    __METAL_ACCESS_ONCE(                                                       \
        (__metal_io_u32 *)(METAL_SIFIVE_CCACHE0_0_BASE_ADDRESS + (offset)))

/* Linker symbols to calculate LIM allocated size */
extern char metal_segment_lim_target_start, metal_segment_lim_target_end;

void sifive_ccache0_init(uint32_t ways) {
    sifive_ccache0_set_enabled_ways(ways);
}

METAL_CONSTRUCTOR(init_sifive_ccache0) {
    size_t lim_size = 0;
    if (&metal_segment_lim_target_end > &metal_segment_lim_target_start) {
        lim_size =
            &metal_segment_lim_target_end - &metal_segment_lim_target_start;
    }

    const uint32_t available_ways = __METAL_GET_FIELD(
        L2_REGW(METAL_SIFIVE_CCACHE0_CONFIG), L2_CONFIG_WAYS_MASK);

    const size_t way_size = METAL_SIFIVE_CCACHE0_0_CACHE_BLOCK_SIZE *
                            METAL_SIFIVE_CCACHE0_0_CACHE_SETS *
                            METAL_SIFIVE_CCACHE0_0_BANK_COUNT;

    uint32_t ways_to_disable = 0;
    if (way_size != 0) {
        /* Divide lim_size by way_size, rounding up */
        ways_to_disable = (lim_size + way_size - 1) / way_size;
    }

    sifive_ccache0_init(available_ways - ways_to_disable);
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
