/* Copyright 2018 SiFive, Inc */
/* SPDX-License-Identifier: Apache-2.0 */

#ifdef METAL_SIFIVE_GPIO_BUTTONS

#include <metal/button.h>
#include <metal/generated/sifive_gpio-buttons.h>
#include <string.h>

static inline uint32_t get_index(struct metal_button button) {
    return button.__button_index;
}

int metal_button_has_label(struct metal_button button, char *label) {
    if (strcmp(dt_button_data[get_index(button)].label, label) == 0) {
        return 1;
    }
    return 0;
}

struct metal_button metal_button_get(char *label) {
    for (uint32_t i = 0; i < __METAL_DT_NUM_BUTTONS; i++) {
        struct metal_button b = (struct metal_button) { i };
        if (metal_button_has_label(b, label)) {
            return b;
        }
    }
    assert(0);
    return (struct metal_button) { 0 };
}

struct metal_interrupt *
metal_button_interrupt_controller(struct metal_button button) {
    return dt_button_data[get_index(button)].interrupt_parent;
}

int metal_button_get_interrupt_id(struct metal_button button) {
    return dt_button_data[get_index(button)].interrupt_id;
}

#endif

typedef int no_empty_translation_units;
