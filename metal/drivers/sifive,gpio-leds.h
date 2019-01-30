/* Copyright 2018 SiFive, Inc */
/* SPDX-License-Identifier: Apache-2.0 */

#ifndef MEE__DRIVERS__SIFIVE_GPIO_LEDS_H
#define MEE__DRIVERS__SIFIVE_GPIO_LEDS_H

#include <mee/drivers/sifive,gpio0.h>
#include <mee/led.h>
#include <mee/compiler.h>

struct __mee_driver_vtable_sifive_led {
  struct mee_led_vtable led_vtable;
};

int  __mee_driver_led_exist(struct mee_led *led, char *label);
void __mee_driver_led_enable(struct mee_led *led);
void __mee_driver_led_on(struct mee_led *led);
void __mee_driver_led_off(struct mee_led *led);
void __mee_driver_led_toggle(struct mee_led *led);

__MEE_DECLARE_VTABLE(__mee_driver_vtable_sifive_led) = {
    .led_vtable.led_exist   = __mee_driver_led_exist,
    .led_vtable.led_enable  = __mee_driver_led_enable,
    .led_vtable.led_on      = __mee_driver_led_on,
    .led_vtable.led_off     = __mee_driver_led_off,
    .led_vtable.led_toggle  = __mee_driver_led_toggle,
};

struct __mee_driver_sifive_gpio_led {
    struct mee_led led;
    const struct __mee_driver_vtable_sifive_led *vtable;
    struct __mee_driver_sifive_gpio0 *gpio;
    const char *label;
    const int pin;
};

#endif
