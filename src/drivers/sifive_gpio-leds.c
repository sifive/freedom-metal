/* Copyright 2018 SiFive, Inc */
/* SPDX-License-Identifier: Apache-2.0 */

#ifdef METAL_SIFIVE_GPIO_LEDS

#include <metal/generated/sifive_gpio-leds.h>
#include <metal/gpio.h>
#include <string.h>

static inline uint32_t get_index(struct metal_led led) {
    return led.__led_index;
}

int metal_led_has_label(struct metal_led led, char *label) {
    if (strcmp(dt_led_data[get_index(led)].label, label) == 0) {
        return 1;
    }
    return 0;
}

struct metal_led metal_led_get(char *label) {
    return metal_led_get_rgb(label, "");
}

struct metal_led metal_led_get_rgb(char *label, char *color) {
    assert(label != NULL);
    assert(color != NULL);

    char led_label[100];
    strcpy(led_label, label);
    strcat(led_label, color);

    for (uint32_t i = 0; i < __METAL_DT_NUM_LEDS; i++) {
        struct metal_led led = (struct metal_led ) { i };
        if (metal_led_has_label(led, led_label)) {
            return led;
        }
    }

    assert(0);
    return (struct metal_led) { 0 };
}

void metal_led_enable(struct metal_led led) {
    struct metal_gpio gpio = dt_led_data[get_index(led)].gpio;
    uint32_t pin = dt_led_data[get_index(led)].pin;

    /* Configure LED as output */
    metal_gpio_disable_input(gpio, pin);
    metal_gpio_enable_output(gpio, pin);
}

void metal_led_on(struct metal_led led) {
    struct metal_gpio gpio = dt_led_data[get_index(led)].gpio;
    uint32_t pin = dt_led_data[get_index(led)].pin;

    metal_gpio_set_pin(gpio, pin, 1);
}

void metal_led_off(struct metal_led led) {
    struct metal_gpio gpio = dt_led_data[get_index(led)].gpio;
    uint32_t pin = dt_led_data[get_index(led)].pin;

    metal_gpio_set_pin(gpio, pin, 0);
}

void metal_led_toggle(struct metal_led led) {
    struct metal_gpio gpio = dt_led_data[get_index(led)].gpio;
    uint32_t pin = dt_led_data[get_index(led)].pin;

    metal_gpio_toggle_pin(gpio, pin);
}

#endif

typedef int no_empty_translation_units;
