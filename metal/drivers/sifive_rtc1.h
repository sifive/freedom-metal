/* Copyright 2019 SiFive, Inc */
/* SPDX-License-Identifier: Apache-2.0 */

#ifndef METAL__DRIVERS__SIFIVE_RTC1_H
#define METAL__DRIVERS__SIFIVE_RTC1_H

#include <metal/compiler.h>
#include <metal/io.h>

#include <metal/clock.h>
#include <metal/interrupt.h>
#include <metal/rtc.h>

struct __metal_driver_vtable_sifive_rtc1 {
    const struct metal_rtc_vtable rtc;
};

struct __metal_driver_sifive_rtc1;

__METAL_DECLARE_VTABLE(__metal_driver_vtable_sifive_rtc1)

struct __metal_driver_sifive_rtc1 {
    const struct metal_rtc rtc;
};

#endif
