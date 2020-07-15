/* Copyright 2018 SiFive, Inc */
/* SPDX-License-Identifier: Apache-2.0 */

#include <assert.h>
#include <metal/button.h>
#include <stdbool.h>

struct metal_button metal_button_get(char *label) __attribute__((weak));
struct metal_button metal_button_get(char *label) {
    assert(false);
    return (struct metal_button) { 0 };
}
