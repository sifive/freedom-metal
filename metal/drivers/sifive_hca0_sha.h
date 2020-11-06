/* Copyright 2020 SiFive, Inc */
/* SPDX-License-Identifier: Apache-2.0 */

#ifndef METAL__HCA_SHA_H
#define METAL__HCA_SHA_H

/*!
 * @file hca_sha.h
 * @brief API for AES crypto operation through HCA
 */

#include <assert.h>
#include <metal/crypto.h>
#include <metal/drivers/sifive_hca0.h>
#include <metal/interrupt.h>
#include <stdint.h>

/*! @brief SHA256 block size */
#define SHA256_BYTE_BLOCKSIZE 64
/*! @brief SHA512 block size */
#define SHA512_BYTE_BLOCKSIZE 128

/*! @brief number of rounds per SHA256 block computation */
#define SHA256_ROUNDS_NUMBER 64
/*! @brief number of rounds per SHA512 block computation */
#define SHA512_ROUNDS_NUMBER 80

/*! @brief number of words (32 bits) in hash */
#define SHA256_SIZE_WORDS 8
/*! @brief number of words (64 bits) in hash */
#define SHA512_SIZE_WORDS 8

/*! @brief number of word (32 bits) in one block */
#define SHA256_BLOCK_WORDS 16
/*! @brief number of word (64 bits) in one block */
#define SHA512_BLOCK_WORDS 16

/*! @brief the nb of bytes for storing the size in the last block */
#define SHA256_BYTE_SIZE_BLOCKSIZE 8
/*! @brief The nb of bytes for storing the size in the last block */
#define SHA512_BYTE_SIZE_BLOCKSIZE 16

/*! @brief SHA 256 context */
typedef struct {
    /*! @brief intermediate state and then final hash */
    uint32_t h[SHA256_SIZE_WORDS];
    /*! @brief total bits length computed */
    uint64_t bitlen;
    /*! @brief block buffer */
    uint8_t block_buffer[SHA256_BYTE_BLOCKSIZE] __attribute__((aligned(4)));
} hca_sha256_ctx_t;
/**
 * @brief SHA224 context
 * @note Since SHA224 use SHA256 block computation, it can also use the same
 * context
 */
typedef hca_sha256_ctx_t hca_sha224_ctx_t;

/*! @brief SHA 512 context */
typedef struct {
    /*! @brief Initial, intermediate and then final hash. */
    uint64_t h[SHA512_SIZE_WORDS];
    /*! @brief bit len */
    uint64_t bitlen;
    /*! @brief  block buffer */
    uint8_t block_buffer[SHA512_BYTE_BLOCKSIZE] __attribute__((aligned(8)));
} hca_sha512_ctx_t;

/**
 * @brief SHA384 context
 * @note Since SHA384 use SHA512 block computation, it can also use the same
 * context
 */
typedef hca_sha512_ctx_t hca_sha384_ctx_t;

/*! @brief Unified SHA context */
typedef struct {
    /*! @brief supported SHA contexts union */
    union {
        hca_sha224_ctx_t sha224;
        hca_sha256_ctx_t sha256;
        hca_sha384_ctx_t sha384;
        hca_sha512_ctx_t sha512;
    } ctx;
    /*! Hash mode  */
    metal_crypto_hash_mode_t mode;
} hca0_sha_ctx_t;

/**
 * @addtogroup HCA
 * @addtogroup HCA_SHA_API
 * @ingroup HCA
 *  @{
 */

/**
 * @brief Init hardware sha context
 *
 * @param[in] scl               metal scl context
 * @param[out] ctx              sha context
 * @param[in] hash_mode         hash mode
 * @param[in] data_endianness   sclendianess of the input data
 * @return 0                    SUCCESS
 * @return != 0                 otherwise @ref scl_errors_t
 */
int32_t sifive_hca0_sha_init(struct sifive_hca0 hca, hca0_sha_ctx_t *const ctx,
                             metal_crypto_hash_mode_t hash_mode,
                             metal_crypto_endianness_t data_endianness)
    __attribute__((weak));

/**
 * @brief Compute intermediate hash value of the chunk of data in parameter
 *
 * @param[in] scl               metal scl context
 * @param[in,out] ctx           sha context
 * @param[in] data              data to hash
 * @param[in] data_byte_len     data length to hash
 * @return 0                    SUCCESS
 * @return != 0                 otherwise @ref scl_errors_t
 */
int32_t sifive_hca0_sha_core(struct sifive_hca0 hca, hca0_sha_ctx_t *const ctx,
                             const uint8_t *const data, size_t data_byte_len)
    __attribute__((weak));

/**
 * @brief Compute final hash value of the concatenated block pass to
 * hca_sha_core()
 *
 * @param[in] scl               metal scl context
 * @param[in] ctx               sha context
 * @param[out] hash             hash output buffer
 * @param[in,out] hash_len      length of the hash buffer/length of the hash
 * @return 0                    SUCCESS
 * @return != 0                 otherwise @ref scl_errors_t
 */
int32_t sifive_hca0_sha_finish(struct sifive_hca0 hca,
                               hca0_sha_ctx_t *const ctx, uint8_t *const hash,
                               size_t *const hash_len) __attribute__((weak));

/** @}*/
#endif /* METAL__HCA_SHA_H */
