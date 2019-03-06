/* Copyright 2018 SiFive, Inc */
/* SPDX-License-Identifier: Apache-2.0 */

#ifndef METAL__DRIVERS__SIFIVE_FE310_G000_PLL_H
#define METAL__DRIVERS__SIFIVE_FE310_G000_PLL_H

struct __metal_driver_sifive_fe310_g000_pll;

#include <metal/clock.h>
#include <metal/drivers/sifive,fe310-g000,prci.h>

void __metal_driver_sifive_fe310_g000_pll_init(struct __metal_driver_sifive_fe310_g000_pll *pll);
long __metal_driver_sifive_fe310_g000_pll_get_rate_hz(const struct metal_clock *clock);
long __metal_driver_sifive_fe310_g000_pll_set_rate_hz(struct metal_clock *clock, long rate);

struct __metal_driver_vtable_sifive_fe310_g000_pll {
    void (*init)(struct __metal_driver_sifive_fe310_g000_pll *pll);
    struct __metal_clock_vtable clock;
};

__METAL_DECLARE_VTABLE(__metal_driver_vtable_sifive_fe310_g000_pll) = {
    .init = __metal_driver_sifive_fe310_g000_pll_init,
    .clock.get_rate_hz = __metal_driver_sifive_fe310_g000_pll_get_rate_hz,
    .clock.set_rate_hz = __metal_driver_sifive_fe310_g000_pll_set_rate_hz,
};

struct __metal_driver_sifive_fe310_g000_pll {
    struct metal_clock clock;
    const struct __metal_driver_vtable_sifive_fe310_g000_pll *vtable;
    const struct metal_clock *pllsel0;
    const struct metal_clock *pllref;
    const struct __metal_driver_sifive_fe310_g000_prci *config_base;
    const long config_offset;
    const struct __metal_driver_sifive_fe310_g000_prci *divider_base;
    const long divider_offset;
    const long init_rate;
};

#endif
