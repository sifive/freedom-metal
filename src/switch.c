/* Copyright 2018 SiFive, Inc */
/* SPDX-License-Identifier: Apache-2.0 */

#include <assert.h>
#include <metal/generated/switch.h>
#include <metal/switch.h>

bool metal_switch_is_on(struct metal_switch sw) __attribute__((weak));
bool metal_switch_is_on(struct metal_switch sw) { return false; }

int metal_switch_enable_interrupt(struct metal_switch sw) __attribute__((weak));
int metal_switch_enable_interrupt(struct metal_switch sw) { return -1; }

int metal_switch_disable_interrupt(struct metal_switch sw)
    __attribute__((weak));
int metal_switch_disable_interrupt(struct metal_switch sw) { return -1; }

__attribute__((weak)) int metal_switch_clear_interrupt(struct metal_switch sw) {
    return -1;
}
