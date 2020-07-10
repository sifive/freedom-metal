/* Copyright 2018 SiFive, Inc */
/* SPDX-License-Identifier: Apache-2.0 */

#include <metal/interrupt.h>
#include <string.h>

/* Stubs for when no interrupt controller exists */

void metal_interrupt_init(struct metal_interrupt controller) {}

int
metal_interrupt_set_vector_mode(struct metal_interrupt controller,
                                metal_vector_mode mode) {
    return -1;
}

metal_vector_mode
metal_interrupt_get_vector_mode(struct metal_interrupt controller) {
    return METAL_DIRECT_MODE;
}

int
metal_interrupt_set_privilege(struct metal_interrupt controller,
                              metal_intr_priv_mode mode) {
    return -1;
}
metal_intr_priv_mode
metal_interrupt_get_privilege(struct metal_interrupt controller) {
    return METAL_INTR_PRIV_M_MODE;
}

int
metal_interrupt_set_threshold(struct metal_interrupt controller,
                              unsigned int level) {
    return -1;
}
unsigned int
metal_interrupt_get_threshold(struct metal_interrupt controller) {
    return 0;
}

unsigned int
metal_interrupt_get_priority(struct metal_interrupt controller, int id) {
    return 0;
}

int
metal_interrupt_set_priority(struct metal_interrupt controller, int id,
                             unsigned int priority) {
    return -1;
}

int
metal_interrupt_set_preemptive_level(struct metal_interrupt controller, int id,
                                     unsigned int level) {
    return -1;
}

unsigned int
metal_interrupt_get_preemptive_level(struct metal_interrupt controller,
                                     int id) {
    return 0;
}

int metal_interrupt_enable(struct metal_interrupt controller,
                                             int id) {
    return -1;
}

int
metal_interrupt_disable(struct metal_interrupt controller, int id) {
    return -1;
}

int
metal_interrupt_vector_enable(struct metal_interrupt controller, int id) {
    return -1;
}

int
metal_interrupt_vector_disable(struct metal_interrupt controller, int id) {
    return -1;
}

int
_metal_interrupt_command_request(struct metal_interrupt controller, int cmd,
                                 void *data) {
    return -1;
}

metal_affinity
metal_interrupt_affinity_enable(struct metal_interrupt controller,
                                metal_affinity bitmask, int id) {
    return (metal_affinity) { 0 };
}

metal_affinity
metal_interrupt_affinity_disable(struct metal_interrupt controller,
                                 metal_affinity bitmask, int id) {
    return (metal_affinity) { 0 };
}

metal_affinity
metal_interrupt_affinity_set_threshold(struct metal_interrupt controller,
                                       metal_affinity bitmask,
                                       unsigned int level) {
    return (metal_affinity) { 0 };
}

unsigned int
metal_interrupt_affinity_get_threshold(struct metal_interrupt controller,
                                       int contextid) {
    return 0;
}
