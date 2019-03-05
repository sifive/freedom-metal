/* Copyright 2018 SiFive, Inc */
/* SPDX-License-Identifier: Apache-2.0 */

#include <string.h>
#include <metal/gpio.h>
#include <metal/drivers/sifive,gpio-leds.h>

int  __metal_driver_led_exist (const struct metal_led *led, char *label)
{
    struct __metal_driver_sifive_gpio_led *_led = (void *)(led);

    if (strcmp(_led->label, label) == 0) {
	return 1;
    }
    return 0;
}

void __metal_driver_led_enable (const struct metal_led *led)
{
    struct __metal_driver_sifive_gpio_led *_led = (void *)(led);

    if (_led->gpio != NULL) {
	/* Configure LED as output */
        metal_gpio_disable_input(_led->gpio, _led->pin);
        metal_gpio_enable_output(_led->gpio, _led->pin);
    }
}

void __metal_driver_led_on (const struct metal_led *led)
{
    struct __metal_driver_sifive_gpio_led *_led = (void *)(led);

    if (_led->gpio != NULL) {
        metal_gpio_set_pin(_led->gpio, led_pin, 1);
    }
}

void __metal_driver_led_off (const struct metal_led *led)
{
    struct __metal_driver_sifive_gpio_led *_led = (void *)(led);

    if (_led->gpio != NULL) {
        metal_gpio_set_pin(_led->gpio, led_pin, 0);
    }
}

void __metal_driver_led_toggle (const struct metal_led *led)
{
    struct __metal_driver_sifive_gpio_led *_led = (void *)(led);

    if (_led->gpio != NULL) {
        metal_gpio_toggle_pin(_led->gpio, led_pin);
    }
}

