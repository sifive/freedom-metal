/* Copyright 2019 SiFive, Inc */
/* SPDX-License-Identifier: Apache-2.0 */

#include <metal/gpio.h>

/* Weak stubs for when no driver exists */

int metal_gpio_disable_input(struct metal_gpio gpio, int pin)
    __attribute__((weak));
int metal_gpio_disable_input(struct metal_gpio gpio, int pin) { return -1; }

int metal_gpio_enable_input(struct metal_gpio gpio, int pin)
    __attribute__((weak));
int metal_gpio_enable_input(struct metal_gpio gpio, int pin) { return -1; }

int metal_gpio_enable_output(struct metal_gpio gpio, int pin)
    __attribute__((weak));
int metal_gpio_enable_output(struct metal_gpio gpio, int pin) { return -1; }

int metal_gpio_disable_output(struct metal_gpio gpio, int pin)
    __attribute__((weak));
int metal_gpio_disable_output(struct metal_gpio gpio, int pin) { return -1; }

int metal_gpio_get_output_pin(struct metal_gpio gpio, int pin)
    __attribute__((weak));
int metal_gpio_get_output_pin(struct metal_gpio gpio, int pin) { return -1; }

int metal_gpio_get_input_pin(struct metal_gpio gpio, int pin)
    __attribute__((weak));
int metal_gpio_get_input_pin(struct metal_gpio gpio, int pin) { return -1; }

int metal_gpio_set_pin(struct metal_gpio gpio, int pin, int value)
    __attribute__((weak));
int metal_gpio_set_pin(struct metal_gpio gpio, int pin, int value) {
    return -1;
}

int metal_gpio_clear_pin(struct metal_gpio gpio, int pin)
    __attribute__((weak));
int metal_gpio_clear_pin(struct metal_gpio gpio, int pin) { return -1; }

int metal_gpio_toggle_pin(struct metal_gpio gpio, int pin)
    __attribute__((weak));
int metal_gpio_toggle_pin(struct metal_gpio gpio, int pin) { return -1; }

int metal_gpio_enable_pinmux(struct metal_gpio gpio, long pin_mask,
                             long io_function_mask) __attribute__((weak));
int metal_gpio_enable_pinmux(struct metal_gpio gpio, long pin_mask,
                             long io_function_mask) {
    return -1;
}

int metal_gpio_disable_pinmux(struct metal_gpio gpio, long pin_mask)
    __attribute__((weak));
int metal_gpio_disable_pinmux(struct metal_gpio gpio, long pin_mask) {
    return -1;
}

int metal_gpio_config_interrupt(struct metal_gpio gpio, int pin, enum metal_gpio_int_type int_type)
    __attribute__((weak));
int metal_gpio_config_interrupt(struct metal_gpio gpio, int pin,
                                enum metal_gpio_int_type int_type) {
    return -1;
}

int metal_gpio_clear_interrupt(struct metal_gpio gpio, int pin, enum metal_gpio_int_type int_type)
    __attribute__((weak));
int metal_gpio_clear_interrupt(struct metal_gpio gpio, int pin,
                               enum metal_gpio_int_type int_type) {
    return -1;
}
