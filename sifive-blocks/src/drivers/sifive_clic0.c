/* Copyright 2018 SiFive, Inc */
/* SPDX-License-Identifier: Apache-2.0 */

#include <metal/machine/platform.h>

#ifdef METAL_SIFIVE_CLIC0

#include <metal/cpu.h>
#include <metal/drivers/riscv_cpu_intc.h>
#include <metal/drivers/sifive_clic0.h>
#include <metal/generated/sifive_clic0.h>
#include <metal/init.h>
#include <metal/io.h>
#include <metal/riscv.h>
#include <stdbool.h>

#define CLICCFG_NMBITS_MASK 0x60
#define CLICCFG_NLBITS_MASK 0x1E
#define CLICCFG_NVBITS_MASK 0x01

#define CLIC_REGB(offset)                                                      \
    __METAL_ACCESS_ONCE(                                                       \
        (__metal_io_u8 *)(METAL_SIFIVE_CLIC0_0_BASE_ADDRESS + (offset)))
#define CLIC_REGW(offset)                                                      \
    __METAL_ACCESS_ONCE(                                                       \
        (__metal_io_u32 *)(METAL_SIFIVE_CLIC0_0_BASE_ADDRESS + (offset)))

struct clic_config {
    uint8_t _pad : 1;
    uint8_t nmBits : 2;
    uint8_t nlBits : 4;
    bool nvBits : 1;
};

const struct clic_config default_clic_config = {
    .nmBits = 0, /* Clear nmBits */
    .nlBits = CLIC_NLBITS,
    .nvBits = true,
};

static bool init_done = false;

static void set_clic_config(const struct clic_config cfg) {
    uint8_t val = 0;

    val = __METAL_SET_FIELD(val, CLICCFG_NMBITS_MASK, cfg.nmBits);
    val = __METAL_SET_FIELD(val, CLICCFG_NLBITS_MASK, cfg.nlBits);
    val = __METAL_SET_FIELD(val, CLICCFG_NVBITS_MASK, cfg.nvBits);

    CLIC_REGB(METAL_SIFIVE_CLIC0_CLICCFG) = (uint8_t)val;
}

static struct clic_config get_clic_config(void) {
    struct clic_config cfg;
    uint8_t val = CLIC_REGB(METAL_SIFIVE_CLIC0_CLICCFG);

    cfg.nmBits = __METAL_GET_FIELD(val, CLICCFG_NMBITS_MASK);
    cfg.nlBits = __METAL_GET_FIELD(val, CLICCFG_NLBITS_MASK);
    cfg.nvBits = __METAL_GET_FIELD(val, CLICCFG_NVBITS_MASK);

    return cfg;
}

/* CPU API */

void metal_cpu_enable_ipi(void) {
    struct metal_interrupt clic = (struct metal_interrupt){0};
    sifive_clic0_enable(clic, RISCV_MACHINE_SOFTWARE_INTERRUPT_ID);
}

void metal_cpu_disable_ipi(void) {
    struct metal_interrupt clic = (struct metal_interrupt){0};
    sifive_clic0_disable(clic, RISCV_MACHINE_SOFTWARE_INTERRUPT_ID);
}

void metal_cpu_set_ipi(struct metal_cpu cpu) {
    struct metal_interrupt clic = (struct metal_interrupt){0};
    sifive_clic0_set(clic, RISCV_MACHINE_SOFTWARE_INTERRUPT_ID);
}

void metal_cpu_clear_ipi(struct metal_cpu cpu) {
    struct metal_interrupt clic = (struct metal_interrupt){0};
    sifive_clic0_clear(clic, RISCV_MACHINE_SOFTWARE_INTERRUPT_ID);
}

int metal_cpu_get_ipi(struct metal_cpu cpu) {
    struct metal_interrupt clic = (struct metal_interrupt){0};

    metal_vector_mode mode = sifive_clic0_get_vector_mode(clic);
    if (mode == METAL_CLIC_DIRECT_MODE || mode == METAL_CLIC_VECTORED_MODE) {
        /* In CLIC mode, check CLICINTIP */
        return CLIC_REGB(METAL_SIFIVE_CLIC0_CLICINTIP_BASE +
                         RISCV_MACHINE_SOFTWARE_INTERRUPT_ID) == 1;
    }

    /* In CLINT mode, check the mip CSR */
    riscv_xlen_t mip;
    __asm__ volatile("csrr %0, mip" : "=r"(mip));

    return !!(mip & RISCV_MIP_MSIP);
}

