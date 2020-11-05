/* Copyright 2020 SiFive, Inc */
/* SPDX-License-Identifier: Apache-2.0 */

#include <metal/hca.h>
#include <metal/hca_aes.h>

/* Stubs for when no HCA AES driver is present */
int32_t metal_hca_aes_setkey(struct metal_hca hca,
                                       metal_crypto_aes_key_type_t type,
                                       const uint64_t *const key,
                                       metal_crypto_process_t aes_process)
    __attribute__((weak));
int32_t metal_hca_aes_setkey(struct metal_hca hca,
                                       metal_crypto_aes_key_type_t type,
                                       const uint64_t *const key,
                                       metal_crypto_process_t aes_process)
{ return -1; }

int32_t metal_hca_aes_setiv(struct metal_hca hca,
                                      const uint64_t *const iv);

int32_t metal_hca_aes_cipher(
    struct metal_hca hca, metal_crypto_aes_mode_t aes_mode,
    metal_crypto_process_t aes_process, metal_crypto_endianness_t data_endianness,
    const uint8_t *const data_in, size_t data_len, uint8_t *const data_out)
    __attribute__((weak));
int32_t metal_hca_aes_cipher(
    struct metal_hca hca, metal_crypto_aes_mode_t aes_mode,
    metal_crypto_process_t aes_process, metal_crypto_endianness_t data_endianness,
    const uint8_t *const data_in, size_t data_len, uint8_t *const data_out)
{ return -1; }

int32_t metal_hca_aes_auth_init(
    struct metal_hca hca, hca_aes_auth_ctx_t *const ctx, 
    metal_crypto_aes_mode_t aes_mode, metal_crypto_process_t aes_process,
    metal_crypto_endianness_t data_endianness, uint32_t auth_option,
    const uint8_t *const aad, size_t aad_byte_len, uint64_t payload_len)
    __attribute__((weak));
int32_t metal_hca_aes_auth_init(
    struct metal_hca hca, hca_aes_auth_ctx_t *const ctx, 
    metal_crypto_aes_mode_t aes_mode, metal_crypto_process_t aes_process,
    metal_crypto_endianness_t data_endianness, uint32_t auth_option,
    const uint8_t *const aad, size_t aad_byte_len, uint64_t payload_len)
{ return -1; }

int32_t metal_hca_aes_auth_core(struct metal_hca hca, 
                                          hca_aes_auth_ctx_t *const ctx,
                                          const uint8_t *const payload, 
                                          uint64_t payload_len, 
                                          uint8_t *const data_out, 
                                          size_t *const out_len)
    __attribute__((weak));

int32_t metal_hca_aes_auth_core(struct metal_hca hca, 
                                          hca_aes_auth_ctx_t *const ctx,
                                          const uint8_t *const payload, 
                                          uint64_t payload_len, 
                                          uint8_t *const data_out, 
                                          size_t *const out_len)
{ return -1; }

int32_t hca_aes_auth_finish(struct metal_hca hca, 
                                            hca_aes_auth_ctx_t *const ctx, 
                                            uint8_t *const data_out, 
                                            uint64_t *const tag)
    __attribute__((weak));
int32_t hca_aes_auth_finish(struct metal_hca hca, 
                                            hca_aes_auth_ctx_t *const ctx, 
                                            uint8_t *const data_out, 
                                            uint64_t *const tag)
{ return -1; }
