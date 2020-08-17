/* Copyright (c) 2020 SiFive, Inc */
/* SPDX-License-Identifier: Apache-2.0 */

#include <metal/platform.h>

#ifdef METAL_SIFIVE_GPIO_BUTTONS

#include <metal/button.h>
#include <metal/gpio.h>
#include <metal/private/metal_private_sifive_gpio_buttons.h>

bool sifive_gpio_buttons_is_pressed(struct metal_button button) {
    if (metal_button_is_none(button))
        return false;
    metal_gpio_enable_input(BUTTON_GPIO(button), BUTTON_GPIO_PIN(button));
    return metal_gpio_get_input_pin(BUTTON_GPIO(button),
                                    BUTTON_GPIO_PIN(button));
}

int sifive_gpio_buttons_enable_interrupt(struct metal_button button) {
    if (metal_button_is_none(button))
        return 0;
    metal_gpio_enable_input(BUTTON_GPIO(button), BUTTON_GPIO_PIN(button));
    return metal_gpio_config_interrupt(
        BUTTON_GPIO(button), BUTTON_GPIO_PIN(button), METAL_GPIO_INT_RISING);
}

int sifive_gpio_buttons_disable_interrupt(struct metal_button button) {
    if (metal_button_is_none(button))
        return 0;
    metal_gpio_enable_input(BUTTON_GPIO(button), BUTTON_GPIO_PIN(button));
    return metal_gpio_config_interrupt(
        BUTTON_GPIO(button), BUTTON_GPIO_PIN(button), METAL_GPIO_INT_DISABLE);
}

int sifive_gpio_buttons_clear_interrupt(struct metal_button button) {
    if (metal_button_is_none(button))
        return 0;
    return metal_gpio_clear_interrupt(
        BUTTON_GPIO(button), BUTTON_GPIO_PIN(button), METAL_GPIO_INT_RISING);
}

#endif

typedef int no_empty_translation_units;
