/* Copyright 2018 SiFive, Inc */
/* SPDX-License-Identifier: Apache-2.0 */

#ifdef METAL_SIFIVE_GPIO_SWITCHES

#include <metal/button.h>
#include <metal/generated/sifive_gpio-buttons.h>
#include <string.h>

static inline uint32_t get_index(struct metal_switch sw) {
    return sw.__switch_index;
}

int metal_switch_has_label(struct metal_switch sw, char *label) {
    if (strcmp(dt_switch_data[get_index(sw)].label, label) == 0) {
        return 1;
    }
    return 0;
}

struct metal_switch metal_switch_get(char *label) {
    for (uint32_t i = 0; i < __METAL_DT_NUM_SWITCHES; i++) {
        struct metal_switch b = (struct metal_switch) { i };
        if (metal_switch_has_label(b, label)) {
            return b;
        }
    }
    assert(0);
    return (struct metal_switch) { 0 };
}

struct metal_interrupt *
metal_switch_interrupt_controller(struct metal_switch sw) {
    return dt_switch_data[get_index(sw)].interrupt_parent;
}

int metal_switch_get_interrupt_id(struct metal_switch sw) {
    return dt_switch_data[get_index(sw)].interrupt_id;
}

#endif

#endif

typedef int no_empty_translation_units;
