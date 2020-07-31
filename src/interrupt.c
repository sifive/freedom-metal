/* Copyright 2018 SiFive, Inc */
/* SPDX-License-Identifier: Apache-2.0 */

#include <metal/interrupt.h>
#include <string.h>

/* Stubs for when no interrupt controller exists */

void metal_interrupt_init(struct metal_interrupt controller) {}

int metal_interrupt_enable(struct metal_interrupt controller, int id) {
    return -1;
}

int metal_interrupt_disable(struct metal_interrupt controller, int id) {
    return -1;
}
