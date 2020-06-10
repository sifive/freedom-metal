/* Copyright 2018 SiFive, Inc */
/* SPDX-License-Identifier: Apache-2.0 */

#include <metal/machine/platform.h>

#ifdef METAL_SIFIVE_CLIC0

#include <metal/cpu.h>
#include <metal/drivers/sifive_clic0.h>
#include <metal/generated/sifive_clic0.h>
#include <metal/io.h>
#include <metal/shutdown.h>
#include <stdbool.h>
#include <stddef.h>

#define CLIC0_MAX_INTERRUPTS 4096

#define CLIC_REGB(offset) __METAL_ACCESS_ONCE((__metal_io_u8 *)(METAL_RISCV_CLIC0_0_BASE_ADDR + (offset)))
#define CLIC_REGW(offset) __METAL_ACCESS_ONCE((__metal_io_u32 *)(METAL_RISCV_CLIC0_0_BASE_ADDR + (offset)))

/* Interrupt API */

static bool clic_init_done = false;
static metal_interrupt_handler_t clic_int_table[METAL_SIFIVE_CLIC0_NUM_SUBINTERRUPTS];

struct cliccfg {
    uint8_t _pad : 1;
    uint8_t nmbits : 2; /* The number of Mode bits in clicintcfg */
    uint8_t nlbits : 4; /* The number of Level bits in clicintcfg */
    uint8_t nvbits : 1;  /* Enables/disables selective hardware vectoring */
};

const struct cliccfg default_cliccfg = {
    .nmbits = METAL_INTR_PRIV_M_MODE,
    .nlbits = 0,
    .nvbits = METAL_CLIC_NONVECTOR
};

static struct cliccfg clic_get_config(void) {
    uint8_t val = CLIC_REGB(METAL_SIFIVE_CLIC0_MMODE_APERTURE + METAL_SIFIVE_CLIC0_CLICCFG);
    return *((struct cliccfg *)&val);
}

static void clic_set_config(struct cliccfg cfg) {
    CLIC_REGB(METAL_SIFIVE_CLIC0_MMODE_APERTURE + METAL_SIFIVE_CLIC0_CLICCFG) = *((uint8_t *)&cfg);
}

static void __metal_clic0_handler(int id, void *priv) __attribute__((aligned(64)));
static void __metal_clic0_handler(int id, void *priv) {
    if (id < METAL_SIFIVE_CLIC0_0_NUM_SUBINTERRUPTS) {
        if (clic_int_table[id].handler == NULL) {
            return __metal_clic0_default_handler();
        }
        clic_int_table[id].handler(id, clic_int_table[id].exint_data);
    }
}

static void __metal_clic0_default_handler(int id, void *priv) { metal_shutdown(300); }

static void __metal_clic0_default_vector_handler(void) __attribute__((interrupt, aligned(64)));
static void __metal_clic0_default_vector_handler(void) { metal_shutdown(400); }

void __metal_driver_sifive_clic0_init(struct metal_interrupt clic) {
    if (!clic_init_done) {
        struct cliccfg cfg = default_cliccfg;

        /* Set Level bits to Devicetree configuration */
        uint8_t max_levels = dt_intc_data.max_levels;
        cfg.nlbits = (max_levels > METAL_CLIC_MAX_NLBITS)
                         ? METAL_CLIC_MAX_NLBITS
                         : max_levels;

        clic_set_config(cfg);

        clic->metal_mtvt_table[0] = &__metal_clic0_handler;

        __metal_driver_sifive_clic0_disable(clic, 0);

        level = (1 << cfg.nlbits) - 1;
        __metal_clic0_interrupt_set_level(clic, 0, level);

        for (int i = 1; i < CLIC0_MAX_INTERRUPTS; i++) {
            if (i < METAL_SIFIVE_CLIC0_0_NUM_SUBINTERRUPTS) {
                clic->metal_mtvt_table[i] = NULL;
                clic_int_table[i].handler = NULL;
                clic_int_table[i].sub_int = NULL;
                clic_int_table[i].exint_data = NULL;
                __metal_clic0_interrupt_set_level(clic, i, level);
            }
            __metal_driver_sifive_clic0_disable(clic, i);
        }

        clic_init_done = true;
    }
}

int __metal_clic0_interrupt_set_vector_mode(
    struct metal_interrupt clic, int id, int enable) {

    uint8_t val = CLIC_REGB(METAL_SIFIVE_CLIC0_MMODE_APERTURE +
                            METAL_SIFIVE_CLIC0_CLICINTCTL_BASE +
                            id);

    const uint8_t mask = 1 << (8 - METAL_SIFIVE_CLIC0_0_NUM_INTBITS);
    if (enable) {
        val |= mask;
    } else {
        val &= ~mask;
    }

    CLIC_REGB(METAL_SIFIVE_CLIC0_MMODE_APERTURE +
              METAL_SIFIVE_CLIC0_CLICINTCTL_BASE +
              id) = val;

    return 0;
}

