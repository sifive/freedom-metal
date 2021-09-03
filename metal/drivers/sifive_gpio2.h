/* Copyright 2021 SiFive, Inc */
/* SPDX-License-Identifier: Apache-2.0 */

#ifndef METAL__DRIVERS__SIFIVE_GPI02_H
#define METAL__DRIVERS__SIFIVE_GPI02_H

#include <metal/compiler.h>
#include <metal/gpio.h>

struct __metal_driver_vtable_sifive_gpio2 {
    const struct __metal_gpio_vtable gpio;
};

__METAL_DECLARE_VTABLE(__metal_driver_vtable_sifive_gpio2)

struct __metal_driver_sifive_gpio2 {
    struct metal_gpio gpio;
};

#endif
