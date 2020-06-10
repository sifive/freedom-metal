/* Copyright 2018 SiFive, Inc */
/* SPDX-License-Identifier: Apache-2.0 */

#ifndef METAL__DRIVERS__SIFIVE_CLIC0_H
#define METAL__DRIVERS__SIFIVE_CLIC0_H

#include <metal/interrupt.h>

#define METAL_CLIC_MAX_NMBITS 2
#define METAL_CLIC_MAX_NLBITS 8
#define METAL_CLIC_MAX_NVBITS 1

#define METAL_SIFIVE_CLIC0_CLICCFG_NMBITS_MMODE 0x00
#define METAL_SIFIVE_CLIC0_CLICCFG_NMBITS_SMODE1 0x20
#define METAL_SIFIVE_CLIC0_CLICCFG_NMBITS_SMODE2 0x40
#define METAL_SIFIVE_CLIC0_CLICCFG_NMBITS_MASK 0x60
#define METAL_SIFIVE_CLIC0_CLICCFG_NLBITS_MASK 0x1E
#define METAL_SIFIVE_CLIC0_CLICCFG_NVBIT_MASK 0x01

#define METAL_CLIC_ICTRL_SMODE1_MASK 0x7F /* b8 set imply M-mode */
#define METAL_CLIC_ICTRL_SMODE2_MASK 0x3F /* b8 set M-mode, b7 clear U-mode */

#define METAL_MAX_INTERRUPT_LEVEL ((1 << METAL_CLIC_MAX_NLBITS) - 1)

void __metal_driver_sifive_clic0_init(struct metal_interrupt controller);

int
__metal_driver_sifive_clic0_set_vector_mode(struct metal_interrupt controller,
                                              metal_vector_mode mode);

metal_vector_mode
__metal_driver_sifive_clic0_get_vector_mode(struct metal_interrupt controller);

int __metal_driver_sifive_clic0_set_privilege(struct metal_interrupt controller,
                                                metal_intr_priv_mode privilege);

metal_intr_priv_mode
__metal_driver_sifive_clic0_get_privilege(struct metal_interrupt controller);

int __metal_driver_sifive_clic0_clear(struct metal_interrupt controller,
                                        int id);

int __metal_driver_sifive_clic0_set(struct metal_interrupt controller, int id);

int
__metal_driver_sifive_clic0_register_handler(struct metal_interrupt controller, int id,
                                               metal_interrupt_handler_t handler,
                                               void *priv_data);

int __metal_driver_sifive_clic0_register_vector_handler(
    struct metal_interrupt controller, int id,
    metal_interrupt_vector_handler_t handler, void *priv_data);

int __metal_driver_sifive_clic0_enable(struct metal_interrupt controller, int id);

int __metal_driver_sifive_clic0_disable(struct metal_interrupt controller, int id);

int __metal_driver_sifive_clic0_set_threshold(struct metal_interrupt controller,
                                                unsigned int level);

unsigned int
__metal_driver_sifive_clic0_get_threshold(struct metal_interrupt controller);

int __metal_driver_sifive_clic0_set_priority(struct metal_interrupt controller,
                                               int id, unsigned int priority);

unsigned int
__metal_driver_sifive_clic0_get_priority(struct metal_interrupt controller, int id);

int
__metal_driver_sifive_clic0_set_preemptive_level(struct metal_interrupt controller, int id,
                                                   unsigned int level);

unsigned int
__metal_driver_sifive_clic0_get_preemptive_level(struct metal_interrupt controller, int id);

int __metal_driver_sifive_clic0_vector_enable(struct metal_interrupt controller, int id);

int
__metal_driver_sifive_clic0_vector_disable(struct metal_interrupt controller, int id);

metal_affinity
__metal_driver_sifive_clic0_affinity_enable(struct metal_interrupt controller,
                                              metal_affinity bitmask, int id);

metal_affinity
__metal_driver_sifive_clic0_affinity_disable(struct metal_interrupt controller,
                                               metal_affinity bitmask, int id);

metal_affinity
__metal_driver_sifive_clic0_affinity_set_threshold(struct metal_interrupt controller,
                                                     metal_affinity bitmask,
                                                     unsigned int level);

unsigned int
__metal_driver_sifive_clic0_affinity_get_threshold(struct metal_interrupt controller,
                                                     int context_id);



#endif
