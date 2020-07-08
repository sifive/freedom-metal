/* Copyright 2018 SiFive, Inc */
/* SPDX-License-Identifier: Apache-2.0 */

#include <metal/machine/platform.h>

#ifdef METAL_RISCV_PLIC0

#include <metal/cpu.h>
#include <metal/drivers/riscv_plic0.h>
#include <metal/generated/riscv_plic0.h>
#include <metal/interrupt.h>
#include <metal/init.h>
#include <metal/io.h>
#include <metal/shutdown.h>
#include <stdbool.h>

#define get_index(plic) ((plic).__interrupt_index)

extern metal_interrupt_handler_t *__metal_global_interrupt_table;

static struct plic_state {
    bool init_done;
    void *int_data[__METAL_DT_RISCV_PLIC0_MAX_INTERRUPTS];
} plic_state[__METAL_DT_NUM_RISCV_PLIC0S];

static inline unsigned int
__metal_plic0_claim_interrupt(struct metal_interrupt plic,
                              int context_id) {
    unsigned long base = dt_intc_data[get_index(plic)].base;
    return __METAL_ACCESS_ONCE(
        (__metal_io_u32 *)(base + METAL_RISCV_PLIC0_CONTEXT_BASE +
                           (context_id * METAL_RISCV_PLIC0_CONTEXT_PER_HART) +
                           METAL_RISCV_PLIC0_CONTEXT_CLAIM));
}

static inline void __metal_plic0_complete_interrupt(struct metal_interrupt plic,
                                      int context_id, unsigned int id) {
    unsigned long base = dt_intc_data[get_index(plic)].base;
    __METAL_ACCESS_ONCE(
        (__metal_io_u32 *)(base + METAL_RISCV_PLIC0_CONTEXT_BASE +
                           (context_id * METAL_RISCV_PLIC0_CONTEXT_PER_HART) +
                           METAL_RISCV_PLIC0_CONTEXT_CLAIM)) = id;
}

static inline int __metal_riscv_plic0_set_threshold(struct metal_interrupt plic,
                                int context_id, unsigned int threshold) {
    unsigned long base = dt_intc_data[get_index(plic)].base;
    __METAL_ACCESS_ONCE(
        (__metal_io_u32 *)(base + METAL_RISCV_PLIC0_CONTEXT_BASE +
                           (context_id * METAL_RISCV_PLIC0_CONTEXT_PER_HART) +
                           METAL_RISCV_PLIC0_CONTEXT_THRESHOLD)) = threshold;
    return 0;
}

static inline unsigned int __metal_riscv_plic0_get_threshold(struct metal_interrupt plic,
                                         int context_id) {
    unsigned long base = dt_intc_data[get_index(plic)].base;
    return __METAL_ACCESS_ONCE(
        (__metal_io_u32 *)(base + METAL_RISCV_PLIC0_CONTEXT_BASE +
                           (context_id * METAL_RISCV_PLIC0_CONTEXT_PER_HART) +
                           METAL_RISCV_PLIC0_CONTEXT_THRESHOLD));
}

static inline int __metal_plic0_enable(struct metal_interrupt plic,
                         int context_id, int id, int enable) {
    unsigned int current;
    unsigned long base = dt_intc_data[get_index(plic)].base;

    current = __METAL_ACCESS_ONCE(
        (__metal_io_u32 *)(base + METAL_RISCV_PLIC0_ENABLE_BASE +
                           (context_id * METAL_RISCV_PLIC0_ENABLE_PER_HART) +
                           (id >> METAL_PLIC_SOURCE_SHIFT) * 4));
    __METAL_ACCESS_ONCE(
        (__metal_io_u32 *)(base + METAL_RISCV_PLIC0_ENABLE_BASE +
                           (context_id * METAL_RISCV_PLIC0_ENABLE_PER_HART) +
                           ((id >> METAL_PLIC_SOURCE_SHIFT) * 4))) =
        enable ? (current | (1 << (id & METAL_PLIC_SOURCE_MASK)))
               : (current & ~(1 << (id & METAL_PLIC_SOURCE_MASK)));

    return 0;
}

static inline void __metal_plic0_default_handler(int id, void *priv) { metal_shutdown(300); }

void metal_riscv_plic0_source_0_handler() {
    struct metal_interrupt plic = { 0 };
    int context_id = dt_intc_data[get_index(plic)].context_id;
    unsigned int idx = __metal_plic0_claim_interrupt(plic, context_id);
    unsigned int num_interrupts = dt_intc_data[get_index(plic)].num_interrupts;

    if (idx < num_interrupts) {
        __metal_global_interrupt_table[idx](idx, plic_state[get_index(plic)].int_data[idx]);
    }

    __metal_plic0_complete_interrupt(plic, context_id, idx);
}

void __metal_driver_riscv_plic0_init(struct metal_interrupt plic) {

    if (!plic_state[get_index(plic)].init_done) {
        for (int parent = 0; parent < __METAL_PLIC_NUM_PARENTS; parent++) {
            uint32_t num_interrupts = dt_intc_data[get_index(plic)].num_interrupts;
            struct metal_interrupt intc = dt_intc_data[get_index(plic)].interrupt_parent;
            int line = dt_intc_data[get_index(plic)].interrupt_id;

            /* Initialize ist parent controller, aka cpu_intc. */
            metal_interrupt_init(intc);

            for (int i = 0; i < __METAL_DT_RISCV_PLIC0_MAX_INTERRUPTS; i++) {
                __metal_plic0_enable(plic, parent, i, 0);
                if (i < num_interrupts) {
                    __metal_driver_riscv_plic0_set_priority(intc, i, 0);
                    plic_state[get_index(plic)].int_data[i] = NULL;
                }
            }

            __metal_riscv_plic0_set_threshold(intc, parent, 0);

            /* Enable plic (ext) interrupt with with parent controller */
            metal_interrupt_enable(intc, line);
        }
        plic_state[get_index(plic)].init_done = 1;
    }
}