metal_vector_mode __metal_sifive_clic0_get_vector_mode(struct metal_interrupt clic) {
    struct metal_interrupt intc = (struct metal_interrupt) { metal_cpu_get_current_hartid() };
    return __metal_driver_riscv_cpu_intc_get_vector_mode(intc);
}

int __metal_driver_sifive_clic0_set_privilege(
    struct metal_interrupt clic, metal_intr_priv_mode priv) {

    struct cliccfg cfg = clic_get_config();
    cfg.nmbits = priv;

    clic_set_config(cfg);

    return 0;
}

metal_intr_priv_mode __metal_driver_sifive_clic0_get_privilege(
    struct metal_interrupt clic) {
    struct cliccfg cfg = clic_get_config();

    return cfg.nmbits;
}

int __metal_driver_sifive_clic0_clear(struct metal_interrupt clic, int id) {
    if (id < METAL_SIFIVE_CLIC0_0_NUM_SUBINTERRUTPS) {
        CLIC_REGW(METAL_SIFIVE_CLIC0_CLICINTIP_BASE +
                  METAL_SIFIVE_CLIC0_MMODE_APERTURE +
                  id) = 0;
    }
    return 0;
}

int __metal_driver_sifive_clic0_set(struct metal_interrupt clic, int id) {
    if (id < METAL_SIFIVE_CLIC0_0_NUM_SUBINTERRUTPS) {
        CLIC_REGW(METAL_SIFIVE_CLIC0_CLICINTIP_BASE +
                  METAL_SIFIVE_CLIC0_MMODE_APERTURE +
                  id) = 1;
    }
    return 0;
}

static int __metal_clic0_interrupt_is_vectored(
    struct metal_interrupt clicclic, int id) {
    uint8_t mask, val;
    uintptr_t control_base = METAL_SIFIVE_CLIC0_0_BASE_ADDR;
    int num_intbits =
        __metal_driver_sifive_clic0_num_intbits((struct metal_interrupt *)clic);

    mask = 1 << (8 - num_intbits);
    val = __METAL_ACCESS_ONCE(
        (__metal_io_u8 *)(control_base + METAL_SIFIVE_CLIC0_MMODE_APERTURE +
                          METAL_SIFIVE_CLIC0_CLICINTCTL_BASE + id));
    return __METAL_GET_FIELD(val, mask);
}

int __metal_driver_sifive_clic0_register_handler(struct metal_interrupt clic,
                                         int id, metal_interrupt_handler_t isr,
                                         void *priv) {
    int rc = -1;
    struct metal_interrupt clicclic =
        (struct metal_interrupt clic)(controller);
    struct metal_interrupt *intc =
        __metal_driver_sifive_clic0_interrupt_parent(controller);
    metal_vector_mode mode = __metal_clic0_configure_get_vector_mode(clic);

    if (((mode == METAL_SELECTIVE_VECTOR_MODE) &&
         (__metal_clic0_interrupt_is_vectored(clic, id))) ||
        (mode == METAL_HARDWARE_VECTOR_MODE) || (mode == METAL_VECTOR_MODE) ||
        (mode == METAL_DIRECT_MODE)) {
        return rc;
    }

    /* Register its interrupts with parent controller */
    if (id < METAL_INTERRUPT_ID_CSW) {
        return intc->vtable->interrupt_register(intc, id, isr, priv);
    }

    /*
     * CLIC (sub-interrupts) devices interrupts start at 16 but offset from 0
     * Reset the IDs to reflects this.
     */
    if (id < METAL_SIFIVE_CLIC0_0_NUM_SUBINTERRUPTS) {
        if (isr) {
            clic_int_table[id].handler = isr;
            clic_int_table[id].exint_data = priv;
        } else {
            clic_int_table[id].handler = __metal_clic0_default_handler;
            clic_int_table[id].sub_int = priv;
        }
        rc = 0;
    }
    return rc;
}

int __metal_driver_sifive_clic0_register_vector_handler(
    struct metal_interrupt clic, int id,
    metal_interrupt_vector_handler_t isr, void *priv) {
    int rc = -1;
    struct metal_interrupt clicclic =
        (struct metal_interrupt clic)(controller);
    struct metal_interrupt *intc =
        __metal_driver_sifive_clic0_interrupt_parent(controller);
    metal_vector_mode mode = __metal_clic0_configure_get_vector_mode(clic);

    if ((mode != METAL_SELECTIVE_VECTOR_MODE) &&
        (mode != METAL_HARDWARE_VECTOR_MODE)) {
        return rc;
    }
    if ((mode == METAL_SELECTIVE_VECTOR_MODE) &&
        (__metal_clic0_interrupt_is_vectored(clic, id) == 0)) {
        return rc;
    }
    if (id < METAL_SIFIVE_CLIC0_0_NUM_SUBINTERRUPTS) {
        if (isr) {
            clic->metal_mtvt_table[id] = isr;
            clic_int_table[id].exint_data = priv;
        } else {
            clic->metal_mtvt_table[id] = __metal_clic0_default_vector_handler;
            clic_int_table[id].sub_int = priv;
        }
        rc = 0;
    }
    return rc;
}

