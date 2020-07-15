/* Copyright 2020 SiFive, Inc */
/* SPDX-License-Identifier: Apache-2.0 */

#include <metal/machine/platform.h>

#ifdef METAL_SIFIVE_GPIO_LEDS

#include <assert.h>
#include <metal/generated/sifive_gpio_leds.h>
#include <metal/gpio.h>
#include <metal/led.h>
#include <stdbool.h>
#include <string.h>

struct metal_led sifive_gpio_leds_get(char *label) {
    assert(label != NULL);
    for(uint32_t i = 0; i < __METAL_DT_NUM_SIFIVE_GPIO_LEDS; i++) {
        struct metal_led led = (struct metal_led) { i };
        if(strcmp(LED_LABEL(led), label) == 0) {
            return led;
        }
    }
    assert(false);
}

struct metal_led sifive_gpio_leds_get_rgb(char *label, char *color) {
    assert(label != NULL);
    assert(color != NULL);

    char led_label[100];
    strncpy(led_label, label, sizeof(led_label));
    led_label[sizeof(led_label) - 1] = '\0';

    /* Append the color to the buffer */
    size_t label_length = strlen(led_label);
    if (sizeof(led_label) > label_length) {
        strncat(led_label, color, sizeof(led_label) - label_length);
    }

    for(uint32_t i = 0; i < __METAL_DT_NUM_SIFIVE_GPIO_LEDS; i++) {
        struct metal_led led = (struct metal_led) { i };
        if(strcmp(LED_LABEL(led), led_label) == 0) {
            return led;
        }
    }
    assert(false);
}

void sifive_gpio_leds_enable(struct metal_led led) {
    metal_gpio_disable_input(LED_GPIO(led), LED_GPIO_PIN(led));
    metal_gpio_enable_output(LED_GPIO(led), LED_GPIO_PIN(led));
}

void sifive_gpio_leds_on(struct metal_led led) {
    metal_gpio_set_pin(LED_GPIO(led), LED_GPIO_PIN(led), 1);
}

void sifive_gpio_leds_off(struct metal_led led) {
    metal_gpio_set_pin(LED_GPIO(led), LED_GPIO_PIN(led), 0);
}

void sifive_gpio_leds_toggle(struct metal_led led) {
    metal_gpio_toggle_pin(LED_GPIO(led), LED_GPIO_PIN(led));
}

#endif
