/* Copyright 2018 SiFive, Inc. */
/* SPDX-License-Identifier: Apache-2.0 */

#include <metal/platform.h>

#ifdef METAL_FIXED_CLOCK

#include <metal/drivers/fixed_clock.h>
#include <metal/private/metal_private_fixed_clock.h>

#define get_index(clk) ((clk).__clock_index)

uint64_t fixed_clock_get_rate_hz(struct metal_clock clk) {
    return dt_clock_rate[get_index(clk)];
}

uint64_t fixed_clock_set_rate_hz(struct metal_clock clk, uint64_t target_hz) {
    return dt_clock_rate[get_index(clk)];
}

#endif /* METAL_FIXED_CLOCK */

typedef int no_empty_translation_units;
