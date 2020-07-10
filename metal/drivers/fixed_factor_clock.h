/* Copyright 2020 SiFive, Inc. */
/* SPDX-License-Identifier: Apache-2.0 */

#ifndef __METAL__FIXED_FACTOR_CLOCK_H
#define __METAL__FIXED_FACTOR_CLOCK_H

#include <metal/clock.h>
#include <stdint.h>

uint64_t fixed_factor_clock_get_rate_hz(struct metal_clock clk);

uint64_t fixed_factor_clock_set_rate_hz(struct metal_clock clk,
                                            uint64_t target_hz);

#endif /* __METAL__FIXED_FACTOR_CLOCK_H */
