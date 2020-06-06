/* Copyright 2018 SiFive, Inc */
/* SPDX-License-Identifier: Apache-2.0 */

#ifndef METAL__DRIVERS__SIFIVE_NB2WDT_H
#define METAL__DRIVERS__SIFIVE_NB2WDT_H

#include <metal/io.h>
#include <metal/compiler.h>
#include <metal/watchdog.h>
#include <metal/clock.h>
#include <metal/interrupt.h>

#define WDT_REGW(base,offset)   (__METAL_ACCESS_ONCE((uint32_t *)(base + offset)))

struct __metal_driver_vtable_sifive_nb2wdt {
    const struct metal_watchdog_vtable watchdog;
};

struct __metal_driver_sifive_nb2wdt;

__METAL_DECLARE_VTABLE(__metal_driver_vtable_sifive_nb2wdt);

struct __metal_driver_sifive_nb2wdt {
    const struct metal_watchdog watchdog;
};

#endif