METAL_CONSTRUCTOR(riscv_plic0_init) {
    for(int i = 0; i < __METAL_DT_NUM_RISCV_PLIC0S; i++) {
        struct metal_interrupt plic = (struct metal_interrupt) { i };
        __metal_driver_riscv_plic0_init(plic);    
    }
}

int
__metal_driver_riscv_plic0_set_vector_mode(struct metal_interrupt controller,
                                              metal_vector_mode mode) {
    return -1;
}

metal_vector_mode
__metal_driver_riscv_plic0_get_vector_mode(struct metal_interrupt controller) {
    return METAL_DIRECT_MODE;
}

int __metal_driver_riscv_plic0_set_privilege(struct metal_interrupt controller,
                                                metal_intr_priv_mode privilege) {
    return -1;
}

metal_intr_priv_mode
__metal_driver_riscv_plic0_get_privilege(struct metal_interrupt controller) {
    return METAL_INTR_PRIV_M_MODE;
}

int __metal_driver_riscv_plic0_clear(struct metal_interrupt controller,
                                        int id) {
    return -1;
}

int __metal_driver_riscv_plic0_set(struct metal_interrupt controller, int id) {
    return -1;
}

int __metal_driver_riscv_plic0_enable(struct metal_interrupt plic,
                                      int id) {

    if (id >= dt_intc_data[get_index(plic)].num_interrupts) {
        return -1;
    }

    __metal_plic0_enable(plic, metal_cpu_get_current_hartid(), id, 1);
    return 0;
}

int __metal_driver_riscv_plic0_disable(struct metal_interrupt plic,
                                       int id) {

    if (id >= dt_intc_data[get_index(plic)].num_interrupts) {
        return -1;
    }
    __metal_plic0_enable(plic, metal_cpu_get_current_hartid(), id, 0);
    return 0;
}

int __metal_driver_riscv_plic0_set_priority(struct metal_interrupt plic,
                                            int id, unsigned int priority) {
    unsigned long base = dt_intc_data[get_index(plic)].base;
    unsigned int max_priority = dt_intc_data[get_index(plic)].max_priority;
    if (priority < max_priority) {
        __METAL_ACCESS_ONCE(
            (__metal_io_u32 *)(base + METAL_RISCV_PLIC0_PRIORITY_BASE +
                               (id << METAL_PLIC_SOURCE_PRIORITY_SHIFT))) =
            priority;
        return 0;
    }
    return -1;
}

unsigned int
__metal_driver_riscv_plic0_get_priority(struct metal_interrupt plic,
                                        int id) {
    unsigned long base = dt_intc_data[get_index(plic)].base;

    return __METAL_ACCESS_ONCE(
        (__metal_io_u32 *)(base + METAL_RISCV_PLIC0_PRIORITY_BASE +
                           (id << METAL_PLIC_SOURCE_PRIORITY_SHIFT)));
}

int
__metal_driver_riscv_plic0_set_preemptive_level(struct metal_interrupt controller, int id,
                                                   unsigned int level) {
    return -1;
}

unsigned int
__metal_driver_riscv_plic0_get_preemptive_level(struct metal_interrupt controller, int id) {
    return 0;
}

int __metal_driver_riscv_plic0_vector_enable(struct metal_interrupt controller, int id) {
    return -1;
}

int
__metal_driver_riscv_plic0_vector_disable(struct metal_interrupt controller, int id) {
    return -1;
}

metal_affinity
__metal_driver_riscv_plic0_affinity_enable(struct metal_interrupt plic,
                                           metal_affinity bitmask, int id) {
    metal_affinity ret = {0};
    int context;


    if (id >= dt_intc_data[get_index(plic)].num_interrupts) {
        metal_affinity_set_val(ret, -1);
        return ret;
    }

    for_each_metal_affinity(context, bitmask) {
        if (context != 0)
            metal_affinity_set_bit(
                ret, context,
                __metal_plic0_enable(plic, context, id, 1));
    }

    return ret;
}

metal_affinity
__metal_driver_riscv_plic0_affinity_disable(struct metal_interrupt plic,
                                            metal_affinity bitmask, int id) {
    metal_affinity ret = {0};
    int context;


    if (id >= dt_intc_data[get_index(plic)].num_interrupts) {
        metal_affinity_set_val(ret, -1);
        return ret;
    }

    for_each_metal_affinity(context, bitmask) {
        if (context != 0)
            metal_affinity_set_bit(
                ret, context,
                __metal_plic0_enable(plic, context, id, 0));
    }

    return ret;
}

metal_affinity __metal_driver_riscv_plic0_affinity_set_threshold(
    struct metal_interrupt plic, metal_affinity bitmask,
    unsigned int threshold) {
    metal_affinity ret = {0};
    int context;

    for_each_metal_affinity(context, bitmask) {
        if (context != 0)
            metal_affinity_set_bit(
                ret, context,
                __metal_riscv_plic0_set_threshold(plic, context, threshold));
    }

    return ret;
}

unsigned int __metal_driver_riscv_plic0_affinity_get_threshold(
    struct metal_interrupt plic, int context_id) {
    return __metal_riscv_plic0_get_threshold(plic, context_id);
}

#endif /* METAL_RISCV_PLIC0 */

typedef int no_empty_translation_units;
