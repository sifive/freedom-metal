/* Copyright 2018 SiFive, Inc */
/* SPDX-License-Identifier: Apache-2.0 */

#include <string.h>
#include <metal/gpio.h>
#include <metal/drivers/sifive,gpio-leds.h>

int  __metal_driver_led_exist (struct metal_led *led, char *label)
{
    struct __metal_driver_sifive_gpio_led *_led = (void *)(led);

    if (strcmp(_led->label, label) == 0) {
	return 1;
    }
    return 0;
}

void __metal_driver_led_enable (struct metal_led *led)
{
    struct __metal_driver_sifive_gpio_led *_led = (void *)(led);

    if (_led->gpio != NULL) {
	/* Configure LED as output */
        metal_gpio_disable_input((struct metal_gpio *) _led->gpio, _led->pin);
        metal_gpio_enable_output((struct metal_gpio *) _led->gpio, _led->pin);
    }
}

void __metal_driver_led_on (struct metal_led *led)
{
    struct __metal_driver_sifive_gpio_led *_led = (void *)(led);

    if (_led->gpio != NULL) {
        metal_gpio_set_pin((struct metal_gpio *) _led->gpio, _led->pin, 1);
    }
}

void __metal_driver_led_off (struct metal_led *led)
{
    struct __metal_driver_sifive_gpio_led *_led = (void *)(led);

    if (_led->gpio != NULL) {
        metal_gpio_set_pin((struct metal_gpio *) _led->gpio, _led->pin, 0);
    }
}

void __metal_driver_led_toggle (struct metal_led *led)
{
    struct __metal_driver_sifive_gpio_led *_led = (void *)(led);

    if (_led->gpio != NULL) {
        metal_gpio_toggle_pin((struct metal_gpio *) _led->gpio, _led->pin);
    }
}

