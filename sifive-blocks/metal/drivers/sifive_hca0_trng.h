/* Copyright 2020 SiFive, Inc */
/* SPDX-License-Identifier: Apache-2.0 */

#ifndef METAL__DRIVERS__SIFIVE_HCA0_TRNG_H
#define METAL__DRIVERS__SIFIVE_HCA0_TRNG_H

/*!
 * @file sifive_hca0_trng.h
 * @brief API for TRNG crypto operation through HCA
 */

#include <assert.h>
#include <metal/crypto.h>
#include <metal/drivers/sifive_hca0.h>
#include <metal/interrupt.h>
#include <stdint.h>

/**
 * @addtogroup HCA
 * @addtogroup HCA_TRNG_API
 * @ingroup HCA
 *  @{
 */

uint32_t sifive_hca0_trng_getrev(struct sifive_hca0 hca) __attribute__((weak));

int32_t sifive_hca0_trng_init(struct sifive_hca0 hca) __attribute__((weak));

int32_t sifive_hca0_trng_getdata(struct sifive_hca0 hca, uint32_t *data_out)
    __attribute__((weak));

/** @}*/
#endif /* METAL__DRIVERS__SIFIVE_HCA0_TRNG_H */
