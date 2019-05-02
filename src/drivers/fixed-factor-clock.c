/* Copyright 2018 SiFive, Inc. */
/* SPDX-License-Identifier: Apache-2.0 */

#include <metal/machine/platform.h>

#ifdef METAL_FIXED_FACTOR_CLOCK

#include <metal/drivers/fixed-factor-clock.h>
#include <stddef.h>

long __metal_driver_fixed_factor_clock_get_rate_hz(const struct metal_clock *gclk)
{
    const struct __metal_driver_fixed_factor_clock *clk = (void *)gclk;

    long parent_rate = 1;
    if(clk->parent) {
        parent_rate = clk->parent->vtable->get_rate_hz(clk->parent);
    }

    return clk->mult * parent_rate / clk->div;
}

long __metal_driver_fixed_factor_clock_set_rate_hz(struct metal_clock *gclk, long target_hz)
{
    return __metal_driver_fixed_factor_clock_get_rate_hz(gclk);
}

#endif /* METAL_FIXED_FACTOR_CLOCK */
