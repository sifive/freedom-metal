/* Copyright 2021 SiFive, Inc */
/* SPDX-License-Identifier: Apache-2.0 */

#include <metal/machine/platform.h>

#ifdef METAL_SIFIVE_L2PF1

#include <metal/drivers/sifive_l2pf1.h>
#include <metal/machine.h>
#include <stdint.h>

/* Macros to access memory mapped registers. */
#define REGW(x) *((volatile uint32_t *)(l2pf_base[hartid] + x))

/* Macros for register bit masks. */
#define REG_MASK_BITWIDTH_1 0x01
#define REG_MASK_BITWIDTH_2 0x03
#define REG_MASK_BITWIDTH_4 0x0f
#define REG_MASK_BITWIDTH_5 0x1f
#define REG_MASK_BITWIDTH_6 0x3f

/* Macros to specify register bit shift. */
#define REG_BITSHIFT_2 2
#define REG_BITSHIFT_4 4
#define REG_BITSHIFT_8 8
#define REG_BITSHIFT_9 9
#define REG_BITSHIFT_13 13
#define REG_BITSHIFT_14 14
#define REG_BITSHIFT_19 19
#define REG_BITSHIFT_20 20
#define REG_BITSHIFT_21 21
#define REG_BITSHIFT_28 28
#define REG_BITSHIFT_29 29

/* Array of base addresses with HART IDs as the index. */
unsigned long l2pf_base[] = METAL_SIFIVE_L2PF1_BASE_ADDR;
int l2pf_base_len = sizeof(l2pf_base) / sizeof(unsigned long);

/* Array of distance bits with HART IDs as the index. */
unsigned long l2pf_distance_bits[] = METAL_SIFIVE_L2PF1_DISTANCE_BITS;

static void _sifive_l2pf1_set_config(int hartid, sifive_l2pf1_config *config) {
    uint32_t val;

    /* Check for NULL, valid base address. */
    if ((config) && (hartid < l2pf_base_len) && (l2pf_base[hartid] != 0UL)) {
        uint32_t distance_bits_mask = (1 << l2pf_distance_bits[hartid]) - 1;

        /* Set basic control register configuration values. */
        val = (uint32_t)(
            (config->ScalarLoadSupportEn & REG_MASK_BITWIDTH_1) |
            ((config->Dist & distance_bits_mask) << REG_BITSHIFT_2) |
            ((config->MaxAllowedDist & distance_bits_mask) << REG_BITSHIFT_8) |
            ((config->LinToExpThrd & distance_bits_mask) << REG_BITSHIFT_14) |
            ((config->CrossPageEn & REG_MASK_BITWIDTH_1) << REG_BITSHIFT_28) |
            ((config->ForgiveThrd & REG_MASK_BITWIDTH_1) << REG_BITSHIFT_29));

        /* Set L2 user bits control register configuration values. */
        REGW(METAL_SIFIVE_L2PF1_BASIC_CONTROL) = val;

        val = (uint32_t)(
            (config->QFullnessThrd & REG_MASK_BITWIDTH_4) |
            ((config->HitCacheThrd & REG_MASK_BITWIDTH_5) << REG_BITSHIFT_4) |
            ((config->HitMSHRThrd & REG_MASK_BITWIDTH_4) << REG_BITSHIFT_9) |
            ((config->Window & REG_MASK_BITWIDTH_6) << REG_BITSHIFT_13) |
            ((config->ScalarStoreSupportEn & REG_MASK_BITWIDTH_1)
             << REG_BITSHIFT_19) |
            ((config->VectorLoadSupportEn & REG_MASK_BITWIDTH_1)
             << REG_BITSHIFT_20) |
            ((config->VectorStoreSupportEn & REG_MASK_BITWIDTH_1)
             << REG_BITSHIFT_21));

        REGW(METAL_SIFIVE_L2PF1_USER_CONTROL) = val;
    }
}

static void _sifive_l2pf1_set_all_harts_config(sifive_l2pf1_config *config) {
    for (int hartid = 0; hartid < l2pf_base_len; hartid++) {
        _sifive_l2pf1_set_config(hartid, config);
    }
}

