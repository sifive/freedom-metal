/* Copyright 2018 SiFive, Inc */
/* SPDX-License-Identifier: Apache-2.0 */

#ifndef METAL__DRIVERS__SIFIVE_GPIO0_H
#define METAL__DRIVERS__SIFIVE_GPIO0_H

#include <metal/compiler.h>
#include <metal/gpio.h>

struct __metal_driver_vtable_sifive_gpio0 {
    const struct __metal_gpio_vtable gpio;
};

struct __metal_driver_sifive_gpio0;

int __metal_driver_sifive_gpio0_disable_input(struct metal_gpio *gpio, long source);
long __metal_driver_sifive_gpio0_output(struct metal_gpio *gpio);
int __metal_driver_sifive_gpio0_enable_output(struct metal_gpio *gpio, long source);
int __metal_driver_sifive_gpio0_output_set(struct metal_gpio *gpio, long value);
int __metal_driver_sifive_gpio0_output_clear(struct metal_gpio *gpio, long value);
int __metal_driver_sifive_gpio0_output_toggle(struct metal_gpio *gpio, long value);
int __metal_driver_sifive_gpio0_enable_io(struct metal_gpio *, long source, long dest);

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
    struct metal_interrupt *interrupt_parent;
    const int num_interrupts;
    const int interrupt_lines[];
};

#endif
