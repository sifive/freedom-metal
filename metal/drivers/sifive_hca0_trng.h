/* Copyright 2020 SiFive, Inc */
/* SPDX-License-Identifier: Apache-2.0 */

#ifndef METAL__HCA_TRNG_H
#define METAL__HCA_TRNG_H

/*!
 * @file hca_trng.h
 * @brief API for TRNG crypto operation through HCA
 */

#include <assert.h>
#include <metal/interrupt.h>
#include <metal/crypto.h>
#include <metal/drivers/sifive_hca0.h>
#include <stdint.h>

/**
 * @addtogroup HCA
 * @addtogroup HCA_TRNG_API
 * @ingroup HCA
 *  @{
 */

uint32_t sifive_hca0_trng_getrev(struct sifive_hca0 hca) __attribute__((weak));

int32_t sifive_hca0_trng_init(struct sifive_hca0 hca) __attribute__((weak));

int32_t sifive_hca0_trng_getdata(struct sifive_hca0 hca, uint32_t *data_out) __attribute__((weak));

/** @}*/
#endif /* METAL__HCA_TRNG_H */