void sifive_l2pf1_enable(void) {
    int hartid;
    __asm__ volatile("csrr %0, mhartid" : "=r"(hartid));

    if ((hartid < l2pf_base_len) && (l2pf_base[hartid] != 0UL)) {
        uint32_t val = REGW(METAL_SIFIVE_L2PF1_BASIC_CONTROL);

        /* Enable L2 stride prefetch unit for current hart. */
        val |= REG_MASK_BITWIDTH_1;

        REGW(METAL_SIFIVE_L2PF1_BASIC_CONTROL) = val;
    }
}

void sifive_l2pf1_disable(void) {
    int hartid;
    __asm__ volatile("csrr %0, mhartid" : "=r"(hartid));

    if ((hartid < l2pf_base_len) && (l2pf_base[hartid] != 0UL)) {
        uint32_t val = REGW(METAL_SIFIVE_L2PF1_BASIC_CONTROL);

        /* Disable L2 stride prefetch unit for current hart. */
        val &= ~REG_MASK_BITWIDTH_1;

        REGW(METAL_SIFIVE_L2PF1_BASIC_CONTROL) = val;
    }
}

void sifive_l2pf1_get_config(sifive_l2pf1_config *config) {
    int hartid;
    __asm__ volatile("csrr %0, mhartid" : "=r"(hartid));
    uint32_t val;

    /* Check for NULL, valid base address. */
    if ((config) && (hartid < l2pf_base_len) && (l2pf_base[hartid] != 0UL)) {
        uint32_t distance_bits_mask = (1 << l2pf_distance_bits[hartid]) - 1;

        /* Get basic control register configuration values. */
        val = REGW(METAL_SIFIVE_L2PF1_BASIC_CONTROL);

        config->ScalarLoadSupportEn = (val & REG_MASK_BITWIDTH_1);
        config->Dist = ((val >> REG_BITSHIFT_2) & distance_bits_mask);
        config->MaxAllowedDist = ((val >> REG_BITSHIFT_8) & distance_bits_mask);
        config->LinToExpThrd = ((val >> REG_BITSHIFT_14) & distance_bits_mask);
        config->CrossPageEn = ((val >> REG_BITSHIFT_28) & REG_MASK_BITWIDTH_1);
        config->ForgiveThrd = ((val >> REG_BITSHIFT_29) & REG_MASK_BITWIDTH_2);

        /* Get L2 user bits control register configuration values. */
        val = REGW(METAL_SIFIVE_L2PF1_USER_CONTROL);

        config->QFullnessThrd = (val & REG_MASK_BITWIDTH_4);
        config->HitCacheThrd = ((val >> REG_BITSHIFT_4) & REG_MASK_BITWIDTH_5);
        config->HitMSHRThrd = ((val >> REG_BITSHIFT_9) & REG_MASK_BITWIDTH_4);
        config->Window = ((val >> REG_BITSHIFT_13) & REG_MASK_BITWIDTH_6);
        config->ScalarStoreSupportEn =
            ((val >> REG_BITSHIFT_19) & REG_MASK_BITWIDTH_1);
        config->VectorLoadSupportEn =
            ((val >> REG_BITSHIFT_20) & REG_MASK_BITWIDTH_1);
        config->VectorStoreSupportEn =
            ((val >> REG_BITSHIFT_21) & REG_MASK_BITWIDTH_1);
    }
}

void sifive_l2pf1_set_config(sifive_l2pf1_config *config) {
    int hartid;
    __asm__ volatile("csrr %0, mhartid" : "=r"(hartid));
    _sifive_l2pf1_set_config(hartid, config);
}

void sifive_l2pf1_init(void) {
    sifive_l2pf1_config config;

    /* Basic control register initial configuration (0x15811). */
    config.ScalarLoadSupportEn = 0x1;
    config.Dist = 0x4;
    config.MaxAllowedDist = 0x18;
    config.LinToExpThrd = 0x5;

    /* L2 user bits control register initial configuration (0x38c84e). */
    config.QFullnessThrd = 0xe;
    config.HitCacheThrd = 0x4;
    config.HitMSHRThrd = 0x4;
    config.Window = 0x6;
    config.ScalarStoreSupportEn = 0x1;
    config.VectorLoadSupportEn = 0x1;
    config.VectorStoreSupportEn = 0x1;

    _sifive_l2pf1_set_all_harts_config(&config);
}

#endif

typedef int no_empty_translation_units;
