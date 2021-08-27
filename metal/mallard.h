/* Copyright 2021 SiFive, Inc */
/* SPDX-License-Identifier: Apache-2.0 */

#ifndef METAL__MALLARD_H
#define METAL__MALLARD_H

#include <metal/cpu.h>
#include <stdint.h>

struct mallard_twinjet_ops {
    int (*l1_enable)(void);
    int (*l2_enable)(void);
    int (*l1_disable)(void);
    int (*l2_disable)(void);
    void (*l1_set)(uint64_t value);
    void (*l2_set)(uint64_t value);
    uint64_t (*l1_get)(void);
    uint64_t (*l2_get)(void);
};

struct mallard_twinjet {
    struct mallard_twinjet_ops *ops;
};

struct metal_sifive_mallard0 {
    struct mallard_twinjet *twinjet;
};

__inline__ int metal_mallard_twinjet_l1_enable(struct metal_cpu *cpu) {
    struct metal_sifive_mallard0 *specific =
        (struct metal_sifive_mallard0 *)cpu->vtable->get_cpu_specific(cpu);

    if (specific)
        return specific->twinjet->ops->l1_enable();

    return -1;
}

__inline__ int metal_mallard_twinjet_l2_enable(struct metal_cpu *cpu) {
    struct metal_sifive_mallard0 *specific =
        (struct metal_sifive_mallard0 *)cpu->vtable->get_cpu_specific(cpu);

    if (specific)
        return specific->twinjet->ops->l2_enable();

    return -1;
}

__inline__ int metal_mallard_twinjet_l1_disable(struct metal_cpu *cpu) {
    struct metal_sifive_mallard0 *specific =
        (struct metal_sifive_mallard0 *)cpu->vtable->get_cpu_specific(cpu);

    if (specific)
        return specific->twinjet->ops->l1_disable();

    return -1;
}

__inline__ int metal_mallard_twinjet_l2_disable(struct metal_cpu *cpu) {
    struct metal_sifive_mallard0 *specific =
        (struct metal_sifive_mallard0 *)cpu->vtable->get_cpu_specific(cpu);

    if (specific)
        return specific->twinjet->ops->l2_disable();

    return -1;
}

__inline__ void metal_mallard_twinjet_l1_set(struct metal_cpu *cpu,
                                             uint64_t value) {
    struct metal_sifive_mallard0 *specific =
        (struct metal_sifive_mallard0 *)cpu->vtable->get_cpu_specific(cpu);

    if (specific)
        specific->twinjet->ops->l1_set(value);
}

__inline__ void metal_mallard_twinjet_l2_set(struct metal_cpu *cpu,
                                             uint64_t value) {
    struct metal_sifive_mallard0 *specific =
        (struct metal_sifive_mallard0 *)cpu->vtable->get_cpu_specific(cpu);

    if (specific)
        specific->twinjet->ops->l2_set(value);
}

__inline__ uint64_t metal_mallard_twinjet_l1_get(struct metal_cpu *cpu) {
    struct metal_sifive_mallard0 *specific =
        (struct metal_sifive_mallard0 *)cpu->vtable->get_cpu_specific(cpu);

    if (specific)
        return specific->twinjet->ops->l1_get();

    return -1;
}

__inline__ uint64_t metal_mallard_twinjet_l2_get(struct metal_cpu *cpu) {
    struct metal_sifive_mallard0 *specific =
        (struct metal_sifive_mallard0 *)cpu->vtable->get_cpu_specific(cpu);

    if (specific)
        return specific->twinjet->ops->l2_get();

    return -1;
}

#endif /* METAL__MALLARD_H */