void metal_cpu_enable_timer_interrupt(void) {
    struct metal_interrupt clic = (struct metal_interrupt){0};
    sifive_clic0_enable(clic, RISCV_MACHINE_TIMER_INTERRUPT_ID);
}

void metal_cpu_disable_timer_interrupt(void) {
    struct metal_interrupt clic = (struct metal_interrupt){0};
    sifive_clic0_disable(clic, RISCV_MACHINE_TIMER_INTERRUPT_ID);
}

void metal_cpu_enable_external_interrupt(void) {
    struct metal_interrupt clic = (struct metal_interrupt){0};
    sifive_clic0_enable(clic, RISCV_MACHINE_EXTERNAL_INTERRUPT_ID);
}

void metal_cpu_disable_external_interrupt(void) {
    struct metal_interrupt clic = (struct metal_interrupt){0};
    sifive_clic0_disable(clic, RISCV_MACHINE_EXTERNAL_INTERRUPT_ID);
}

uint64_t metal_cpu_get_mtime(struct metal_cpu cpu) {
    uint64_t low, high;

    /* Guard against rollover when reading */
    do {
        high = CLIC_REGW(METAL_SIFIVE_CLIC0_MTIME + 4);
        low = CLIC_REGW(METAL_SIFIVE_CLIC0_MTIME);
    } while (CLIC_REGW(METAL_SIFIVE_CLIC0_MTIME + 4) != high);

    return (high << 32) | low;
}

int metal_cpu_set_mtimecmp(struct metal_cpu cpu, uint64_t time) {
    /* Per spec, the RISC-V MTIME/MTIMECMP registers are 64 bit,
     * and are NOT internally latched for multiword transfers.
     * Need to be careful about sequencing to avoid triggering
     * spurious interrupts: For that set the high word to a max
     * value first.
     */
    const uint32_t hartid = cpu.__hartid;

    CLIC_REGW(METAL_SIFIVE_CLIC0_MTIMECMP_BASE + (8 * hartid) + 4) = 0xFFFFFFFF;
    CLIC_REGW(METAL_SIFIVE_CLIC0_MTIMECMP_BASE + (8 * hartid)) =
        0xFFFFFFFF & time;
    CLIC_REGW(METAL_SIFIVE_CLIC0_MTIMECMP_BASE + (8 * hartid) + 4) =
        0xFFFFFFFF & (time >> 32);

    return 0;
}

/* Interrupt API */

void sifive_clic0_init(struct metal_interrupt clic) {
    if (!init_done) {
        /* Initialize the parent riscv,cpu-intc */
        struct metal_interrupt cpu_intc = (struct metal_interrupt){0};
        riscv_cpu_intc_init(cpu_intc);

        /* Initialize the CLIC configuration */
        set_clic_config(default_clic_config);

        /* Disable all interrupts and set their priority to 0 */
        for (uint32_t i = 0; i < CLIC_NUMINTS; i++) {
            sifive_clic0_set_priority(clic, i, 0);
            sifive_clic0_disable(clic, i);
        }

        /* Configure CLIC interrupt vectoring */
        clic_configure_vectoring();

        init_done = true;
    }
}

METAL_CONSTRUCTOR(init_sifive_clic0) {
    struct metal_interrupt clic = (struct metal_interrupt){0};
    sifive_clic0_init(clic);
}

int sifive_clic0_set_vector_mode(struct metal_interrupt clic,
                                 metal_vector_mode mode) {
    struct clic_config cfg = get_clic_config();

    switch (mode) {
    default:
    case METAL_VECTORED_MODE:
    case METAL_DIRECT_MODE:
    case METAL_CLIC_VECTORED_MODE:
        cfg.nvBits = false;
        break;
    case METAL_CLIC_DIRECT_MODE:
        /* Enable selective hardware vectoring only when we're in CLIC
         * mode when not all interrupts are vectored.
         */
        cfg.nvBits = true;
        break;
    }

    set_clic_config(cfg);

    return 0;
}

