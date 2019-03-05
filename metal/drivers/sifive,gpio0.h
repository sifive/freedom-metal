/* Copyright 2018 SiFive, Inc */
/* SPDX-License-Identifier: Apache-2.0 */

#ifndef METAL__DRIVERS__SIFIVE_GPIO0_H
#define METAL__DRIVERS__SIFIVE_GPIO0_H

#include <metal/compiler.h>

struct __metal_driver_sifive_gpio0;

int __metal_driver_sifive_gpio0_disable_input(const struct __metal_driver_sifive_gpio0 *gpio, long source);
long __metal_driver_sifive_gpio0_output(const struct __metal_driver_sifive_gpio0 *gpio);
int __metal_driver_sifive_gpio0_enable_output(const struct __metal_driver_sifive_gpio0 *gpio, long source);
int __metal_driver_sifive_gpio0_output_set(const struct __metal_driver_sifive_gpio0 *gpio, long value);
int __metal_driver_sifive_gpio0_output_clear(const struct __metal_driver_sifive_gpio0 *gpio, long value);
int __metal_driver_sifive_gpio0_output_toggle(const struct __metal_driver_sifive_gpio0 *gpio, long value);
int __metal_driver_sifive_gpio0_enable_io(const struct __metal_driver_sifive_gpio0 *, long source, long dest);

struct __metal_driver_vtable_sifive_gpio0 {
    int (*disable_input)(const struct __metal_driver_sifive_gpio0 *, long source);
    long (*output)(const struct __metal_driver_sifive_gpio0 *);
    int (*enable_output)(const struct __metal_driver_sifive_gpio0 *, long source);
    int (*output_set)(const struct __metal_driver_sifive_gpio0 *, long value);
    int (*output_clear)(const struct __metal_driver_sifive_gpio0 *, long value);
    int (*output_toggle)(const struct __metal_driver_sifive_gpio0 *, long value);
    int (*enable_io)(const struct __metal_driver_sifive_gpio0 *, long source, long dest);
};

__METAL_DECLARE_VTABLE(__metal_driver_vtable_sifive_gpio0) = {
    .gpio.disable_input = __metal_driver_sifive_gpio0_disable_input,
    .gpio.output        = __metal_driver_sifive_gpio0_output,
    .gpio.enable_output = __metal_driver_sifive_gpio0_enable_output,
    .gpio.output_set    = __metal_driver_sifive_gpio0_output_set,
    .gpio.output_clear  = __metal_driver_sifive_gpio0_output_clear,
    .gpio.output_toggle = __metal_driver_sifive_gpio0_output_toggle,
    .gpio.enable_io = __metal_driver_sifive_gpio0_enable_io,
};

struct __metal_driver_sifive_gpio0 {
    struct metal_gpio gpio;
    const struct __metal_driver_vtable_sifive_gpio0 *vtable;
    const long base;
    const long size;
    const struct metal_interrupt *interrupt_parent;
    const int num_interrupts;
    const int interrupt_lines[];
};

#endif
