/* Copyright 2021 SiFive, Inc */
/* SPDX-License-Identifier: Apache-2.0 */

#include <metal/machine/platform.h>

#ifdef METAL_SIFIVE_MALLARD0

#include <metal/cpu.h>
#include <metal/csr.h>
#include <metal/drivers/sifive_mallard0.h>
#include <metal/init.h>
#include <metal/machine.h>
#include <metal/mallard.h>

#define MALLARD_TWINJET_CSR_WRITE(csr, val)                                    \
    ({                                                                         \
        if (csr == CSR_HWPF1CSR)                                               \
            METAL_CPU_SET_CSR(CSR_HWPF1CSR, val)                               \
        else if (csr == CSR_HWPF2CSR)                                          \
            METAL_CPU_SET_CSR(CSR_HWPF2CSR, val)                               \
    })

#define MALLARD_TWINJET_CSR_READ(csr, val)                                     \
    ({                                                                         \
        if (csr == CSR_HWPF1CSR)                                               \
            METAL_CPU_GET_CSR(CSR_HWPF1CSR, val)                               \
        else if (csr == CSR_HWPF2CSR)                                          \
            METAL_CPU_GET_CSR(CSR_HWPF2CSR, val)                               \
    })

static uint64_t sifive_mallard0_twinjet_l1_get(void) {
    uint64_t value;

    METAL_CPU_GET_CSR(CSR_HWPF1CSR, value);

    return value;
}

static uint64_t sifive_mallard0_twinjet_l2_get(void) {
    uint64_t value;

    METAL_CPU_GET_CSR(CSR_HWPF2CSR, value);

    return value;
}

static void sifive_mallard0_twinjet_l1_set(uint64_t value) {
    METAL_CPU_SET_CSR(CSR_HWPF1CSR, value);
}

static void sifive_mallard0_twinjet_l2_set(uint64_t value) {
    METAL_CPU_SET_CSR(CSR_HWPF2CSR, value);
}

static int sifive_mallard0_twinjet_toggle(uint32_t reg, uint64_t enable) {
    uint64_t value;

    MALLARD_TWINJET_CSR_READ(reg, value);

    if (enable) {
        MALLARD_TWINJET_CSR_WRITE(reg, value | HWPFCSR_ENABLE_MASK);
    } else {
        MALLARD_TWINJET_CSR_WRITE(reg, value & ~HWPFCSR_ENABLE_MASK);
    }

    MALLARD_TWINJET_CSR_READ(reg, value);

    if ((value & HWPFCSR_ENABLE_MASK) != enable)
        return -1;

    return 0;
}

static int sifive_mallard0_twinjet_l1_enable(void) {
    return sifive_mallard0_twinjet_toggle(CSR_HWPF1CSR, 1);
}

static int sifive_mallard0_twinjet_l2_enable(void) {
    return sifive_mallard0_twinjet_toggle(CSR_HWPF2CSR, 1);
}

static int sifive_mallard0_twinjet_l1_disable(void) {
    return sifive_mallard0_twinjet_toggle(CSR_HWPF1CSR, 0);
}

static int sifive_mallard0_twinjet_l2_disable(void) {
    return sifive_mallard0_twinjet_toggle(CSR_HWPF2CSR, 0);
}

struct mallard_twinjet_ops __metal_driver_sifive_mallard0_twinjet_ops = {
    .l1_enable = sifive_mallard0_twinjet_l1_enable,
    .l2_enable = sifive_mallard0_twinjet_l2_enable,
    .l1_disable = sifive_mallard0_twinjet_l1_disable,
    .l2_disable = sifive_mallard0_twinjet_l2_disable,
    .l1_set = sifive_mallard0_twinjet_l1_set,
    .l2_set = sifive_mallard0_twinjet_l2_set,
    .l1_get = sifive_mallard0_twinjet_l1_get,
    .l2_get = sifive_mallard0_twinjet_l2_get,
};

struct mallard_twinjet __metal_driver_sifive_mallard0_twinjet = {
    .ops = &__metal_driver_sifive_mallard0_twinjet_ops,
};

struct metal_sifive_mallard0 __metal_driver_sifive_mallard0 = {
    .twinjet = &__metal_driver_sifive_mallard0_twinjet,
};

static void __metal_driver_sifive_mallard0_init(void) {
    uint64_t value;

    value = MALLARD_HWPF1CSR_ENABLE << HWPF1CSR_ENABLE_OFFSET |
            MALLARD_HWPF1CSR_WINDOW << HWPF1CSR_WINDOW_OFFSET |
            MALLARD_HWPF1CSR_DIST << HWPF1CSR_DIST_OFFSET |
            MALLARD_HWPF1CSR_MAXALLOWEDDIST << HWPF1CSR_MAXALLOWEDDIST_OFFSET |
            MALLARD_HWPF1CSR_LINTOEXPTHRD << HWPF1CSR_LINTOEXPTHRD_OFFSET |
            MALLARD_HWPF1CSR_QFULLNESSTHRDL1
                << HWPF1CSR_QFULLNESSTHRDL1_OFFSET |
            MALLARD_HWPF1CSR_HITCACHETHRDL1 << HWPF1CSR_HITCACHETHRDL1_OFFSET |
            MALLARD_HWPF1CSR_HITMSHRTHRDL1 << HWPF1CSR_HITMSHRTHRDL1_OFFSET |
            MALLARD_HWPF1CSR_ISSUEBUBBLE << HWPF1CSR_ISSUEBUBBLE_OFFSET |
            MALLARD_HWPF1CSR_MAXL1PFDIST << HWPF1CSR_MAXL1PFDIST_OFFSET |
            MALLARD_HWPF1CSR_FORGIVETHRD << HWPF1CSR_FORGIVETHRD_OFFSET |
            MALLARD_HWPF1CSR_NUML1PFLSSQENT << HWPF1CSR_NUML1PFLSSQENT_OFFSET;
    METAL_CPU_SET_CSR(CSR_HWPF1CSR, value);

    value = MALLARD_HWPF2CSR_L2PFENABLE << HWPF2CSR_L2PFENABLE_OFFSET |
            MALLARD_HWPF2CSR_QFULLNESSTHRDL2
                << HWPF2CSR_QFULLNESSTHRDL2_OFFSET |
            MALLARD_HWPF2CSR_HITCACHETHRDL2 << HWPF2CSR_HITCACHETHRDL2_OFFSET |
            MALLARD_HWPF2CSR_HITMSHRTHRDL2 << HWPF2CSR_HITMSHRTHRDL2_OFFSET |
            MALLARD_HWPF2CSR_NUML2PFLSSQENT << HWPF2CSR_NUML2PFLSSQENT_OFFSET;
    METAL_CPU_SET_CSR(CSR_HWPF2CSR, value);

    metal_cpu_set_cpu_specific(metal_cpu_get(metal_cpu_get_current_hartid()),
                               &__metal_driver_sifive_mallard0);
}

METAL_CONSTRUCTOR(sifive_mallard0_init) {
    __metal_driver_sifive_mallard0_init();
}

#endif /* METAL_SIFIVE_MALLARD0 */

typedef int no_empty_translation_units;
