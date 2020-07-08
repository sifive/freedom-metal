/* Copyright 2019 SiFive, Inc */
/* SPDX-License-Identifier: Apache-2.0 */

#ifndef METAL__DRIVERS__SIFIVE_FE310_G000_LFROSC_H
#define METAL__DRIVERS__SIFIVE_FE310_G000_LFROSC_H

#include <metal/clock.h>

uint64_t __metal_driver_sifive_fe310_g000_lfrosc_get_rate_hz(
    struct metal_clock clock);

uint64_t __metal_driver_sifive_fe310_g000_lfrosc_set_rate_hz(
    struct metal_clock clock, uint64_t rate);

#endif
