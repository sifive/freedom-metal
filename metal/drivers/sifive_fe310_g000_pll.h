/* Copyright 2020 SiFive, Inc */
/* SPDX-License-Identifier: Apache-2.0 */

#ifndef METAL__DRIVERS__SIFIVE_FE310_G000_PLL_H
#define METAL__DRIVERS__SIFIVE_FE310_G000_PLL_H

#include <metal/clock.h>

uint64_t __metal_driver_sifive_fe310_g000_pll_get_rate_hz(struct metal_clock clock);
uint64_t __metal_driver_sifive_fe310_g000_pll_set_rate_hz(struct metal_clock clock);
uint64_t __metal_driver_sifive_fe310_g000_pll_register_pre_rate_change_callback(struct metal_clock clock);
uint64_t __metal_driver_sifive_fe310_g000_pll_register_post_rate_change_callback(struct metal_clock clock);

#endif
