/* Copyright 2020 SiFive, Inc */
/* SPDX-License-Identifier: Apache-2.0 */

#include <metal/crypto.h>
#include <metal/drivers/sifive_hca0.h>
#include <metal/platform.h>

#ifdef METAL_SIFIVE_HCA0

#include <metal/io.h>
#include <metal/private/metal_private_sifive_hca0.h>
#include <metal/private/metal_private_sifive_hca0_trng.h>

#define get_index(hca) ((hca).__hca_index)

uint32_t sifive_hca0_trng_getrev(struct sifive_hca0 hca) {
    HCA_Type *hca_regs;

    if (get_index(hca) == METAL_HCA_INVALID_INDEX)
        return 0;

    hca_regs = (HCA_Type *)dt_hca_data[get_index(hca)].hca_regs;

    return (hca_regs->TRNG_REV);
}

int32_t sifive_hca0_trng_init(struct sifive_hca0 hca) {
    volatile uint32_t reg32;
    int32_t ret = METAL_CRYPTO_OK;
    HCA_Type *hca_regs;

    if (get_index(hca) == METAL_HCA_INVALID_INDEX)
        return METAL_CRYPTO_ERROR;

    hca_regs = (HCA_Type *)dt_hca_data[get_index(hca)].hca_regs;

    if (0 == (hca_regs->TRNG_REV)) {
        // revision of TRNG is Zero so the TRNG is not present.
        return METAL_CRYPTO_ERROR;
    }

    // Lock Trim Value
    reg32 = hca_regs->TRNG_TRIM;
    reg32 &= ~(HCA_TRNG_TRIM_LOCK_Msk);
    reg32 |= ((1 << HCA_TRNG_TRIM_LOCK_Pos) & HCA_TRNG_TRIM_LOCK_Msk);
    hca_regs->TRNG_TRIM = reg32;

    // start on-demand health test
    reg32 = hca_regs->TRNG_CR;
    reg32 &= ~(HCA_TRNG_CR_HTSTART_Msk);
    reg32 |= ((1 << HCA_TRNG_CR_HTSTART_Pos) & HCA_TRNG_CR_HTSTART_Msk);
    hca_regs->TRNG_CR = reg32;

    while (hca_regs->TRNG_SR & HCA_TRNG_SR_HTR_Msk) {
        // test that all 0's are read back from TRNG_DATA during startup health
        // test
        if (hca_regs->TRNG_DATA != 0) {
            if (hca_regs->TRNG_SR & HCA_TRNG_SR_HTR_Msk) {
                return METAL_CRYPTO_RNG_ERROR;
            }
        }
    }

    // Test Heath test status
    if ((hca_regs->TRNG_SR & HCA_TRNG_SR_HTR_Msk) != 0) {
        ret = METAL_CRYPTO_RNG_ERROR;
    }

    // stop on-demand health test
    reg32 = hca_regs->TRNG_CR;
    reg32 &= ~(HCA_TRNG_CR_HTSTART_Msk);
    hca_regs->TRNG_CR = reg32;

    return ret;
}

int32_t sifive_hca0_trng_getdata(struct sifive_hca0 hca, uint32_t *data_out) {
    volatile uint32_t reg32;
    int32_t ret = METAL_CRYPTO_OK;
    HCA_Type *hca_regs;

    if (get_index(hca) == METAL_HCA_INVALID_INDEX)
        return METAL_CRYPTO_ERROR;

    hca_regs = (HCA_Type *)dt_hca_data[get_index(hca)].hca_regs;

    if (0 == (hca_regs->TRNG_REV)) {
        // revision of TRNG is Zero so the TRNG is not present.
        return METAL_CRYPTO_ERROR;
    }

    // Poll for RNDRDY bit
    while ((hca_regs->TRNG_SR & HCA_TRNG_SR_RNDRDY_Msk) == 0) {
        __asm__ volatile("nop");
    }

    // read TRNG_DATA register
    *data_out = hca_regs->TRNG_DATA;

    return METAL_CRYPTO_OK;
}

#else /* METAL_SIFIVE_HCA0 */

/* Stubs for when no HCA TRNG is present */
uint32_t sifive_hca0_trng_getrev(struct sifive_hca0 hca) { return 0; }

int32_t sifive_hca0_trng_init(struct sifive_hca0 hca) {
    return METAL_CRYPTO_ERROR;
}

int32_t sifive_hca0_trng_getdata(struct sifive_hca0 hca, uint32_t *data_out) {
    return METAL_CRYPTO_ERROR;
}

#endif /* METAL_SIFIVE_HCA0 */
