/* Copyright 2018 SiFive, Inc */
/* SPDX-License-Identifier: Apache-2.0 */

#ifndef METAL__DRIVERS__FIXED_CLOCK_H
#define METAL__DRIVERS__FIXED_CLOCK_H

struct __metal_driver_fixed_clock;

#include <metal/compiler.h>
#include <metal/clock.h>

struct __metal_driver_vtable_fixed_clock {
    struct __metal_clock_vtable clock;
};

long __metal_driver_fixed_clock_get_rate_hz(const struct metal_clock *gclk);
long __metal_driver_fixed_clock_set_rate_hz(struct metal_clock *gclk, long target_hz);

__METAL_DECLARE_VTABLE(__metal_driver_vtable_fixed_clock) = {
    .clock.get_rate_hz = __metal_driver_fixed_clock_get_rate_hz,
    .clock.set_rate_hz = __metal_driver_fixed_clock_set_rate_hz,
};

struct __metal_driver_fixed_clock {
    struct metal_clock clock;
    const struct __metal_driver_vtable_fixed_clock *vtable;
    long rate;
};

#endif
