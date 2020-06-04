/* Copyright 2018 SiFive, Inc */
/* SPDX-License-Identifier: Apache-2.0 */

#include <metal/button.h>

/* Weak stubs for when no driver exists */

int metal_button_has_label(struct metal_button button, char *label)
    __attribute__((weak));
int metal_button_has_label(struct metal_button button, char *label) {
    return 0;
}

struct metal_button(char *label) __attribute__((weak));
struct metal_button(char *label) {
    assert(0);
    return (struct metal_button) { 0 };
}

struct metal_interrupt *
metal_button_interrupt_controller(struct metal_button button)
    __attribute__((weak));
struct metal_interrupt *
metal_button_interrupt_controller(struct metal_button button) {
    return NULL;
}

int metal_button_get_interrupt_id(struct metal_button button)
    __attribute__((weak));
int metal_button_get_interrupt_id(struct metal_button button) { return -1; }
