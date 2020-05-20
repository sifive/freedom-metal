/* Copyright 2018 SiFive, Inc */
/* SPDX-License-Identifier: Apache-2.0 */

#include <metal/machine.h>
#include <metal/switch.h>

struct metal_switch *metal_switch_get(char *label) {
    int i;
    struct metal_switch *flip;

    if ((__METAL_DT_MAX_BUTTONS == 0) || (label == NULL)) {
        return NULL;
    }

    for (i = 0; i < __METAL_DT_MAX_BUTTONS; i++) {
        flip = (struct metal_switch *)__metal_switch_table[i];
        if (metal_switch_has_label(flip, label)) {
            return flip;
        }
    }
    return NULL;
}

/* Weak stubs for when no driver exists */

int metal_switch_has_label(struct metal_switch *flip, char *label)
    __attribute__((weak));
int metal_switch_has_label(struct metal_switch *flip, char *label) { return 0; }

struct metal_interrupt *
metal_switch_interrupt_controller(struct metal_switch *flip)
    __attribute__((weak));
struct metal_interrupt *
metal_switch_interrupt_controller(struct metal_switch *flip) {
    return NULL;
}

int metal_switch_get_interrupt_id(struct metal_switch *flip)
    __attribute__((weak));
int metal_switch_get_interrupt_id(struct metal_switch *flip) { return -1; }
