/* Copyright 2018 SiFive, Inc */
/* SPDX-License-Identifier: Apache-2.0 */

#ifndef METAL__DRIVERS__SIFIVE_GPIO_SWITCHES_H
#define METAL__DRIVERS__SIFIVE_GPIO_SWITCHES_H

#include <metal/drivers/sifive,gpio0.h>
#include <metal/switch.h>
#include <metal/compiler.h>

struct __metal_driver_vtable_sifive_switch {
  struct metal_switch_vtable switch_vtable;
};

int  __metal_driver_switch_exist(struct metal_switch *flip, char *label);
struct metal_interrupt*
     __metal_driver_switch_interrupt_controller(struct metal_switch *flip);
int  __metal_driver_switch_get_interrupt_id(struct metal_switch *flip);

__METAL_DECLARE_VTABLE(__metal_driver_vtable_sifive_switch) = {
    .switch_vtable.switch_exist   = __metal_driver_switch_exist,
    .switch_vtable.interrupt_controller = __metal_driver_switch_interrupt_controller,
    .switch_vtable.get_interrupt_id = __metal_driver_switch_get_interrupt_id,
};

struct __metal_driver_sifive_gpio_switch {
    struct metal_switch flip;
    const struct __metal_driver_vtable_sifive_switch *vtable;
    struct metal_interrupt *interrupt_parent;
    struct __metal_driver_sifive_gpio0 *gpio;
    const char *label;
    const int pin;
    const int interrupt_line;
};

#endif
