/* Copyright 2018 SiFive, Inc */
/* SPDX-License-Identifier: Apache-2.0 */

#ifndef MEE__DRIVERS__SIFIVE_GPIO0_H
#define MEE__DRIVERS__SIFIVE_GPIO0_H

#include <mee/compiler.h>

struct __mee_driver_sifive_gpio0;

long __mee_driver_sifive_gpio0_disable_input(const struct __mee_driver_sifive_gpio0 *gpio, long source);
long __mee_driver_sifive_gpio0_output(const struct __mee_driver_sifive_gpio0 *gpio);
long __mee_driver_sifive_gpio0_enable_output(const struct __mee_driver_sifive_gpio0 *gpio, long source);
long __mee_driver_sifive_gpio0_output_set(const struct __mee_driver_sifive_gpio0 *gpio, long value);
long __mee_driver_sifive_gpio0_output_clear(const struct __mee_driver_sifive_gpio0 *gpio, long value);
long __mee_driver_sifive_gpio0_output_toggle(const struct __mee_driver_sifive_gpio0 *gpio, long value);
long __mee_driver_sifive_gpio0_enable_io(const struct __mee_driver_sifive_gpio0 *, long source, long dest);

struct __mee_driver_vtable_sifive_gpio0 {
    long (*disable_input)(const struct __mee_driver_sifive_gpio0 *, long source);
    long (*output)(const struct __mee_driver_sifive_gpio0 *);
    long (*enable_output)(const struct __mee_driver_sifive_gpio0 *, long source);
    long (*output_set)(const struct __mee_driver_sifive_gpio0 *, long value);
    long (*output_clear)(const struct __mee_driver_sifive_gpio0 *, long value);
    long (*output_toggle)(const struct __mee_driver_sifive_gpio0 *, long value);
    long (*enable_io)(const struct __mee_driver_sifive_gpio0 *, long source, long dest);
};

__MEE_DECLARE_VTABLE(__mee_driver_vtable_sifive_gpio0) = {
    .disable_input = __mee_driver_sifive_gpio0_disable_input,
    .output        = __mee_driver_sifive_gpio0_output,
    .enable_output = __mee_driver_sifive_gpio0_enable_output,
    .output_set    = __mee_driver_sifive_gpio0_output_set,
    .output_clear  = __mee_driver_sifive_gpio0_output_clear,
    .output_toggle = __mee_driver_sifive_gpio0_output_toggle,
    .enable_io = __mee_driver_sifive_gpio0_enable_io,
};

struct __mee_driver_sifive_gpio0 {
    const struct __mee_driver_vtable_sifive_gpio0 *vtable;
    const long base;
    const long size;
    struct mee_interrupt *interrupt_parent;
    const int num_interrupts;
    const int interrupt_lines[];
};

#endif
