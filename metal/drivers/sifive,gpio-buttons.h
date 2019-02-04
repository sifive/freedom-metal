/* Copyright 2018 SiFive, Inc */
/* SPDX-License-Identifier: Apache-2.0 */

#ifndef METAL__DRIVERS__SIFIVE_GPIO_BUTTONS_H
#define METAL__DRIVERS__SIFIVE_GPIO_BUTTONS_H

#include <string.h>
#include <metal/button.h>
#include <metal/compiler.h>

struct __metal_driver_vtable_sifive_button {
  struct metal_button_vtable button_vtable;
};

int  __metal_driver_button_exist(struct metal_button *button, char *label);
struct metal_interrupt*
     __metal_driver_button_interrupt_controller(struct metal_button *button);
int  __metal_driver_button_get_interrupt_id(struct metal_button *button);

__METAL_DECLARE_VTABLE(__metal_driver_vtable_sifive_button) = {
    .button_vtable.button_exist   = __metal_driver_button_exist,
    .button_vtable.interrupt_controller = __metal_driver_button_interrupt_controller,
    .button_vtable.get_interrupt_id = __metal_driver_button_get_interrupt_id,
};

struct __metal_driver_sifive_gpio_button {
    struct metal_button button;
    const struct __metal_driver_vtable_sifive_button *vtable;
    struct metal_interrupt *interrupt_parent;
    struct __metal_driver_sifive_gpio0 *gpio;
    const char *label;
    const int pin;
    const int interrupt_line;
};

#endif
