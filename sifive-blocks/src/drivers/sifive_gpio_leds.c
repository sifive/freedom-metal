/* Copyright 2020 SiFive, Inc */
/* SPDX-License-Identifier: Apache-2.0 */

#include <metal/platform.h>

#ifdef METAL_SIFIVE_GPIO_LEDS

#include <assert.h>
#include <metal/generated/sifive_gpio_leds.h>
#include <metal/gpio.h>
#include <metal/led.h>
#include <stdbool.h>
#include <string.h>

void sifive_gpio_leds_enable(struct metal_led led) {
    if (metal_led_is_none(led))
        return;
    metal_gpio_disable_input(LED_GPIO(led), LED_GPIO_PIN(led));
    metal_gpio_enable_output(LED_GPIO(led), LED_GPIO_PIN(led));
}

void sifive_gpio_leds_on(struct metal_led led) {
    if (metal_led_is_none(led))
        return;
    metal_gpio_set_pin(LED_GPIO(led), LED_GPIO_PIN(led), 1);
}

void sifive_gpio_leds_off(struct metal_led led) {
    if (metal_led_is_none(led))
        return;
    metal_gpio_set_pin(LED_GPIO(led), LED_GPIO_PIN(led), 0);
}

void sifive_gpio_leds_toggle(struct metal_led led) {
    if (metal_led_is_none(led))
        return;
    metal_gpio_toggle_pin(LED_GPIO(led), LED_GPIO_PIN(led));
}

#endif
