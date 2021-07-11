/* Copyright 2020 SiFive, Inc */
/* Copyright © 2020 Keith Packard */
/*
 * Licensed under the Apache License, Version 2.0 (the “License”); you
 * may not use this file except in compliance with the License. A copy
 * of this license can be found in the LICENSE.Apache2 file included
 * with this software. You may also obtain a copy of the License at
 *
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an “AS IS” BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or
 * implied. See the License for the specific language governing
 * permissions and limitations under the License.
 */
/* SPDX-License-Identifier: Apache-2.0 */

#ifndef METAL__DRIVERS__SIFIVE_CLIC0_H
#define METAL__DRIVERS__SIFIVE_CLIC0_H

#include <metal/interrupt.h>

#define SIFIVE_CLIC_SOFTWARE_INTERRUPT_ID 12

void sifive_clic0_init(struct metal_interrupt clic);

int sifive_clic0_set_vector_mode(struct metal_interrupt clic,
                                 metal_vector_mode mode);

metal_vector_mode sifive_clic0_get_vector_mode(struct metal_interrupt clic);

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

#endif
