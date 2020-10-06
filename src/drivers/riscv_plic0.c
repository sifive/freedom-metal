/* Copyright 2018 SiFive, Inc */
/* SPDX-License-Identifier: Apache-2.0 */

#include <metal/platform.h>

#ifdef METAL_RISCV_PLIC0

#include <metal/cpu.h>
#include <metal/drivers/riscv_cpu_intc.h>
#include <metal/drivers/riscv_plic0.h>
#include <metal/init.h>
#include <metal/io.h>
#include <metal/private/metal_private_riscv_plic0.h>
#include <stdbool.h>
#include <stdint.h>

#define PLIC_REGW(offset)                                                      \
    (__METAL_ACCESS_ONCE(                                                      \
        (__metal_io_u32 *)(METAL_RISCV_PLIC0_0_BASE_ADDRESS + (offset))))

#define for_each_metal_affinity(bit, metal_affinity)                           \
    for (bit = 0; metal_affinity.bitmask; bit++, metal_affinity.bitmask >>= 1)

#define metal_affinity_set_val(metal_affinity, val)                            \
    metal_affinity.bitmask = val;

#define metal_affinity_set_bit(metal_affinity, bit, val)                       \
    metal_affinity.bitmask |= ((val & 0x1) << bit);

extern metal_interrupt_handler_t __metal_global_interrupt_table[];

static __inline__ unsigned int __metal_plic0_claim_interrupt(uint32_t hartid) {
    return PLIC_REGW(
        METAL_RISCV_PLIC0_CONTEXT_BASE +
        (PLIC_CONTEXT_ID(hartid) * METAL_RISCV_PLIC0_PER_CONTEXT_OFFSET) +
        METAL_RISCV_PLIC0_CONTEXT_CLAIM);
}

static __inline__ void __metal_plic0_complete_interrupt(int hartid,
                                                        unsigned int id) {
    PLIC_REGW(METAL_RISCV_PLIC0_CONTEXT_BASE +
              (PLIC_CONTEXT_ID(hartid) * METAL_RISCV_PLIC0_PER_CONTEXT_OFFSET) +
              METAL_RISCV_PLIC0_CONTEXT_CLAIM) = id;
}

static __inline__ int
__metal_riscv_plic0_set_threshold(int hartid, unsigned int threshold) {
    PLIC_REGW(METAL_RISCV_PLIC0_CONTEXT_BASE +
              (PLIC_CONTEXT_ID(hartid) * METAL_RISCV_PLIC0_PER_CONTEXT_OFFSET) +
              METAL_RISCV_PLIC0_CONTEXT_CLAIM) = threshold;
    return 0;
}

static __inline__ unsigned int __metal_riscv_plic0_get_threshold(int hartid) {
    return PLIC_REGW(
        METAL_RISCV_PLIC0_CONTEXT_BASE +
        (PLIC_CONTEXT_ID(hartid) * METAL_RISCV_PLIC0_PER_CONTEXT_OFFSET) +
        METAL_RISCV_PLIC0_CONTEXT_CLAIM);
}

static __inline__ int __metal_plic0_enable(int hartid, int id) {
    if (id >= METAL_RISCV_PLIC0_0_RISCV_NDEV) {
        return -1;
    }

    const uintptr_t id_offset = (id / 32) * 4;
    const uint32_t enable_mask = (1 << id % 32);

    PLIC_REGW(METAL_RISCV_PLIC0_ENABLE_BASE +
              (PLIC_CONTEXT_ID(hartid) * METAL_RISCV_PLIC0_ENABLE_PER_CONTEXT) +
              id_offset) |= enable_mask;

    metal_cpu_enable_external_interrupt();

    return 0;
}

static __inline__ int __metal_plic0_disable(int hartid, int id) {
    if (id >= METAL_RISCV_PLIC0_0_RISCV_NDEV) {
        return -1;
    }

    const uintptr_t id_offset = (id / 32) * 4;
    const uint32_t disable_mask = (1 << id % 32);

    PLIC_REGW(METAL_RISCV_PLIC0_ENABLE_BASE +
              (PLIC_CONTEXT_ID(hartid) * METAL_RISCV_PLIC0_ENABLE_PER_CONTEXT) +
              id_offset) &= ~disable_mask;

    /* Check if any PLIC interrupts remain enabled and simply exit
     * if they are. */
    for (int i = 0; i < (METAL_RISCV_PLIC0_0_RISCV_NDEV / 32); i++) {
        if (PLIC_REGW(METAL_RISCV_PLIC0_ENABLE_BASE +
                      (PLIC_CONTEXT_ID(hartid) *
                       METAL_RISCV_PLIC0_ENABLE_PER_CONTEXT) +
                      (i * 4)) != 0) {
            return 0;
        }
    }

    /* No more PLIC interrupts are enabled, so disable the external interrupt.
     */
    metal_cpu_disable_external_interrupt();

    return 0;
}

/* This is the interrupt handler for the machine external interrupt, which is
 * automatically named metal_riscv_plic0_source_0_handler() when a PLIC exists.
 */
void metal_riscv_plic0_source_0_handler(void) {
    uint32_t hartid = metal_cpu_get_current_hartid();

    unsigned int idx = __metal_plic0_claim_interrupt(hartid);

    if (idx < METAL_RISCV_PLIC0_0_RISCV_NDEV) {
        __metal_global_interrupt_table[idx]();
    }

    __metal_plic0_complete_interrupt(hartid, idx);
}

