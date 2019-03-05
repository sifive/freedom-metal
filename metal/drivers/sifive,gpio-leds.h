/* Copyright 2018 SiFive, Inc */
/* SPDX-License-Identifier: Apache-2.0 */

#ifndef METAL__DRIVERS__SIFIVE_GPIO_LEDS_H
#define METAL__DRIVERS__SIFIVE_GPIO_LEDS_H

#include <metal/drivers/sifive,gpio0.h>
#include <metal/led.h>
#include <metal/compiler.h>

struct __metal_driver_vtable_sifive_led {
  const struct metal_led_vtable led_vtable;
};

int  __metal_driver_led_exist(const struct metal_led *led, char *label);
void __metal_driver_led_enable(const struct metal_led *led);
void __metal_driver_led_on(const struct metal_led *led);
void __metal_driver_led_off(const struct metal_led *led);
void __metal_driver_led_toggle(const struct metal_led *led);

__METAL_DECLARE_VTABLE(__metal_driver_vtable_sifive_led) = {
    .led_vtable.led_exist   = __metal_driver_led_exist,
    .led_vtable.led_enable  = __metal_driver_led_enable,
    .led_vtable.led_on      = __metal_driver_led_on,
    .led_vtable.led_off     = __metal_driver_led_off,
    .led_vtable.led_toggle  = __metal_driver_led_toggle,
};

struct __metal_driver_sifive_gpio_led {
    const struct metal_led led;
    const struct __metal_driver_vtable_sifive_led *vtable;
    const struct __metal_driver_sifive_gpio0 *gpio;
    const char *label;
    const int pin;
};

#endif
