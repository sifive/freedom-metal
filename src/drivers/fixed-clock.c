/* Copyright 2018 SiFive, Inc. */
/* SPDX-License-Identifier: Apache-2.0 */

#include <metal/drivers/fixed-clock.h>
#include <stddef.h>

long __metal_driver_fixed_clock_get_rate_hz(const struct metal_clock *gclk)
{
    const struct __metal_driver_fixed_clock *clk = (void *)gclk;
    return clk->rate;
}

long __metal_driver_fixed_clock_set_rate_hz(struct metal_clock *gclk, long target_hz)
{
    return __metal_driver_fixed_clock_get_rate_hz(gclk);
}
