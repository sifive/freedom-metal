/* Copyright 2020 SiFive, Inc */
/* SPDX-License-Identifier: Apache-2.0 */

#include <metal/hca_trng.h>

/* Stubs for when no HCA TRNG is present */
uint32_t metal_hca_trng_getrev(struct metal_hca hca)
    __attribute__((weak));
uint32_t metal_hca_trng_getrev(struct metal_hca hca)
{ return 0; }

int32_t metal_hca_trng_init(struct metal_hca hca)
    __attribute__((weak));
int32_t metal_hca_trng_init(struct metal_hca hca)
{ return -1; }

int32_t metal_hca_trng_getdata(struct metal_hca hca, uint32_t *data_out)
    __attribute__((weak));
int32_t metal_hca_trng_getdata(struct metal_hca hca, uint32_t *data_out)
{ return -1; }
