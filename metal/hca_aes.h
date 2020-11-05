/* Copyright 2020 SiFive, Inc */
/* SPDX-License-Identifier: Apache-2.0 */

#ifndef METAL__HCA_AES_H
#define METAL__HCA_AES_H

/*!
 * @file hca_aes.h
 * @brief API for AES crypto operation through HCA
 */

#include <assert.h>
#include <metal/interrupt.h>
#include <metal/crypto.h>
#include <metal/hca.h>
#include <metal/private/metal_private_hca0.h>
#include <stdint.h>

#define BLOCK128_NB_BYTE        16

#define BLOCK128_NB_UINT64      2
#define BLOCK128_NB_UINT32      4

/*! @brief AES context for AES auth modes */
typedef struct { 
    /*! Total Hash lenght */
    uint64_t pld_len;
    /*! lenght of data stored into buffer */
    size_t buf_len;
    /*! buffer storage for incomplet data */
    uint64_t buf[BLOCK128_NB_UINT64]    __attribute__ ((aligned (8)));
    /*! buffer storage for incomplet data */
    metal_crypto_endianness_t data_endianness;
} hca_aes_auth_ctx_t;

/**
 * @addtogroup HCA
 * @addtogroup HCA_AES_API
 * @ingroup HCA
 *  @{
 */

/**
 * @brief set AES key
 *
 * @param[in] hca               The HCA device handle
 * @param[in] type              key type (128, 192, 256) @ref metal_crypto_aes_key_type_t
 * @param[in] key               key value (256 bits in any case)
 * @param[in] aes_process       aes process (encrypt or decrypt)
 * @return 0                    SUCCESS
 * @return != 0                 otherwise @ref metal_crypto_errors_t
 */
int32_t metal_hca_aes_setkey(struct metal_hca hca,
                                       metal_crypto_aes_key_type_t type,
                                       const uint64_t *const key,
                                       metal_crypto_process_t aes_process);

/**
 * @brief set IV (Initial Value)
 *
 * @param[in] hca               The HCA device handle
 * @param[in] iv                iv value (128 bits)
 * @return 0                    SUCCESS
 * @return != 0                 otherwise @ref metal_crypto_errors_t
 */
int32_t metal_hca_aes_setiv(struct metal_hca hca,
                                      const uint64_t *const iv);

/**
 * @brief perform AES cipher operation
 *
 * @param[in] hca               The HCA device handle
 * @param[in] aes_mode          AES mode @ref metal_crypto_aes_mode_t
 * @param[in] aes_process       AES process (encrypt or decrypt)
 * @param[in] data_endianness   endianess of the input data
 * @param[in] data_in           data to process
 * @param[in] data_len          length of the data to process (in byte)
 * @param[out] data_out         data output buffer
 * @return 0                    SUCCESS
 * @return != 0                 otherwise @ref metal_crypto_errors_t
 */
int32_t metal_hca_aes_cipher(
    struct metal_hca hca, metal_crypto_aes_mode_t aes_mode,
    metal_crypto_process_t aes_process, metal_crypto_endianness_t data_endianness,
    const uint8_t *const data_in, size_t data_len, uint8_t *const data_out);

/**
 * @brief initiliaze AES cipher with authentication operation
 *
 * @param[in] hca               The HCA device handle
 * @param[in,out] ctx           aes authenticate context
 * @param[in] aes_mode          AES mode @ref metal_crypto_aes_mode_t
 * @param[in] aes_process       AES process (encrypt or decrypt)
 * @param[in] data_endianness   endianess of the input data
 * @param[in] auth_option       option for the ccm mode
 * @param[in] aad               add data
 * @param[in] aad_byte_len      length of the add data (in bytes)
 * @param[in] payload_len       length of the payload data (in bytes) 
 * @return 0                    SUCCESS
 * @return != 0                 otherwise @ref metal_crypto_errors_t
 */
int32_t metal_hca_aes_auth_init(
    struct metal_hca hca, hca_aes_auth_ctx_t *const ctx, 
    metal_crypto_aes_mode_t aes_mode, metal_crypto_process_t aes_process,
    metal_crypto_endianness_t data_endianness, uint32_t auth_option,
    const uint8_t *const aad, size_t aad_byte_len, uint64_t payload_len);

/**
 * @brief perform AES cipher with authentication operation
 *
 * @param[in] hca               The HCA device handle
 * @param[in,out] ctx           aes authenticate context
 * @param[in] payload           data payload to process
 * @param[in] payload_len       length of the current data payload to process (in bytes)
 * @param[out] data_out         data output buffer
 * @param[out] out_len          length of data (in bytes) write into output buffer
 * @return 0                    SUCCESS
 * @return != 0                 otherwise @ref metal_crypto_errors_t
 */
int32_t metal_hca_aes_auth_core(struct metal_hca hca, 
                                          hca_aes_auth_ctx_t *const ctx,
                                          const uint8_t *const payload, 
                                          uint64_t payload_len, 
                                          uint8_t *const data_out, 
                                          size_t *const out_len);

/**
 * @brief finish AES cipher with authentication operation
 *
 * @param[in] hca               The HCA device handle
 * @param[in,out] ctx           aes authenticate context
 * @param[out] data_out         data output buffer to complete operation
 * @param[out] tag              tag output buffer (128 bits)
 * @return 0                    SUCCESS
 * @return != 0                 otherwise @ref metal_crypto_errors_t
 */
int32_t metal_hca_aes_auth_finish(struct metal_hca hca, 
                                            hca_aes_auth_ctx_t *const ctx, 
                                            uint8_t *const data_out, 
                                            uint64_t *const tag);

/** @}*/
#endif /* METAL__HCA_AES_H */
