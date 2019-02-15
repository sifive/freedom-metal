/* Copyright 2018 SiFive, Inc */
/* SPDX-License-Identifier: Apache-2.0 */

#include <string.h>
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
        _led->gpio->vtable->disable_input(_led->gpio, (0x1 << _led->pin));
        _led->gpio->vtable->enable_output(_led->gpio, (0x1 << _led->pin));
    }
}

void __metal_driver_led_on (const struct metal_led *led)
{
    struct __metal_driver_sifive_gpio_led *_led = (void *)(led);

    if (_led->gpio != NULL) {
        _led->gpio->vtable->output_set(_led->gpio, (0x1 << _led->pin));
    }
}

void __metal_driver_led_off (const struct metal_led *led)
{
    struct __metal_driver_sifive_gpio_led *_led = (void *)(led);

    if (_led->gpio != NULL) {
        _led->gpio->vtable->output_clear(_led->gpio, (0x1 << _led->pin));
    }
}

void __metal_driver_led_toggle (const struct metal_led *led)
{
    struct __metal_driver_sifive_gpio_led *_led = (void *)(led);

    if (_led->gpio != NULL) {
        _led->gpio->vtable->output_toggle(_led->gpio, (0x1 << _led->pin));
    }
}

