/* Copyright 2020 SiFive, Inc */
/* SPDX-License-Identifier: Apache-2.0 */

#ifndef METAL__DRIVERS__SIFIVE_CLIC0_H
#define METAL__DRIVERS__SIFIVE_CLIC0_H

#include <metal/interrupt.h>

#define SIFIVE_CLIC_SOFTWARE_INTERRUPT_ID 12

void sifive_clic0_init(struct metal_interrupt clic);

int sifive_clic0_set_vector_mode(struct metal_interrupt clic,
                                 metal_vector_mode mode);

metal_vector_mode sifive_clic0_get_vector_mode(struct metal_interrupt clic);

int sifive_clic0_set_privilege(struct metal_interrupt clic,
                               metal_intr_priv_mode privilege);

metal_intr_priv_mode sifive_clic0_get_privilege(struct metal_interrupt clic);

int sifive_clic0_clear(struct metal_interrupt clic, int id);

int sifive_clic0_set(struct metal_interrupt clic, int id);

int sifive_clic0_enable(struct metal_interrupt clic, int id);

int sifive_clic0_disable(struct metal_interrupt clic, int id);

int sifive_clic0_set_threshold(struct metal_interrupt clic, unsigned int level);

unsigned int sifive_clic0_get_threshold(struct metal_interrupt clic);

int sifive_clic0_set_priority(struct metal_interrupt clic, int id,
                              unsigned int priority);

unsigned int sifive_clic0_get_priority(struct metal_interrupt clic, int id);

int sifive_clic0_set_preemptive_level(struct metal_interrupt clic, int id,
                                      unsigned int level);

unsigned int sifive_clic0_get_preemptive_level(struct metal_interrupt clic,
                                               int id);

int sifive_clic0_vector_enable(struct metal_interrupt clic, int id);

int sifive_clic0_vector_disable(struct metal_interrupt clic, int id);

metal_affinity sifive_clic0_affinity_enable(struct metal_interrupt clic,
                                            metal_affinity bitmask, int id);

metal_affinity sifive_clic0_affinity_disable(struct metal_interrupt clic,
                                             metal_affinity bitmask, int id);

metal_affinity sifive_clic0_affinity_set_threshold(struct metal_interrupt clic,
                                                   metal_affinity bitmask,
                                                   unsigned int level);

unsigned int sifive_clic0_affinity_get_threshold(struct metal_interrupt clic,
                                                 int context_id);

#endif