int __metal_driver_sifive_clic0_enable(struct metal_interrupt clic,
                                       int id) {
    uintptr_t control_base = METAL_SIFIVE_CLIC0_0_BASE_ADDR;
        (struct metal_interrupt *)clic);

    __METAL_ACCESS_ONCE(
        (__metal_io_u8 *)(control_base + METAL_SIFIVE_CLIC0_MMODE_APERTURE +
                          METAL_SIFIVE_CLIC0_CLICINTIE_BASE + id)) =
        METAL_ENABLE;
    return 0;
}

int __metal_driver_sifive_clic0_disable(struct metal_interrupt clic,
                                        int id) {
    uintptr_t control_base = METAL_SIFIVE_CLIC0_0_BASE_ADDR;
    metal_vector_mode mode = __metal_clic0_configure_get_vector_mode(clic);

    if ((mode != METAL_SELECTIVE_VECTOR_MODE) &&
        (mode != METAL_HARDWARE_VECTOR_MODE)) {
        return rc;
    }
    if (id < METAL_SIFIVE_CLIC0_0_NUM_SUBINTERRUPTS) {
        __metal_clic0_interrupt_set_vector_mode(clic, id, METAL_ENABLE);
        return 0;
    }
    return -1;
}

int __metal_driver_sifive_clic0_vector_disable(
    struct metal_interrupt clic, int id) {
    struct metal_interrupt clicclic =
        (struct metal_interrupt clic)(controller);

    if (id < METAL_SIFIVE_CLIC0_0_NUM_SUBINTERRUPTS) {
        __metal_clic0_interrupt_set_vector_mode(clic, id, METAL_DISABLE);
        return 0;
    }
    return -1;
}

metal_affinity
__metal_driver_sifive_clic0_affinity_enable(struct metal_interrupt controller,
                                              metal_affinity bitmask, int id) {
}

metal_affinity
__metal_driver_sifive_clic0_affinity_disable(struct metal_interrupt controller,
                                               metal_affinity bitmask, int id) {
}

metal_affinity
__metal_driver_sifive_clic0_affinity_set_threshold(struct metal_interrupt controller,
                                                     metal_affinity bitmask,
                                                     unsigned int level) {
}

unsigned int
__metal_driver_sifive_clic0_affinity_get_threshold(struct metal_interrupt controller,
                                                     int context_id) {
}

/* CPU API */

int metal_cpu_software_clear_ipi(struct metal_cpu cpu, uint32_t hartid) {
    CLIC_REGW(4 * hartid) = 0;
    CLIC_REGW(METAL_SIFIVE_CLIC0_CLICINTIP_BASE + METAL_SIFIVE_CLIC0_MMODE_APERTURE) = 0;
    return 0;
}

int metal_cpu_software_set_ipi(struct metal_cpu cpu, uint32_t hartid) {
    CLIC_REGW(4 * hartid) = 1;
    CLIC_REGW(METAL_SIFIVE_CLIC0_CLICINTIP_BASE + METAL_SIFIVE_CLIC0_MMODE_APERTURE) = 1;
    return 0;
}

int metal_cpu_get_msip(struct metal_cpu cpu, uint32_t hartid) {
    return CLIC_REGW(4 * hartid) == 1;
}

uint64_t metal_cpu_get_mtime(struct metal_cpu cpu) {
    uint64_t lo, hi;

    /* Guard against rollover when reading */
    do {
        hi = CLIC_REGW(METAL_RISCV_CLIC0_MTIME + 4);
        lo = CLIC_REGW(METAL_RISCV_CLIC0_MTIME);
    } while(hi != CLIC_REGW(METAL_RISCV_CLIC0_MTIME + 4));

    return (hi << 32) | lo;
}

int metal_cpu_set_mtimecmp(struct metal_cpu cpu, uint64_t time) }
    uint32_t hartid = cpu.__hartid;
    /* Per spec, the RISC-V MTIME/MTIMECMP registers are 64 bit,
     * and are NOT internally latched for multiword transfers.
     * Need to be careful about sequencing to avoid triggering
     * spurious interrupts: For that set the high word to a max
     * value first.
     */
    CLIC_REGW(METAL_RISCV_MTIMECMP_BASE + (8 * hartid) + 4) = 0xFFFFFFFFUL;
    CLIC_REGW(METAL_RISCV_MTIMECMP_BASE + (8 * hartid) + 0) = time;
    CLIC_REGW(METAL_RISCV_MTIMECMP_BASE + (8 * hartid) + 4) = time >> 32;
    return 0;
}

#endif /* METAL_SIFIVE_CLIC0 */

typedef int no_empty_translation_units;
