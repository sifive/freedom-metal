/* Copyright 2021 SiFive, Inc */
/* SPDX-License-Identifier: Apache-2.0 */

#include <metal/machine.h>
#include <metal/mallard.h>

extern __inline__ int metal_mallard_twinjet_l1_enable(struct metal_cpu *cpu);

extern __inline__ int metal_mallard_twinjet_l2_enable(struct metal_cpu *cpu);

extern __inline__ int metal_mallard_twinjet_l1_disable(struct metal_cpu *cpu);

extern __inline__ int metal_mallard_twinjet_l2_disable(struct metal_cpu *cpu);

extern __inline__ void metal_mallard_twinjet_l1_set(struct metal_cpu *cpu,
                                                    uint64_t value);

extern __inline__ void metal_mallard_twinjet_l2_set(struct metal_cpu *cpu,
                                                    uint64_t value);

extern __inline__ uint64_t metal_mallard_twinjet_l1_get(struct metal_cpu *cpu);

extern __inline__ uint64_t metal_mallard_twinjet_l2_get(struct metal_cpu *cpu);
