/* Copyright 2018 SiFive, Inc. */
/* SPDX-License-Identifier: Apache-2.0 */

#include <metal/machine/platform.h>

#ifdef METAL_FIXED_FACTOR_CLOCK

#include <metal/drivers/fixed_factor_clock.h>
#include <metal/generated/fixed_factor_clock.h>

static inline uint32_t get_index(struct metal_clock clk) {
    return clk.__clock_index;
}

uint64_t __metal_driver_fixed_factor_clock_get_rate_hz(
    struct metal_clock clk) {

    /* Call get_rate_hz for the parent clock defined in the Devicetree */
    struct metal_clock ref = dt_clock_data[get_index(clk)].ref;
    uint64_t parent_rate = metal_clock_get_rate_hz(ref);

    return dt_clock_data[get_index(clk)].mul * parent_rate /
           dt_clock_data[get_index(clk)].mul;
}

uint64_t __metal_driver_fixed_factor_clock_set_rate_hz(struct metal_clock clk,
                                                       uint64_t target_hz) {
    return __metal_driver_fixed_factor_clock_get_rate_hz(clk);
}

void __metal_driver_fixed_factor_clock_register_pre_rate_change_callback(struct metal_clock clk,
    metal_clock_callback *cb) {}

void __metal_driver_fixed_factor_clock_register_post_rate_change_callback(struct metal_clock clk,
    metal_clock_callback *cb) {}

#endif /* METAL_FIXED_FACTOR_CLOCK */

typedef int no_empty_translation_units;
