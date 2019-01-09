/* Copyright 2018 SiFive, Inc */
/* SPDX-License-Identifier: Apache-2.0 */

#include <string.h>
#include <mee/drivers/sifive,gpio-leds.h>

int  __mee_driver_led_exist (struct mee_led *led, char *label)
{
    struct __mee_driver_sifive_gpio_led *_led = (void *)(led);

    if (strcmp(_led->label, label) == 0) {
	return 1;
    }
    return 0;
}

void __mee_driver_led_enable (struct mee_led *led)
{
    struct __mee_driver_sifive_gpio_led *_led = (void *)(led);

    if (_led->gpio != NULL) {
	/* Configure LED as output */
        _led->gpio->vtable->disable_input(_led->gpio, (0x1 << _led->pin));
        _led->gpio->vtable->enable_output(_led->gpio, (0x1 << _led->pin));
    }
}

void __mee_driver_led_on (struct mee_led *led)
{
    struct __mee_driver_sifive_gpio_led *_led = (void *)(led);

    if (_led->gpio != NULL) {
        _led->gpio->vtable->output_set(_led->gpio, (0x1 << _led->pin));
    }
}

void __mee_driver_led_off (struct mee_led *led)
{
    struct __mee_driver_sifive_gpio_led *_led = (void *)(led);

    if (_led->gpio != NULL) {
        _led->gpio->vtable->output_clear(_led->gpio, (0x1 << _led->pin));
    }
}

void __mee_driver_led_toggle (struct mee_led *led)
{
    struct __mee_driver_sifive_gpio_led *_led = (void *)(led);

    if (_led->gpio != NULL) {
        _led->gpio->vtable->output_toggle(_led->gpio, (0x1 << _led->pin));
    }
}

