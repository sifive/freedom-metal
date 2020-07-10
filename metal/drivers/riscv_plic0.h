/* Copyright 2018 SiFive, Inc */
/* SPDX-License-Identifier: Apache-2.0 */

#ifndef METAL__DRIVERS__RISCV_PLIC0_H
#define METAL__DRIVERS__RISCV_PLIC0_H

#include <metal/interrupt.h>

#define METAL_PLIC_SOURCE_MASK 0x1F
#define METAL_PLIC_SOURCE_SHIFT 5
#define METAL_PLIC_SOURCE_PRIORITY_SHIFT 2
#define METAL_PLIC_SOURCE_PENDING_SHIFT 0

void riscv_plic0_init(struct metal_interrupt controller);

int
riscv_plic0_set_vector_mode(struct metal_interrupt controller,
                                              metal_vector_mode mode);

metal_vector_mode
riscv_plic0_get_vector_mode(struct metal_interrupt controller);

int riscv_plic0_set_privilege(struct metal_interrupt controller,
                                                metal_intr_priv_mode privilege);

metal_intr_priv_mode
riscv_plic0_get_privilege(struct metal_interrupt controller);

int riscv_plic0_clear(struct metal_interrupt controller,
                                        int id);

int riscv_plic0_set(struct metal_interrupt controller, int id);

int
riscv_plic0_register_handler(struct metal_interrupt controller, int id,
                                               metal_interrupt_handler_t handler,
                                               void *priv_data);

int riscv_plic0_register_vector_handler(
    struct metal_interrupt controller, int id,
    metal_interrupt_vector_handler_t handler, void *priv_data);

int riscv_plic0_enable(struct metal_interrupt controller, int id);

int riscv_plic0_disable(struct metal_interrupt controller, int id);

int riscv_plic0_set_threshold(struct metal_interrupt controller,
                                                unsigned int level);

unsigned int
riscv_plic0_get_threshold(struct metal_interrupt controller);

int riscv_plic0_set_priority(struct metal_interrupt controller,
                                               int id, unsigned int priority);

unsigned int
riscv_plic0_get_priority(struct metal_interrupt controller, int id);

int
riscv_plic0_set_preemptive_level(struct metal_interrupt controller, int id,
                                                   unsigned int level);

unsigned int
riscv_plic0_get_preemptive_level(struct metal_interrupt controller, int id);

int riscv_plic0_vector_enable(struct metal_interrupt controller, int id);

int
riscv_plic0_vector_disable(struct metal_interrupt controller, int id);

metal_affinity
riscv_plic0_affinity_enable(struct metal_interrupt controller,
                                              metal_affinity bitmask, int id);

metal_affinity
riscv_plic0_affinity_disable(struct metal_interrupt controller,
                                               metal_affinity bitmask, int id);

metal_affinity
riscv_plic0_affinity_set_threshold(struct metal_interrupt controller,
                                                     metal_affinity bitmask,
                                                     unsigned int level);

unsigned int
riscv_plic0_affinity_get_threshold(struct metal_interrupt controller,
                                                     int context_id);

#endif