void riscv_plic0_init(struct metal_interrupt plic) {
    static bool init_done = false;
    if (!init_done) {
        for (int hartid = 0; hartid < __METAL_DT_NUM_HARTS; hartid++) {
            struct metal_interrupt intc = (struct metal_interrupt){hartid};

            /* Initialize riscv,cpu-intc */
            metal_interrupt_init(intc);

            /* Disable all interrupts, but set their priority to 1 so that
             * the interrupt fires when enabled and pending. */
            for (int id = 0; id < METAL_RISCV_PLIC0_0_RISCV_NDEV; id++) {
                __metal_plic0_disable(hartid, id);
                riscv_plic0_set_priority(intc, id, 1);
            }

            /* Set the default threshold to 0 so that any enabled interrupts
             * are above the threshold. */
            __metal_riscv_plic0_set_threshold(hartid, 0);

            /* Enable plic (ext) interrupt with with hartid controller */
            metal_interrupt_enable(intc, METAL_INTERRUPT_ID_EXT);
        }
        init_done = true;
    }
}

METAL_CONSTRUCTOR(init_riscv_plic0) {
    struct metal_interrupt plic = (struct metal_interrupt){0};
    riscv_plic0_init(plic);
}

int riscv_plic0_set_vector_mode(struct metal_interrupt controller,
                                metal_vector_mode mode) {
    if (mode == METAL_DIRECT_MODE) {
        return 0;
    }
    return -1;
}

metal_vector_mode
riscv_plic0_get_vector_mode(struct metal_interrupt controller) {
    return METAL_DIRECT_MODE;
}

int riscv_plic0_enable(struct metal_interrupt plic, int id) {
    return __metal_plic0_enable(metal_cpu_get_current_hartid(), id);
}

int riscv_plic0_disable(struct metal_interrupt plic, int id) {
    __metal_plic0_disable(metal_cpu_get_current_hartid(), id);
    return 0;
}

int riscv_plic0_set_priority(struct metal_interrupt plic, int id,
                             unsigned int priority) {
    if (id >= METAL_RISCV_PLIC0_0_RISCV_NDEV) {
        return -1;
    }
    if (priority >= METAL_RISCV_PLIC0_0_RISCV_MAX_PRIORITY) {
        return -1;
    }
    PLIC_REGW(METAL_RISCV_PLIC0_PRIORITY_BASE +
              (id << METAL_PLIC_SOURCE_PRIORITY_SHIFT)) = priority;
    return 0;
}

unsigned int riscv_plic0_get_priority(struct metal_interrupt plic, int id) {
    return PLIC_REGW(METAL_RISCV_PLIC0_PRIORITY_BASE +
                     (id << METAL_PLIC_SOURCE_PRIORITY_SHIFT));
}

int riscv_plic0_set_threshold(struct metal_interrupt plic,
                              unsigned int priority) {
    for (int hartid = 0; hartid < __METAL_DT_NUM_HARTS; hartid++) {
        __metal_riscv_plic0_set_threshold(hartid, priority);
    }
    return 0;
}

int riscv_plic0_vector_enable(struct metal_interrupt controller, int id) {
    return -1;
}

int riscv_plic0_vector_disable(struct metal_interrupt controller, int id) {
    return 0;
}

metal_affinity riscv_plic0_affinity_enable(struct metal_interrupt plic,
                                           metal_affinity bitmask, int id) {
    metal_affinity ret = {0};

    if (id >= METAL_RISCV_PLIC0_0_RISCV_NDEV) {
        metal_affinity_set_val(ret, -1);
        return ret;
    }

    int hartid;
    for_each_metal_affinity(hartid, bitmask) {
        if (hartid != 0)
            metal_affinity_set_bit(ret, hartid,
                                   __metal_plic0_enable(hartid, id));
    }

    return ret;
}

metal_affinity riscv_plic0_affinity_disable(struct metal_interrupt plic,
                                            metal_affinity bitmask, int id) {
    metal_affinity ret = {0};

    if (id >= METAL_RISCV_PLIC0_0_RISCV_NDEV) {
        metal_affinity_set_val(ret, -1);
        return ret;
    }

    int hartid;
    for_each_metal_affinity(hartid, bitmask) {
        if (hartid != 0)
            metal_affinity_set_bit(ret, hartid,
                                   __metal_plic0_disable(hartid, id));
    }

    return ret;
}

metal_affinity riscv_plic0_affinity_set_threshold(struct metal_interrupt plic,
                                                  metal_affinity bitmask,
                                                  unsigned int threshold) {
    metal_affinity ret = {0};
    int hartid;

    for_each_metal_affinity(hartid, bitmask) {
        if (hartid != 0)
            metal_affinity_set_bit(
                ret, hartid,
                __metal_riscv_plic0_set_threshold(hartid, threshold));
    }

    return ret;
}

unsigned int riscv_plic0_affinity_get_threshold(struct metal_interrupt plic,
                                                int hartid) {
    return __metal_riscv_plic0_get_threshold(hartid);
}

#endif /* METAL_RISCV_PLIC0 */

typedef int no_empty_translation_units;
