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
#include <metal/hca.h>
#include <stdint.h>

/**
 * @addtogroup HCA
 * @addtogroup HCA_TRNG_API
 * @ingroup HCA
 *  @{
 */

uint32_t metal_hca_trng_getrev(struct metal_hca hca);

int32_t metal_hca_trng_init(struct metal_hca hca);

int32_t metal_hca_trng_getdata(struct metal_hca hca, uint32_t *data_out);

/** @}*/
#endif /* METAL__HCA_TRNG_H */
