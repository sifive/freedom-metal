/* Copyright 2020 SiFive, Inc */
/* SPDX-License-Identifier: Apache-2.0 */

#include <metal/crypto.h>
#include <metal/drivers/sifive_hca0.h>
#include <metal/platform.h>

#ifdef METAL_SIFIVE_HCA0

#include <metal/drivers/sifive_hca0_regs.h>
#include <metal/io.h>
#include <metal/private/metal_private_sifive_hca0.h>

#define get_index(hca) ((hca).__hca_index)

uint32_t sifive_hca0_getrev(struct sifive_hca0 hca) {
    HCA_Type *hca_regs;

    if (get_index(hca) == METAL_HCA_INVALID_INDEX)
        return 0;

    hca_regs = (HCA_Type *)dt_hca_data[get_index(hca)].hca_regs;

    return (hca_regs->HCA_REV);
}

#else /* METAL_SIFIVE_HCA0 */

/* Stubs for when no HCA TRNG is present */
uint32_t sifive_hca0_getrev(struct sifive_hca0 hca) { return 0; }

#endif /* METAL_SIFIVE_HCA0 */
