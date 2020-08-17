/* Copyright 2018 SiFive, Inc */
/* SPDX-License-Identifier: Apache-2.0 */

#include <assert.h>
#include <metal/button.h>
#include <metal/private/metal_private_button.h>

bool metal_button_is_pressed(struct metal_button button) __attribute__((weak));
bool metal_button_is_pressed(struct metal_button button) { return false; }

int metal_button_enable_interrupt(struct metal_button button)
    __attribute__((weak));
int metal_button_enable_interrupt(struct metal_button button) { return -1; }

int metal_button_disable_interrupt(struct metal_button button)
    __attribute__((weak));
int metal_button_disable_interrupt(struct metal_button button) { return -1; }

__attribute__((weak)) int
metal_button_clear_interrupt(struct metal_button button) {
    return -1;
}
