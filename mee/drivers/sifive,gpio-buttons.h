/* Copyright 2018 SiFive, Inc */
/* SPDX-License-Identifier: Apache-2.0 */

#ifndef MEE__DRIVERS__SIFIVE_GPIO_BUTTONS_H
#define MEE__DRIVERS__SIFIVE_GPIO_BUTTONS_H

#include <string.h>
#include <mee/button.h>
#include <mee/compiler.h>

struct __mee_driver_vtable_sifive_button {
  struct mee_button_vtable button_vtable;
};

int  __mee_driver_button_exist(struct mee_button *button, char *label);
struct mee_interrupt*
     __mee_driver_button_interrupt_controller(struct mee_button *button);
int  __mee_driver_button_get_interrupt_id(struct mee_button *button);

__MEE_DECLARE_VTABLE(__mee_driver_vtable_sifive_button) = {
    .button_vtable.button_exist   = __mee_driver_button_exist,
    .button_vtable.interrupt_controller = __mee_driver_button_interrupt_controller,
    .button_vtable.get_interrupt_id = __mee_driver_button_get_interrupt_id,
};

struct __mee_driver_sifive_gpio_button {
    struct mee_button button;
    const struct __mee_driver_vtable_sifive_button *vtable;
    struct mee_interrupt *interrupt_parent;
    struct __mee_driver_sifive_gpio0 *gpio;
    const char *label;
    const int pin;
    const int interrupt_line;
};

#endif
