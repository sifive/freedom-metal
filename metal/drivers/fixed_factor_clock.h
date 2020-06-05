/* Copyright 2020 SiFive, Inc. */
/* SPDX-License-Identifier: Apache-2.0 */

#ifndef __METAL__FIXED_FACTOR_CLOCK_H
#define __METAL__FIXED_FACTOR_CLOCK_H

uint64_t __metal_driver_fixed_factor_clock_get_rate_hz(struct metal_clock clk);

uint64_t __metal_driver_fixed_factor_clock_set_rate_hz(struct metal_clock clk,
                                            uint64_t target_hz);

void __metal_driver_fixed_factor_clock_register_pre_rate_change_callback(struct metal_clock clk,
	metal_clock_callback *cb);

void __metal_driver_fixed_factor_clock_register_post_rate_change_callback(struct metal_clock clk,
	metal_clock_callback *cb);

#endif /* __METAL__FIXED_FACTOR_CLOCK_H */
