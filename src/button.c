/* Copyright 2018 SiFive, Inc */
/* SPDX-License-Identifier: Apache-2.0 */

#include <metal/button.h>
#include <metal/machine.h>

int metal_button_has_label(struct metal_button *button, char *label)
    __attribute__((weak));
int metal_button_has_label(struct metal_button *button, char *label) {
    return 0;
}

struct metal_button *metal_button_get(char *label) {
#if __METAL_DT_MAX_BUTTONS > 0
    int i;
    struct metal_button *button;

    if ((__METAL_DT_MAX_BUTTONS == 0) || (label == NULL)) {
        return NULL;
    }

    for (i = 0; i < __METAL_DT_MAX_BUTTONS; i++) {
        button = (struct metal_button *)__metal_button_table[i];
        if (metal_button_has_label(button, label)) {
            return button;
        }
    }
    return NULL;
#else
    return NULL;
#endif
}

/* Weak stubs for when no driver exists */

struct metal_interrupt *
metal_button_interrupt_controller(struct metal_button *button)
    __attribute__((weak));
struct metal_interrupt *
metal_button_interrupt_controller(struct metal_button *button) {
    return NULL;
}

int metal_button_get_interrupt_id(struct metal_button *button)
    __attribute__((weak));
int metal_button_get_interrupt_id(struct metal_button *button) { return -1; }
