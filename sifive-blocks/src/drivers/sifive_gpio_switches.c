/* Copyright (c) 2020 SiFive, Inc */
/* Copyright © 2020 Keith Packard */
/*
 * Licensed under the Apache License, Version 2.0 (the “License”); you
 * may not use this file except in compliance with the License. A copy
 * of this license can be found in the LICENSE.Apache2 file included
 * with this software. You may also obtain a copy of the License at
 *
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an “AS IS” BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or
 * implied. See the License for the specific language governing
 * permissions and limitations under the License.
 */
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
