/* Copyright 2018 SiFive, Inc */
/* SPDX-License-Identifier: Apache-2.0 */

#include <assert.h>
#include <metal/button.h>
#include <metal/generated/button.h>

struct metal_button metal_button_get(uint32_t index) {
    assert(index < __METAL_DT_NUM_BUTTONS);
    return (struct metal_button) { index };
}

bool metal_button_is_pressed(struct metal_button button) __attribute__((weak));
bool metal_button_is_pressed(struct metal_button button) { return false; }

int metal_button_enable_interrupt(struct metal_button button) __attribute__((weak));
int metal_button_enable_interrupt(struct metal_button button) { return -1; }

int metal_button_disable_interrupt(struct metal_button button) __attribute__((weak));
int metal_button_disable_interrupt(struct metal_button button) { return -1; }
