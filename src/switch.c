/* Copyright 2018 SiFive, Inc */
/* SPDX-License-Identifier: Apache-2.0 */

#include <assert.h>
#include <metal/generated/switch.h>
#include <metal/switch.h>

struct metal_switch metal_switch_get(uint32_t index) {
    assert(index < __METAL_DT_NUM_SWITCHES);
    return (struct metal_switch) { index };
}

bool metal_switch_is_on(struct metal_switch sw) __attribute__((weak));
bool metal_switch_is_on(struct metal_switch sw) { return false; }

int metal_switch_enable_interrupt(struct metal_switch sw) __attribute__((weak));
int metal_switch_enable_interrupt(struct metal_switch sw) { return -1; }

int metal_switch_disable_interrupt(struct metal_switch sw) __attribute__((weak));
int metal_switch_disable_interrupt(struct metal_switch sw) { return -1; }
