/* Copyright 2018 SiFive, Inc */
/* SPDX-License-Identifier: Apache-2.0 */

#ifndef MEE__DRIVERS__SIFIVE_GPIO_SWITCHES_H
#define MEE__DRIVERS__SIFIVE_GPIO_SWITCHES_H

#include <mee/drivers/sifive,gpio0.h>
#include <mee/switch.h>
#include <mee/compiler.h>

struct __mee_driver_vtable_sifive_switch {
  struct mee_switch_vtable switch_vtable;
};

int  __mee_driver_switch_exist(struct mee_switch *flip, char *label);
struct mee_interrupt*
     __mee_driver_switch_interrupt_controller(struct mee_switch *flip);
int  __mee_driver_switch_get_interrupt_id(struct mee_switch *flip);

__MEE_DECLARE_VTABLE(__mee_driver_vtable_sifive_switch) = {
    .switch_vtable.switch_exist   = __mee_driver_switch_exist,
    .switch_vtable.interrupt_controller = __mee_driver_switch_interrupt_controller,
    .switch_vtable.get_interrupt_id = __mee_driver_switch_get_interrupt_id,
};

struct __mee_driver_sifive_gpio_switch {
    struct mee_switch flip;
    const struct __mee_driver_vtable_sifive_switch *vtable;
    struct mee_interrupt *interrupt_parent;
    struct __mee_driver_sifive_gpio0 *gpio;
    const char *label;
    const int pin;
    const int interrupt_line;
};

#endif