metal_vector_mode sifive_clic0_get_vector_mode(struct metal_interrupt clic) {
    riscv_xlen_t mtvec;
    __asm__ volatile("csrr %0, mtvec" : "=r"(mtvec));

    switch (__METAL_GET_FIELD(mtvec, RISCV_MTVEC_MODE_MASK)) {
    default:
    case RISCV_MTVEC_MODE_DIRECT:
        return METAL_DIRECT_MODE;
    case RISCV_MTVEC_MODE_VECTORED:
        return METAL_VECTORED_MODE;
    case RISCV_MTVEC_MODE_CLIC_DIRECT:
        return METAL_CLIC_DIRECT_MODE;
    case RISCV_MTVEC_MODE_CLIC_VECTORED:
        return METAL_CLIC_VECTORED_MODE;
    }
}

int sifive_clic0_clear(struct metal_interrupt clic, int id) {
    if (id > CLIC_NUMINTS)
        return -1;

    /* Only the software interrupt can be directly cleared */
    if (id != RISCV_MACHINE_SOFTWARE_INTERRUPT_ID &&
        id != SIFIVE_CLIC_SOFTWARE_INTERRUPT_ID) {
        return -2;
    }

    metal_vector_mode mode = sifive_clic0_get_vector_mode(clic);
    if (mode == METAL_CLIC_DIRECT_MODE || mode == METAL_CLIC_VECTORED_MODE) {
        /* If we're in CLIC mode, use CLICINTIP */
        CLIC_REGB(METAL_SIFIVE_CLIC0_CLICINTIP_BASE + id) = 0;
    } else {
        /* If we're not in CLIC mode, we use MSIP register */
        CLIC_REGW(METAL_SIFIVE_CLIC0_MSIP_BASE) = 0;
    }

    return 0;
}

int sifive_clic0_set(struct metal_interrupt clic, int id) {
    if (id > CLIC_NUMINTS)
        return -1;

    /* Only the software interrupt can be directly set */
    if (id != RISCV_MACHINE_SOFTWARE_INTERRUPT_ID &&
        id != SIFIVE_CLIC_SOFTWARE_INTERRUPT_ID) {
        return -2;
    }

    metal_vector_mode mode = sifive_clic0_get_vector_mode(clic);
    if (mode == METAL_CLIC_DIRECT_MODE || mode == METAL_CLIC_VECTORED_MODE) {
        /* If we're in CLIC mode, use CLICINTIP */
        CLIC_REGB(METAL_SIFIVE_CLIC0_CLICINTIP_BASE + id) = 1;
    } else {
        /* If we're not in CLIC mode, we use MSIP register */
        CLIC_REGW(METAL_SIFIVE_CLIC0_MSIP_BASE) = 1;
    }

    return 0;
}

int sifive_clic0_enable(struct metal_interrupt clic, int id) {
    if (id > CLIC_NUMINTS)
        return -1;

    metal_vector_mode mode = sifive_clic0_get_vector_mode(clic);
    if (mode == METAL_CLIC_DIRECT_MODE || mode == METAL_CLIC_VECTORED_MODE) {
        /* If we're in CLIC mode, we use clicIntIE */
        CLIC_REGB(METAL_SIFIVE_CLIC0_CLICINTIE_BASE + id) = 1;
        return 0;
    }

    /* If we're not in CLIC mode, we use mie CSR */
    switch (id) {
    default:
        /* Unsupported interrupt ID enable in CLINT mode */
        return -1;
    case RISCV_MACHINE_SOFTWARE_INTERRUPT_ID:
        __asm__ volatile("csrs mie, %0" ::"r"(RISCV_MIE_MSIE));
        break;
    case RISCV_MACHINE_TIMER_INTERRUPT_ID:
        __asm__ volatile("csrs mie, %0" ::"r"(RISCV_MIE_MTIE));
        break;
    case RISCV_MACHINE_EXTERNAL_INTERRUPT_ID:
        __asm__ volatile("csrs mie, %0" ::"r"(RISCV_MIE_MEIE));
        break;
    }

    return 0;
}

