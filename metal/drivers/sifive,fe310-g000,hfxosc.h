/* Copyright 2018 SiFive, Inc */
/* SPDX-License-Identifier: Apache-2.0 */

#ifndef METAL__DRIVERS__SIFIVE_FE310_G000_HFXOSC_H
#define METAL__DRIVERS__SIFIVE_FE310_G000_HFXOSC_H

#include <metal/clock.h>
#include <metal/drivers/sifive,fe310-g000,prci.h>

long __metal_driver_sifive_fe310_g000_hfxosc_get_rate_hz(const struct metal_clock *clock);
long __metal_driver_sifive_fe310_g000_hfxosc_set_rate_hz(struct metal_clock *clock, long rate);

struct __metal_driver_vtable_sifive_fe310_g000_hfxosc {
    struct __metal_clock_vtable clock;
};

__METAL_DECLARE_VTABLE(__metal_driver_vtable_sifive_fe310_g000_hfxosc) = {
    .clock.get_rate_hz = __metal_driver_sifive_fe310_g000_hfxosc_get_rate_hz,
    .clock.set_rate_hz = __metal_driver_sifive_fe310_g000_hfxosc_set_rate_hz,
};

struct __metal_driver_sifive_fe310_g000_hfxosc {
    struct metal_clock clock;
    const struct __metal_driver_vtable_sifive_fe310_g000_hfxosc *vtable;
    const struct metal_clock *ref;
    const struct __metal_driver_sifive_fe310_g000_prci *config_base;
    const long config_offset;
};

#endif
