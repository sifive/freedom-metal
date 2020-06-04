/* Copyright 2018 SiFive, Inc */
/* SPDX-License-Identifier: Apache-2.0 */

#include <metal/machine.h>
#include <metal/switch.h>

/* Weak stubs for when no driver exists */

struct metal_switch metal_switch_get(char *label) __attribute__((weak));
struct metal_switch metal_switch_get(char *label) {
    assert(0);
    return (struct metal_switch) { 0 };
}

int metal_switch_has_label(struct metal_switch sw, char *label)
    __attribute__((weak));
int metal_switch_has_label(struct metal_switch sw, char *label) { return 0; }

struct metal_interrupt *
metal_switch_interrupt_controller(struct metal_switch sw)
    __attribute__((weak));
struct metal_interrupt *
metal_switch_interrupt_controller(struct metal_switch sw) {
    return NULL;
}

int metal_switch_get_interrupt_id(struct metal_switch sw)
    __attribute__((weak));
int metal_switch_get_interrupt_id(struct metal_switch sw) { return -1; }
