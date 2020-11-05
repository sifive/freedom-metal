/* Copyright 2020 SiFive, Inc */
/* SPDX-License-Identifier: Apache-2.0 */

#include <metal/hca.h>
#include <metal/hca_sha.h>

/* Stubs for when no HCA SHA is present */
int32_t metal_hca_sha_init(struct metal_hca hca,
    hca_sha_ctx_t *const ctx,
    metal_crypto_hash_mode_t hash_mode,
    metal_crypto_endianness_t data_endianness)
    __attribute__((weak));
int32_t metal_hca_sha_init(struct metal_hca hca,
    hca_sha_ctx_t *const ctx,
    metal_crypto_hash_mode_t hash_mode,
    metal_crypto_endianness_t data_endianness)
{ return -1; }

int32_t metal_hca_sha_core(struct metal_hca hca,
    hca_sha_ctx_t *const ctx,
    const uint8_t *const data,
    size_t data_byte_len)
    __attribute__((weak));
int32_t metal_hca_sha_core(struct metal_hca hca,
    hca_sha_ctx_t *const ctx,
    const uint8_t *const data,
    size_t data_byte_len)
{ return -1; }

int32_t metal_hca_sha_finish(struct metal_hca hca,
    hca_sha_ctx_t *const ctx,
    uint8_t *const hash,
    size_t *const hash_len)
    __attribute__((weak));
int32_t metal_hca_sha_finish(struct metal_hca hca,
    hca_sha_ctx_t *const ctx,
    uint8_t *const hash,
    size_t *const hash_len)
{ return -1; }
