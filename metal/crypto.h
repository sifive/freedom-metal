/* Copyright 2020 SiFive, Inc */
/* SPDX-License-Identifier: Apache-2.0 */

#ifndef METAL__CRYPTO_H
#define METAL__CRYPTO_H

/*! @brief CRYPTO operation error codes */
typedef enum
{
    /*! @brief no error */
    METAL_CRYPTO_OK = 0,
    /*! @brief generic error code */
    METAL_CRYPTO_ERROR = -1,
    /*! @brief error on input parameter */
    METAL_CRYPTO_INVALID_INPUT = -2,
    /*! @brief error on output parameter */
    METAL_CRYPTO_INVALID_OUTPUT = -3,
    /*! @brief error invalid operation mode selected */
    METAL_CRYPTO_INVALID_MODE = -4,
    /*! @brief error invalid length */
    METAL_CRYPTO_INVALID_LENGTH = -5,
    /*! @brief error on Random Number Generation */
    METAL_CRYPTO_RNG_ERROR = -13,
    /*! @brief error busy */
    METAL_CRYPTO_BUSY = -16,
    /*! @brief error zero division */
    METAL_CRYPTO_ZERO_DIVISION = -17,
    /**
     * @brief One of the entry point, use by this function or in the call tree
     * use an API pointer undeclared or misplaced
     */
    METAL_CRYPTO_ERROR_API_ENTRY_POINT = -18,
    /*! @brief error not modular inversion possible */
    METAL_CRYPTO_NOT_INVERSIBLE = -19,
    /*! @brief error on parity */
    METAL_CRYPTO_ERR_PARITY = -20,

    /*! @brief error signature */
    METAL_CRYPTO_ERR_SIGNATURE = -21,
    /*! @brief error invalid hash */
    METAL_CRYPTO_ERR_HASH = -22,

    /*! @brief error functionnality not present */
    METAL_CRYPTO_NOT_PRESENT = -30,
    /*! @brief error functionnality not yet supported */
    METAL_CRYPTO_NOT_YET_SUPPORTED = -31,
} metal_crypto_errors_t;

typedef enum
{
    /*! @brief little endian words */
    METAL_CRYPTO_LITTLE_ENDIAN_MODE = 0,
    /*! @brief big endian words */
    METAL_CRYPTO_BIG_ENDIAN_MODE = 1
} metal_crypto_endianness_t;

/*! @brief  symmetric-crypto encryption mode */
typedef enum
{
    METAL_CRYPTO_ENCRYPT = 0,
    METAL_CRYPTO_DECRYPT = 1
} metal_crypto_process_t;

/*! @brief AES key bit lenght definition */
typedef enum
{
    /*! @brief Define for char lenght of 128 bits key */
    METAL_CRYPTO_KEY128 = 16,
    /*! @brief Define for char lenght of 192 bits key */
    METAL_CRYPTO_KEY192 = 24,
    /*! @brief Define for char lenght of 256 bits key */
    METAL_CRYPTO_KEY256 = 32,
} metal_crypto_key_size_t;

/*! @brief AES key lenght supported */
typedef enum
{
    /*! @brief Define for 128 bits key lenght */
    METAL_CRYPTO_AES_KEY128 = 0,
    /*! @brief Define for 192 bits key lenght */
    METAL_CRYPTO_AES_KEY192 = 1,
    /*! @brief Define for 256 bits key lenght */
    METAL_CRYPTO_AES_KEY256 = 2,
} metal_crypto_aes_key_type_t;

/*! @brief AES mode supported */
typedef enum
{
    /*! @brief Define ECB mode */
    METAL_CRYPTO_AES_ECB = 0,
    /*! @brief Define CBC mode */
    METAL_CRYPTO_AES_CBC = 1,
    /*! @brief Define CFB mode */
    METAL_CRYPTO_AES_CFB = 2,
    /*! @brief Define OFB mode */
    METAL_CRYPTO_AES_OFB = 3,
    /*! @brief Define CTR mode */
    METAL_CRYPTO_AES_CTR = 4,
    /*! @brief Define GCM mode */
    METAL_CRYPTO_AES_GCM = 5,
    /*! @brief Define CCM mode */
    METAL_CRYPTO_AES_CCM = 6
} metal_crypto_aes_mode_t;

/*! @brief Hash mode supported */
typedef enum
{
    /*! @brief Define SHA224 mode */
    METAL_CRYPTO_HASH_SHA224 = 0,
    /*! @brief Define SHA256 mode */
    METAL_CRYPTO_HASH_SHA256 = 1,
    /*! @brief Define SHA384 mode */
    METAL_CRYPTO_HASH_SHA384 = 2,
    /*! @brief Define SHA512 mode */
    METAL_CRYPTO_HASH_SHA512 = 3
} metal_crypto_hash_mode_t;

/*! @brief Hash size of SHA operation in bytes */
typedef enum
{
    /*! @brief Define size of SHA224 in bytes */
    METAL_CRYPTO_HASH_SHA224_BYTE_SIZE = 28,
    /*! @brief Define size of SHA256 in bytes */
    METAL_CRYPTO_HASH_SHA256_BYTE_SIZE = 32,
    /*! @brief Define size of SHA384 in bytes */
    METAL_CRYPTO_HASH_SHA384_BYTE_SIZE = 48,
    /*! @brief Define size of SHA512 in bytes */
    METAL_CRYPTO_HASH_SHA512_BYTE_SIZE = 64
} metal_crypto_hash_sha_byte_size_mode_t;

#endif /* METAL__CRYPTO_H */
