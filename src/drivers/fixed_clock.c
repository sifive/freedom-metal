/* Copyright 2018 SiFive, Inc. */
/* SPDX-License-Identifier: Apache-2.0 */

#include <metal/machine/platform.h>

#ifdef METAL_FIXED_CLOCK

#include <metal/drivers/fixed_clock.h>
#include <metal/generated/fixed_clock.h>

static inline uint32_t get_index(struct metal_clock clk) {
	return clk.__clock_index;
}

uint64_t __metal_driver_fixed_clock_get_rate_hz(struct metal_clock clk) {
	return dt_clock_rate[get_index(clk)];
}

uint64_t __metal_driver_fixed_clock_set_rate_hz(struct metal_clock clk,
                                            uint64_t target_hz) {
	return dt_clock_rate[get_index(clk)];
}

void __metal_driver_fixed_clock_register_pre_rate_change_callback(struct metal_clock clk,
	metal_clock_callback *cb) {}

void __metal_driver_fixed_clock_register_post_rate_change_callback(struct metal_clock clk,
	metal_clock_callback *cb) {}

#endif /* METAL_FIXED_CLOCK */

typedef int no_empty_translation_units;
