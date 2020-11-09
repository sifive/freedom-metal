/* Copyright 2020 SiFive, Inc */
/* SPDX-License-Identifier: Apache-2.0 */

#ifndef METAL__DRIVERS__SIFIVE_HCA0_H
#define METAL__DRIVERS__SIFIVE_HCA0_H

/*!
 * @file sifive_hca0.h
 * @brief
 */

#include <assert.h>
#include <metal/crypto.h>
#include <metal/interrupt.h>
#include <metal/io.h>
#include <metal/private/metal_private_hca0.h>
#include <stdint.h>
#include <stdio.h>
/**
 * @addtogroup HCA0
 *  @{
 */

typedef enum { HCA_AES_MODE = 0, HCA_SHA_MODE = 1 } hca0_mode_t;

/*!
 * @brief Handle for a HCA device
 */
struct sifive_hca0 {
    uint32_t __hca_index;
};

#define METAL_HCA_INVALID_INDEX UINT32_MAX

#ifndef bswap16
/*! @brief swap 16 bit words  */
#define bswap16(x) __bswap16(x)
#endif

#ifndef bswap32
/*! @brief swap 32 bit words  */
#define bswap32(x) __bswap32(x)
#endif

#ifndef bswap64
/*! @brief swap 64 bit words  */
#define bswap64(x) __bswap64(x)
#endif

/*! @brief Macro to copy 32 (no alignement constraint) to 32 (aligned) */
#define GET_32BITS(data, k)                                                    \
    ((((uint32_t) * ((const uint8_t *)(data) + (k) + 3)) << 24) +              \
     (((uint32_t) * ((const uint8_t *)(data) + (k) + 2)) << 16) +              \
     (((uint32_t) * ((const uint8_t *)(data) + (k) + 1)) << 8) +               \
     ((uint32_t) * ((const uint8_t *)(data) + (k))))

/*! @brief Macro to copy 64 (no alignement constraint) to 64 (aligned) */
#define GET_64BITS(data, k)                                                    \
    ((((uint64_t)GET_32BITS((const uint8_t *)data, (k + 4))) << 32) +          \
     (uint64_t)GET_32BITS((const uint8_t *)data, k))

#ifndef IS_ALIGNED_4_BYTES
/**
 * @brief Test is a pointer is aliged on a 4-byte address
 */
#define IS_ALIGNED_4_BYTES(p) (!(((uintptr_t)(p)) & 0x3u))
#endif

#ifndef IS_ALIGNED_8_BYTES
/**
 * @brief Test is a pointer is aliged on a 8-byte address
 */
#define IS_ALIGNED_8_BYTES(p) (!(((uintptr_t)(p)) & 0x7u))
#endif

/*!
 * @brief Get a handle for a HCA device
 * @param device_num The index of the desired HCA device
 * @return A handle to the HCA device, or NULL if the device does not exist
 */
__attribute__((always_inline)) inline struct sifive_hca0
sifive_hca0_get_device(uint32_t index) {
#if __METAL_DT_NUM_HCA0S > 0
    if (index < __METAL_DT_NUM_HCA0S)
        return (struct sifive_hca0){index};
#endif
    return (struct sifive_hca0){METAL_HCA_INVALID_INDEX};
}

uint32_t sifive_hca0_getrev(struct sifive_hca0 hca) __attribute__((weak));

/** @}*/
#endif /* METAL__DRIVERS__SIFIVE_HCA0_H */