int sifive_clic0_disable(struct metal_interrupt clic, int id) {
    if (id > CLIC_NUMINTS)
        return -1;

    metal_vector_mode mode = sifive_clic0_get_vector_mode(clic);
    if (mode == METAL_CLIC_DIRECT_MODE || mode == METAL_CLIC_VECTORED_MODE) {
        /* If we're in CLIC mode, we use clicIntIE */
        CLIC_REGB(METAL_SIFIVE_CLIC0_CLICINTIE_BASE + id) = 0;
        return 0;
    }

    /* If we're not in CLIC mode, we use mie CSR */
    switch (id) {
    default:
        /* Unsupported interrupt ID enable in CLINT mode */
        return -1;
    case RISCV_MACHINE_SOFTWARE_INTERRUPT_ID:
        __asm__ volatile("csrc mie, %0" ::"r"(RISCV_MIE_MSIE));
        break;
    case RISCV_MACHINE_TIMER_INTERRUPT_ID:
        __asm__ volatile("csrc mie, %0" ::"r"(RISCV_MIE_MTIE));
        break;
    case RISCV_MACHINE_EXTERNAL_INTERRUPT_ID:
        __asm__ volatile("csrc mie, %0" ::"r"(RISCV_MIE_MEIE));
        break;
    }

    return 0;
}

int sifive_clic0_set_threshold(struct metal_interrupt clic,
                               unsigned int level) {
    /* The CLIC does not have a configurable threshold */
    return -1;
}

unsigned int sifive_clic0_get_threshold(struct metal_interrupt clic) {
    /* The CLIC does not have a configurable threshold */
    return 0;
}

int sifive_clic0_set_priority(struct metal_interrupt clic, int id,
                              unsigned int priority) {
    if (id > CLIC_NUMINTS)
        return -1;

    const struct clic_config cfg = get_clic_config();

    uint8_t max_priority = 0;
    if (cfg.nvBits) {
        /* Selective vectoring is enabled, so we have numintbits - 1
         * to encode priority */
        max_priority = (1 << (CLIC_NUMINTBITS - 1)) - 1;
    } else {
        /* Selective vectoring is disabled, so we have all numintbits
         * to encode priority */
        max_priority = (1 << CLIC_NUMINTBITS) - 1;
    }

    /* Clamp priority to the maximum value */
    if (priority > max_priority) {
        priority = max_priority;
    }

    const uint8_t clicintcfg = (priority << (8 - CLIC_NUMINTBITS + cfg.nvBits));
    CLIC_REGB(METAL_SIFIVE_CLIC0_CLICINTCFG_BASE + id) = clicintcfg;

    return 0;
}

unsigned int sifive_clic0_get_priority(struct metal_interrupt clic, int id) {
    if (id > CLIC_NUMINTS)
        return 0;

    const uint8_t clicintcfg =
        CLIC_REGB(METAL_SIFIVE_CLIC0_CLICINTCFG_BASE + id);

    const struct clic_config cfg = get_clic_config();

    return (clicintcfg >> (8 - CLIC_NUMINTBITS + cfg.nvBits));
}

int sifive_clic0_vector_enable(struct metal_interrupt clic, int id) {
    if (id > CLIC_NUMINTS)
        return -1;

    /* Selective vectoring is controlled by the least significant
     * implemented bit of clicIntCfg. */
    const uint8_t mask = 1 << (8 - CLIC_NUMINTBITS);

    switch (sifive_clic0_get_vector_mode(clic)) {
    case METAL_CLIC_DIRECT_MODE:
        CLIC_REGB(METAL_SIFIVE_CLIC0_CLICINTCFG_BASE + id) |= mask;
        break;
    case METAL_CLIC_VECTORED_MODE:
    case METAL_VECTORED_MODE:
        break;
    default:
    case METAL_DIRECT_MODE:
        return -1;
    }

    return 0;
}

int sifive_clic0_vector_disable(struct metal_interrupt clic, int id) {
    if (id > CLIC_NUMINTS)
        return -1;

    /* Selective vectoring is controlled by the least significant
     * implemented bit of clicIntCfg. */
    const uint8_t mask = 1 << (8 - CLIC_NUMINTBITS);

    switch (sifive_clic0_get_vector_mode(clic)) {
    case METAL_CLIC_DIRECT_MODE:
        CLIC_REGB(METAL_SIFIVE_CLIC0_CLICINTCFG_BASE + id) &= ~mask;
        break;
    case METAL_CLIC_VECTORED_MODE:
    case METAL_VECTORED_MODE:
        return -1;
    default:
    case METAL_DIRECT_MODE:
        break;
    }

    return 0;
}

#endif /* METAL_SIFIVE_CLIC0 */

typedef int no_empty_translation_units;
