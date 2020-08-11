/* Copyright (c) 2020 SiFive, Inc */
/* SPDX-License-Identifier: Apache-2.0 */

#include <metal/platform.h>

#ifdef METAL_SIFIVE_GPIO_SWITCHES

#include <metal/private/metal_private_sifive_gpio_switches.h>
#include <metal/switch.h>

bool sifive_gpio_switches_is_pressed(struct metal_switch sw) {
    if (metal_switch_is_none(sw))
        return false;
    if (SWITCH_HAS_GPIO(sw)) {
        metal_gpio_enable_input(SWITCH_GPIO(sw), SWITCH_GPIO_PIN(sw));
        return metal_gpio_get_input_pin(SWITCH_GPIO(sw), SWITCH_GPIO_PIN(sw));
    }
    return false;
}

int sifive_gpio_switches_enable_interrupt(struct metal_switch sw) {
    if (metal_switch_is_none(sw))
        return -1;
    if (SWITCH_HAS_GPIO(sw)) {
        metal_gpio_enable_input(SWITCH_GPIO(sw), SWITCH_GPIO_PIN(sw));
        return metal_gpio_config_interrupt(SWITCH_GPIO(sw), SWITCH_GPIO_PIN(sw),
                                           METAL_GPIO_INT_RISING);
    }
    return metal_interrupt_enable(SWITCH_INTC(sw), SWITCH_INTERRUPT_ID(sw));
}

int sifive_gpio_switches_disable_interrupt(struct metal_switch sw) {
    if (metal_switch_is_none(sw))
        return -1;
    if (SWITCH_HAS_GPIO(sw)) {
        metal_gpio_enable_input(SWITCH_GPIO(sw), SWITCH_GPIO_PIN(sw));
        return metal_gpio_config_interrupt(SWITCH_GPIO(sw), SWITCH_GPIO_PIN(sw),
                                           METAL_GPIO_INT_DISABLE);
    }
    return metal_interrupt_disable(SWITCH_INTC(sw), SWITCH_INTERRUPT_ID(sw));
}

int sifive_gpio_switches_clear_interrupt(struct metal_switch sw) {
    if (metal_switch_is_none(sw))
        return -1;
    if (SWITCH_HAS_GPIO(sw)) {
        return metal_gpio_clear_interrupt(SWITCH_GPIO(sw), SWITCH_GPIO_PIN(sw),
                                          METAL_GPIO_INT_RISING);
    }
    return 0;
}

#endif

typedef int no_empty_translation_units;
