/* Copyright 2018 SiFive, Inc */
/* SPDX-License-Identifier: Apache-2.0 */

#include <assert.h>
#include <metal/switch.h>
#include <stdbool.h>

struct metal_switch metal_switch_get(char *label) __attribute__((weak));
struct metal_switch metal_switch_get(char *label) {
    assert(false);
    return (struct metal_switch) { 0 };
}
