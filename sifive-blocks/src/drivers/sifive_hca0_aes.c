/* Copyright 2020 SiFive, Inc */
/* SPDX-License-Identifier: Apache-2.0 */

#include <metal/crypto.h>
#include <metal/drivers/sifive_hca0.h>
#include <metal/drivers/sifive_hca0_aes.h>
#include <metal/platform.h>

#ifdef METAL_SIFIVE_HCA0

#include <limits.h>
#include <metal/io.h>
#include <metal/private/metal_private_sifive_hca0.h>
#include <metal/private/metal_private_sifive_hca0_aes.h>

#define get_index(hca) ((hca).__hca_index)

uint32_t sifive_hca0_aes_getrev(struct sifive_hca0 hca) {
    HCA_Type *hca_regs;

    if (get_index(hca) == METAL_HCA_INVALID_INDEX)
        return 0;

    hca_regs = (HCA_Type *)dt_hca_data[get_index(hca)].hca_regs;

    return (hca_regs->AES_REV);
}

int32_t sifive_hca0_aes_setkey(struct sifive_hca0 hca,
                               metal_crypto_aes_key_type_t type,
                               const uint64_t *const key,
                               metal_crypto_process_t aes_process) {
    volatile uint32_t reg32;
    HCA_Type *hca_regs;

    /*
     * Remove compiler warning about unused parameter.
     * This parameter could be use for a software aes implementation, if you
     * need to precompute aes key round.
     */
    (void)aes_process;

    if (get_index(hca) == METAL_HCA_INVALID_INDEX)
        return METAL_CRYPTO_ERROR;

    hca_regs = (HCA_Type *)dt_hca_data[get_index(hca)].hca_regs;

    if (0 == (hca_regs->AES_REV)) {
        // revision of AES is Zero so the AES is not present.
        return METAL_CRYPTO_ERROR;
    }

    // set the key size
    reg32 = hca_regs->AES_CR;
    reg32 &= ~(HCA_AES_CR_KEYSZ_Msk);
    reg32 |= ((type << HCA_AES_CR_KEYSZ_Pos) & HCA_AES_CR_KEYSZ_Msk);
    hca_regs->AES_CR = reg32;

    hca_regs->AES_KEY[0] = key[0];
    hca_regs->AES_KEY[1] = key[1];
    hca_regs->AES_KEY[2] = key[2];
    hca_regs->AES_KEY[3] = key[3];

    __asm__ volatile("fence.i"); // FENCE

    return METAL_CRYPTO_OK;
}

int32_t sifive_hca0_aes_setiv(struct sifive_hca0 hca,
                              const uint64_t *const iv) {
    volatile uint32_t reg32;
    HCA_Type *hca_regs;

    if (get_index(hca) == METAL_HCA_INVALID_INDEX)
        return METAL_CRYPTO_ERROR;

    hca_regs = (HCA_Type *)dt_hca_data[get_index(hca)].hca_regs;

    if (0 == (hca_regs->AES_REV)) {
        // revision of AES is Zero so the AES is not present.
        return METAL_CRYPTO_ERROR;
    }

    // Set Init Vec
    hca_regs->AES_INITV[0] = iv[0];
    hca_regs->AES_INITV[1] = iv[1];

    __asm__ volatile("fence.i"); // FENCE

    return METAL_CRYPTO_OK;
}

int32_t sifive_hca0_aes_cipher(struct sifive_hca0 hca,
                               metal_crypto_aes_mode_t aes_mode,
                               metal_crypto_process_t aes_process,
                               metal_crypto_endianness_t data_endianness,
                               const uint8_t *const data_in, size_t data_len,
                               uint8_t *const data_out) {
    volatile uint32_t reg32;
    uint64_t i, k;
    uint64_t NbBlocks128;
    HCA_Type *hca_regs;

    if (get_index(hca) == METAL_HCA_INVALID_INDEX)
        return METAL_CRYPTO_ERROR;

    hca_regs = (HCA_Type *)dt_hca_data[get_index(hca)].hca_regs;

    if (0 == (hca_regs->AES_REV)) {
        // revision of AES is Zero so the AES is not present.
        return METAL_CRYPTO_ERROR;
    }

    if (aes_mode > METAL_CRYPTO_AES_CTR)
        return METAL_CRYPTO_INVALID_MODE;

    // Set MODE
    reg32 = hca_regs->CR;
    reg32 &= ~(HCA_CR_IFIFOTGT_Msk);
    /*
     * AES mode --> HCA_AES_MODE = 0
     * reg32 |= ((HCA_AES_MODE << HCA_CR_IFIFOTGT_Pos) & HCA_CR_IFIFOTGT_Msk);
     */
    hca_regs->CR = reg32;

    // Set aes_mode
    reg32 = hca_regs->AES_CR;
    reg32 &= ~(HCA_AES_CR_MODE_Msk);
    reg32 |= ((aes_mode << HCA_AES_CR_MODE_Pos) & HCA_AES_CR_MODE_Msk);
    hca_regs->AES_CR = reg32;

    // Set aes_process
    reg32 = hca_regs->AES_CR;
    reg32 &= ~(HCA_AES_CR_PROCESS_Msk);
    reg32 |= ((aes_process << HCA_AES_CR_PROCESS_Pos) & HCA_AES_CR_PROCESS_Msk);
    hca_regs->AES_CR = reg32;

    // Set endianness
    reg32 = hca_regs->CR;
    reg32 &= ~(HCA_CR_ENDIANNESS_Msk);
    reg32 |=
        ((data_endianness << HCA_CR_ENDIANNESS_Pos) & HCA_CR_ENDIANNESS_Msk);
    hca_regs->CR = reg32;

    if (aes_mode != METAL_CRYPTO_AES_ECB) {
        // Set INIT
        reg32 = hca_regs->AES_CR;
        reg32 &= ~(HCA_AES_CR_INIT_Msk);
        reg32 |= ((1 << HCA_AES_CR_INIT_Pos) & HCA_AES_CR_INIT_Msk);
        hca_regs->AES_CR = reg32;
    }

    NbBlocks128 = (data_len >> 4);

    if (data_len & 0xF)
        return METAL_CRYPTO_NOT_YET_SUPPORTED;

    for (k = 0; k < NbBlocks128; k++) {
        /* Wait for IFIFOEMPTY is cleared */
        while (hca_regs->CR & HCA_CR_IFIFOFULL_Msk) {
            __asm__ volatile("nop");
        }

#if __riscv_xlen == 64
        if (!IS_ALIGNED_8_BYTES(data_in)) {
            i = k << 4;
            hca_regs->FIFO_IN = GET_64BITS(data_in, i);
            hca_regs->FIFO_IN = GET_64BITS(data_in, (i + 8));
        } else {
#pragma GCC diagnostic push
// data_in is known to be aligned on uint64_t
#pragma GCC diagnostic ignored "-Wcast-align"
            const uint64_t *in64 = (const uint64_t *)data_in;
#pragma GCC diagnostic pop
            i = k << 1;
            hca_regs->FIFO_IN = in64[i];
            hca_regs->FIFO_IN = in64[i + 1];
        }
#elif __riscv_xlen == 32
        if (!IS_ALIGNED_4_BYTES(data_in)) {
            i = k << 4;
            hca_regs->FIFO_IN = GET_32BITS(data_in, i);
            hca_regs->FIFO_IN = GET_32BITS(data_in, (i + 4));
            hca_regs->FIFO_IN = GET_32BITS(data_in, (i + 8));
            hca_regs->FIFO_IN = GET_32BITS(data_in, (i + 12));
        } else {
#pragma GCC diagnostic push
// data_in is known to be aligned on uint32_t
#pragma GCC diagnostic ignored "-Wcast-align"
            const uint32_t *in32 = (const uint32_t *)data_in;
#pragma GCC diagnostic pop
            i = k << 2;
            hca_regs->FIFO_IN = in32[i];
            hca_regs->FIFO_IN = in32[i + 1];
            hca_regs->FIFO_IN = in32[i + 2];
            hca_regs->FIFO_IN = in32[i + 3];
        }
#endif /* __riscv_xlen */

        // Wait for OFIFOEMPTY is cleared
        while (hca_regs->CR & HCA_CR_OFIFOEMPTY_Msk) {
            __asm__ volatile("nop");
        }

        // Read AES result
#if __riscv_xlen == 64
        if (!IS_ALIGNED_8_BYTES(data_out)) {
            register uint64_t val;
            i = k << 4;
            val = hca_regs->AES_OUT;
            data_out[i] = (uint8_t)val;
            data_out[i + 1] = (uint8_t)(val >> 8);
            data_out[i + 2] = (uint8_t)(val >> 16);
            data_out[i + 3] = (uint8_t)(val >> 24);
            data_out[i + 4] = (uint8_t)(val >> 32);
            data_out[i + 5] = (uint8_t)(val >> 40);
            data_out[i + 6] = (uint8_t)(val >> 48);
            data_out[i + 7] = (uint8_t)(val >> 56);
            val = hca_regs->AES_OUT;
            data_out[i + 8] = (uint8_t)val;
            data_out[i + 9] = (uint8_t)(val >> 8);
            data_out[i + 10] = (uint8_t)(val >> 16);
            data_out[i + 11] = (uint8_t)(val >> 24);
            data_out[i + 12] = (uint8_t)(val >> 32);
            data_out[i + 13] = (uint8_t)(val >> 40);
            data_out[i + 14] = (uint8_t)(val >> 48);
            data_out[i + 15] = (uint8_t)(val >> 56);
        } else {
#pragma GCC diagnostic push
// data_out is known to be aligned on uint64_t
#pragma GCC diagnostic ignored "-Wcast-align"
            uint64_t *out64 = (uint64_t *)data_out;
#pragma GCC diagnostic pop
            i = k << 1;
            out64[i] = hca_regs->AES_OUT;
            out64[i + 1] = hca_regs->AES_OUT;
        }
#elif __riscv_xlen == 32
        if (!IS_ALIGNED_4_BYTES(data_out)) {
            register uint32_t val;
            i = k << 4;
            val = hca_regs->AES_OUT;
            data_out[i] = (uint8_t)val;
            data_out[i + 1] = (uint8_t)(val >> 8);
            data_out[i + 2] = (uint8_t)(val >> 16);
            data_out[i + 3] = (uint8_t)(val >> 24);
            val = hca_regs->AES_OUT;
            data_out[i + 4] = (uint8_t)val;
            data_out[i + 5] = (uint8_t)(val >> 8);
            data_out[i + 6] = (uint8_t)(val >> 16);
            data_out[i + 7] = (uint8_t)(val >> 24);
            val = hca_regs->AES_OUT;
            data_out[i + 8] = (uint8_t)val;
            data_out[i + 9] = (uint8_t)(val >> 8);
            data_out[i + 10] = (uint8_t)(val >> 16);
            data_out[i + 11] = (uint8_t)(val >> 24);
            val = hca_regs->AES_OUT;
            data_out[i + 12] = (uint8_t)val;
            data_out[i + 13] = (uint8_t)(val >> 8);
            data_out[i + 14] = (uint8_t)(val >> 16);
            data_out[i + 15] = (uint8_t)(val >> 24);
        } else {
#pragma GCC diagnostic push
// data_out is known to be aligned on uint32_t
#pragma GCC diagnostic ignored "-Wcast-align"
            uint32_t *out32 = (uint32_t *)data_out;
#pragma GCC diagnostic pop
            i = k << 2;
            out32[i] = hca_regs->AES_OUT;
            out32[i + 1] = hca_regs->AES_OUT;
            out32[i + 2] = hca_regs->AES_OUT;
            out32[i + 3] = hca_regs->AES_OUT;
        }
#endif /* __riscv_xlen */
    }

    return METAL_CRYPTO_OK;
}

int32_t sifive_hca0_aes_auth_init(
    struct sifive_hca0 hca, hca0_aes_auth_ctx_t *const ctx,
    metal_crypto_aes_mode_t aes_mode, metal_crypto_process_t aes_process,
    metal_crypto_endianness_t data_endianness, uint32_t auth_option,
    const uint8_t *const aad, size_t aad_byte_len, size_t payload_len) {
    volatile uint32_t reg32;
    uint32_t i, j, k;
    uint64_t NbBlocks128;
    uint64_t tmp[BLOCK128_NB_UINT64] __attribute__((aligned(8)));
    uint8_t ccmt = 0;
    uint8_t ccmq = 0;
    HCA_Type *hca_regs;

    if (get_index(hca) == METAL_HCA_INVALID_INDEX)
        return METAL_CRYPTO_ERROR;

    hca_regs = (HCA_Type *)dt_hca_data[get_index(hca)].hca_regs;

    if (0 == (hca_regs->AES_REV)) {
        // revision of AES is Zero so the AES is not present.
        return METAL_CRYPTO_ERROR;
    }

    if ((aes_mode < METAL_CRYPTO_AES_GCM) ||
        (aes_mode > METAL_CRYPTO_AES_CCM)) {
        return METAL_CRYPTO_INVALID_MODE;
    }

    // Reset value for context
    ctx->pld_len = payload_len;
    ctx->buf[0] = 0;
    ctx->buf[1] = 0;
    ctx->buf_len = 0;
    ctx->data_endianness = data_endianness;

    if (aes_mode == METAL_CRYPTO_AES_CCM) {
        ccmt = (uint8_t)(auth_option & 0xF);
        ccmq = (uint8_t)((auth_option >> 4) & 0xF);
        // check CCMT value
        if ((ccmt < 1) || (ccmt > 8)) {
            return METAL_CRYPTO_INVALID_INPUT;
        }

        // check CCMQ value
        if ((ccmq < 2) || (ccmq > 8)) {
            return METAL_CRYPTO_INVALID_INPUT;
        }

        switch (ccmq) {
        case 2:
            if (payload_len >= (((uint64_t)1 << (ccmq * 8)))) {
                return METAL_CRYPTO_INVALID_INPUT;
            }
            break;
        case 3:
        case 4:
        case 5:
        case 6:
        case 7:
            if ((payload_len < (((uint64_t)1 << ((ccmq - 1) * 8)))) ||
                (payload_len >= (((uint64_t)1 << (ccmq * 8))))) {
                return METAL_CRYPTO_INVALID_INPUT;
            }
            break;
        case 8:
            if (payload_len < (((uint64_t)1 << ((ccmq - 1) * 8)))) {
                return METAL_CRYPTO_INVALID_INPUT;
            }
            break;
        default:
            return METAL_CRYPTO_INVALID_INPUT;
        }
    }

    // Set MODE
    reg32 = hca_regs->CR;
    reg32 &= ~(HCA_CR_IFIFOTGT_Msk);
    /*
     * AES mode --> HCA_AES_MODE = 0
     * reg32 |= ((HCA_AES_MODE << HCA_CR_IFIFOTGT_Pos) & HCA_CR_IFIFOTGT_Msk);
     */
    hca_regs->CR = reg32;

    // Set aes_mode
    reg32 = hca_regs->AES_CR;
    reg32 &= ~(HCA_AES_CR_MODE_Msk);
    reg32 |= ((aes_mode << HCA_AES_CR_MODE_Pos) & HCA_AES_CR_MODE_Msk);
    hca_regs->AES_CR = reg32;

    // Set aes_process
    reg32 = hca_regs->AES_CR;
    reg32 &= ~(HCA_AES_CR_PROCESS_Msk);
    reg32 |= ((aes_process << HCA_AES_CR_PROCESS_Pos) & HCA_AES_CR_PROCESS_Msk);
    hca_regs->AES_CR = reg32;

    // Set endianness
    reg32 = hca_regs->CR;
    reg32 &= ~(HCA_CR_ENDIANNESS_Msk);
    reg32 |=
        ((data_endianness << HCA_CR_ENDIANNESS_Pos) & HCA_CR_ENDIANNESS_Msk);
    hca_regs->CR = reg32;

    // Set AES_ALEN
    hca_regs->AES_ALEN = aad_byte_len;

    // Set AES_PLDLEN
    hca_regs->AES_PLDLEN = payload_len;

    if (aes_mode == METAL_CRYPTO_AES_CCM) {
        // Set CCMT
        reg32 = hca_regs->AES_CR;
        reg32 &= ~(HCA_AES_CR_CCMT_Msk);
        reg32 |=
            (((uint32_t)ccmt << HCA_AES_CR_CCMT_Pos) & HCA_AES_CR_CCMT_Msk);
        hca_regs->AES_CR = reg32;

        // Set CCMQ
        reg32 = hca_regs->AES_CR;
        reg32 &= ~(HCA_AES_CR_CCMQ_Msk);
        reg32 |= (((uint32_t)(ccmq - 1) << HCA_AES_CR_CCMQ_Pos) &
                  HCA_AES_CR_CCMQ_Msk);
        hca_regs->AES_CR = reg32;
    }

    if (aad_byte_len) {
        // AAD
        // Set DTYPE
        reg32 = hca_regs->AES_CR;
        reg32 &= ~(HCA_AES_CR_DTYPE_Msk);
        /*
         * Set DTYPE to 0
         * reg32 |= ((0 << HCA_AES_CR_DTYPE_Pos) & HCA_AES_CR_DTYPE_Msk);
         */
        hca_regs->AES_CR = reg32;

        NbBlocks128 = aad_byte_len / BLOCK128_NB_BYTE;

        for (k = 0; k < NbBlocks128; k++) {
            // Wait for IFIFOFULL is cleared
            while (hca_regs->CR & HCA_CR_IFIFOFULL_Msk) {
                __asm__ volatile("nop");
            }
#if __riscv_xlen == 64
            if (!IS_ALIGNED_8_BYTES(aad)) {
                /* get uint8_t index base on 128bits index */
                i = k * BLOCK128_NB_BYTE;
                hca_regs->FIFO_IN = GET_64BITS(aad, i);
                hca_regs->FIFO_IN = GET_64BITS(aad, (i + 8));
            } else {
#pragma GCC diagnostic push
// aad is known to be aligned on uint64_t
#pragma GCC diagnostic ignored "-Wcast-align"
                const uint64_t *aad64 = (const uint64_t *)aad;
#pragma GCC diagnostic pop
                /* get uint64_t index base on 128bits index */
                i = k * BLOCK128_NB_UINT64;
                hca_regs->FIFO_IN = aad64[i];
                hca_regs->FIFO_IN = aad64[i + 1];
            }
#elif __riscv_xlen == 32
            if (!IS_ALIGNED_4_BYTES(aad)) {
                /* get uint8_t index base on 128bits index */
                i = k * BLOCK128_NB_BYTE;
                hca_regs->FIFO_IN = GET_32BITS(aad, i);
                hca_regs->FIFO_IN = GET_32BITS(aad, (i + 4));
                hca_regs->FIFO_IN = GET_32BITS(aad, (i + 8));
                hca_regs->FIFO_IN = GET_32BITS(aad, (i + 12));
            } else {
#pragma GCC diagnostic push
// aad is known to be aligned on uint32_t
#pragma GCC diagnostic ignored "-Wcast-align"
                const uint32_t *aad32 = (const uint32_t *)aad;
#pragma GCC diagnostic pop
                /* get uint32_t index base on 128bits index */
                i = k * BLOCK128_NB_UINT32;
                hca_regs->FIFO_IN = aad32[i];
                hca_regs->FIFO_IN = aad32[i + 1];
                hca_regs->FIFO_IN = aad32[i + 2];
                hca_regs->FIFO_IN = aad32[i + 3];
            }
#endif /* __riscv_xlen */
        }

        // AAD rest
        i = aad_byte_len % BLOCK128_NB_BYTE;
        if (0 != i) {
            tmp[0] = 0;
            tmp[1] = 0;

            /* we take 2 uint64_t */
            if (i < sizeof(uint64_t)) {
                for (j = 0; j < i; j++) {
                    tmp[1] +=
                        ((uint64_t)(
                            *((const uint8_t *)(aad + (k * BLOCK128_NB_BYTE) +
                                                j))))
                        << (j * CHAR_BIT);
                }
            } else {
                tmp[1] = GET_64BITS(aad, (k * BLOCK128_NB_BYTE));

                if (i > sizeof(uint64_t)) {
                    for (j = 0; j < (i - sizeof(uint64_t)); j++) {
                        tmp[0] +=
                            ((uint64_t)(*(
                                (const uint8_t *)(aad + (k * BLOCK128_NB_BYTE) +
                                                  sizeof(uint64_t) + j))))
                            << (j * CHAR_BIT);
                    }
                }
            }

            /* Wait for IFIFOFULL is cleared */
            while (hca_regs->CR & HCA_CR_IFIFOFULL_Msk) {
                __asm__ volatile("nop");
            }

            /* Put 128bits to HCA_FIFO_IN */
#if __riscv_xlen == 64
            if (METAL_CRYPTO_LITTLE_ENDIAN_MODE == data_endianness) {
                hca_regs->FIFO_IN = tmp[0];
                hca_regs->FIFO_IN = tmp[1];
            } else {
                hca_regs->FIFO_IN = tmp[1];
                hca_regs->FIFO_IN = tmp[0];
            }
#elif __riscv_xlen == 32
            uint32_t *aad32 = (uint32_t *)tmp;
            if (METAL_CRYPTO_LITTLE_ENDIAN_MODE == data_endianness) {
                hca_regs->FIFO_IN = aad32[0];
                hca_regs->FIFO_IN = aad32[1];
                hca_regs->FIFO_IN = aad32[2];
                hca_regs->FIFO_IN = aad32[3];
            } else {
                hca_regs->FIFO_IN = aad32[2];
                hca_regs->FIFO_IN = aad32[3];
                hca_regs->FIFO_IN = aad32[0];
                hca_regs->FIFO_IN = aad32[1];
            }
#endif
        }
    }

    return METAL_CRYPTO_OK;
}

int32_t sifive_hca0_aes_auth_core(struct sifive_hca0 hca,
                                  hca0_aes_auth_ctx_t *const ctx,
                                  const uint8_t *const data_in,
                                  size_t payload_len, uint8_t *const data_out,
                                  size_t *const len_out) {
    volatile uint32_t reg32;
#if __riscv_xlen == 64
    const uint64_t *in64 = (const uint64_t *)data_in;
    uint64_t *out64 = (uint64_t *)data_out;
#elif __riscv_xlen == 32
    const uint32_t *in32 = (const uint32_t *)data_in;
    uint32_t *out32 = (uint32_t *)data_out;
#endif /* __riscv_xlen */
    size_t i, k;
    uint64_t NbBlocks128;
    size_t in_offset = 0, out_offset = 0;
    HCA_Type *hca_regs;

    if (get_index(hca) == METAL_HCA_INVALID_INDEX)
        return METAL_CRYPTO_ERROR;

    hca_regs = (HCA_Type *)dt_hca_data[get_index(hca)].hca_regs;

    if (0 == (hca_regs->AES_REV)) {
        // revision of AES is Zero so the AES is not present.
        return METAL_CRYPTO_ERROR;
    }

    if (NULL == ctx) {
        return METAL_CRYPTO_INVALID_INPUT;
    }

    if (payload_len) {
        if (NULL == data_in) {
            return METAL_CRYPTO_INVALID_INPUT;
        }

        if (NULL == data_out) {
            return METAL_CRYPTO_INVALID_INPUT;
        }
    }

    if (payload_len > ctx->pld_len) {
        return METAL_CRYPTO_INVALID_INPUT;
    }

    if (NULL == len_out) {
        return METAL_CRYPTO_INVALID_INPUT;
    }

    *len_out = 0;

    ctx->pld_len -= payload_len;

    // PLD
    // Set DTYPE
    // Wait for IFIFOFULL is cleared to be sure that we do not change the type
    // of data of previous data
    while (hca_regs->CR & HCA_CR_IFIFOFULL_Msk) {
        __asm__ volatile("nop");
    }

    // Set DTYPE
    reg32 = hca_regs->AES_CR;
    reg32 &= ~(HCA_AES_CR_DTYPE_Msk);
    reg32 |= ((1 << HCA_AES_CR_DTYPE_Pos) & HCA_AES_CR_DTYPE_Msk);
    hca_regs->AES_CR = reg32;

    if (0 != ctx->buf_len) {
        // Fill up the previous context
        in_offset = 0;
        if (ctx->buf_len < sizeof(uint64_t)) {
            for (i = 0; i < (sizeof(uint64_t) - ctx->buf_len); i++) {
                ctx->buf[1] += ((uint64_t)(*((const uint8_t *)(data_in + i))))
                               << ((ctx->buf_len + i) * CHAR_BIT);
            }
            ctx->buf_len += i;
            in_offset = i;
        }

        for (i = 0; i < ((2 * sizeof(uint64_t)) - ctx->buf_len); i++) {
            ctx->buf[0] +=
                ((uint64_t)(*((const uint8_t *)(data_in + i))))
                << ((ctx->buf_len - sizeof(uint64_t) + i) * CHAR_BIT);
        }
        in_offset += i;

#if __riscv_xlen == 64
        in64 = (uint64_t *)(ctx->buf);
        if (METAL_CRYPTO_LITTLE_ENDIAN_MODE == ctx->data_endianness) {
            hca_regs->FIFO_IN = in64[0];
            hca_regs->FIFO_IN = in64[1];
        } else {
            hca_regs->FIFO_IN = in64[1];
            hca_regs->FIFO_IN = in64[0];
        }
        in64 = (const uint64_t *)(data_in + in_offset);
#elif __riscv_xlen == 32
        in32 = (uint32_t *)(ctx->buf);
        if (METAL_CRYPTO_LITTLE_ENDIAN_MODE == ctx->data_endianness) {
            hca_regs->FIFO_IN = in32[0];
            hca_regs->FIFO_IN = in32[1];
            hca_regs->FIFO_IN = in32[2];
            hca_regs->FIFO_IN = in32[3];
        } else {
            hca_regs->FIFO_IN = in32[2];
            hca_regs->FIFO_IN = in32[3];
            hca_regs->FIFO_IN = in32[0];
            hca_regs->FIFO_IN = in32[1];
        }
        in32 = (const uint32_t *)(data_in + in_offset);
#endif /* __riscv_xlen */

        // Wait for OFIFOEMPTY is cleared
        while (hca_regs->CR & HCA_CR_OFIFOEMPTY_Msk) {
            __asm__ volatile("nop");
        }

        // Read output result
#if __riscv_xlen == 64
        if (!IS_ALIGNED_8_BYTES(data_out)) {
            register uint64_t val;
            val = hca_regs->AES_OUT;
            data_out[0] = (uint8_t)val;
            data_out[1] = (uint8_t)(val >> 8);
            data_out[2] = (uint8_t)(val >> 16);
            data_out[3] = (uint8_t)(val >> 24);
            data_out[4] = (uint8_t)(val >> 32);
            data_out[5] = (uint8_t)(val >> 40);
            data_out[6] = (uint8_t)(val >> 48);
            data_out[7] = (uint8_t)(val >> 56);
            val = hca_regs->AES_OUT;
            data_out[8] = (uint8_t)val;
            data_out[9] = (uint8_t)(val >> 8);
            data_out[10] = (uint8_t)(val >> 16);
            data_out[11] = (uint8_t)(val >> 24);
            data_out[12] = (uint8_t)(val >> 32);
            data_out[13] = (uint8_t)(val >> 40);
            data_out[14] = (uint8_t)(val >> 48);
            data_out[15] = (uint8_t)(val >> 56);
        } else {
            out64[0] = hca_regs->AES_OUT;
            out64[1] = hca_regs->AES_OUT;
        }
#elif __riscv_xlen == 32
        if (!IS_ALIGNED_4_BYTES(data_out)) {
            register uint32_t val;
            val = hca_regs->AES_OUT;
            data_out[0] = (uint8_t)val;
            data_out[1] = (uint8_t)(val >> 8);
            data_out[2] = (uint8_t)(val >> 16);
            data_out[3] = (uint8_t)(val >> 24);
            val = hca_regs->AES_OUT;
            data_out[4] = (uint8_t)val;
            data_out[5] = (uint8_t)(val >> 8);
            data_out[6] = (uint8_t)(val >> 16);
            data_out[7] = (uint8_t)(val >> 24);
            val = hca_regs->AES_OUT;
            data_out[8] = (uint8_t)val;
            data_out[9] = (uint8_t)(val >> 8);
            data_out[10] = (uint8_t)(val >> 16);
            data_out[11] = (uint8_t)(val >> 24);
            val = hca_regs->AES_OUT;
            data_out[12] = (uint8_t)val;
            data_out[13] = (uint8_t)(val >> 8);
            data_out[14] = (uint8_t)(val >> 16);
            data_out[15] = (uint8_t)(val >> 24);
        } else {
            out32[0] = hca_regs->AES_OUT;
            out32[1] = hca_regs->AES_OUT;
            out32[2] = hca_regs->AES_OUT;
            out32[3] = hca_regs->AES_OUT;
        }
#endif /* __riscv_xlen */
        out_offset = BLOCK128_NB_BYTE;
    }

    // No reming data
    NbBlocks128 = ((payload_len - in_offset) / BLOCK128_NB_BYTE);

    for (k = 0; k < NbBlocks128; k++) {
        /* Wait for IFIFOFULL is cleared */
        while (hca_regs->CR & HCA_CR_IFIFOFULL_Msk) {
            __asm__ volatile("nop");
        }
#if __riscv_xlen == 64
        if (!IS_ALIGNED_8_BYTES(data_in)) {
            i = in_offset + (k * BLOCK128_NB_BYTE);
            hca_regs->FIFO_IN = GET_64BITS(data_in, i);
            hca_regs->FIFO_IN = GET_64BITS(data_in, (i + 8));
        } else {
            i = in_offset + (k * BLOCK128_NB_UINT64);
            hca_regs->FIFO_IN = in64[i];
            hca_regs->FIFO_IN = in64[i + 1];
        }
#elif __riscv_xlen == 32
        if (!IS_ALIGNED_4_BYTES(data_in)) {
            i = in_offset + (k * BLOCK128_NB_BYTE);
            hca_regs->FIFO_IN = GET_32BITS(data_in, i);
            hca_regs->FIFO_IN = GET_32BITS(data_in, (i + 4));
            hca_regs->FIFO_IN = GET_32BITS(data_in, (i + 8));
            hca_regs->FIFO_IN = GET_32BITS(data_in, (i + 12));
        } else {
            i = in_offset + (k * BLOCK128_NB_UINT32);
            hca_regs->FIFO_IN = in32[i];
            hca_regs->FIFO_IN = in32[i + 1];
            hca_regs->FIFO_IN = in32[i + 2];
            hca_regs->FIFO_IN = in32[i + 3];
        }
#endif /* __riscv_xlen */

        /* Wait for OFIFOEMPTY is cleared */
        while (hca_regs->CR & HCA_CR_OFIFOEMPTY_Msk) {
            __asm__ volatile("nop");
        }

        // Read AES result
#if __riscv_xlen == 64
        if (!IS_ALIGNED_8_BYTES(data_out)) {
            register uint64_t val;
            val = hca_regs->AES_OUT;
            data_out[out_offset] = (uint8_t)val;
            data_out[out_offset + 1] = (uint8_t)(val >> 8);
            data_out[out_offset + 2] = (uint8_t)(val >> 16);
            data_out[out_offset + 3] = (uint8_t)(val >> 24);
            data_out[out_offset + 4] = (uint8_t)(val >> 32);
            data_out[out_offset + 5] = (uint8_t)(val >> 40);
            data_out[out_offset + 6] = (uint8_t)(val >> 48);
            data_out[out_offset + 7] = (uint8_t)(val >> 56);
            val = hca_regs->AES_OUT;
            data_out[out_offset + 8] = (uint8_t)val;
            data_out[out_offset + 9] = (uint8_t)(val >> 8);
            data_out[out_offset + 10] = (uint8_t)(val >> 16);
            data_out[out_offset + 11] = (uint8_t)(val >> 24);
            data_out[out_offset + 12] = (uint8_t)(val >> 32);
            data_out[out_offset + 13] = (uint8_t)(val >> 40);
            data_out[out_offset + 14] = (uint8_t)(val >> 48);
            data_out[out_offset + 15] = (uint8_t)(val >> 56);
        } else {
            i = out_offset / sizeof(uint64_t);
            out64[i] = hca_regs->AES_OUT;
            out64[i + 1] = hca_regs->AES_OUT;
        }
#elif __riscv_xlen == 32
        if (!IS_ALIGNED_4_BYTES(data_out)) {
            register uint32_t val;
            val = hca_regs->AES_OUT;
            data_out[out_offset] = (uint8_t)val;
            data_out[out_offset + 1] = (uint8_t)(val >> 8);
            data_out[out_offset + 2] = (uint8_t)(val >> 16);
            data_out[out_offset + 3] = (uint8_t)(val >> 24);
            val = hca_regs->AES_OUT;
            data_out[out_offset + 4] = (uint8_t)val;
            data_out[out_offset + 5] = (uint8_t)(val >> 8);
            data_out[out_offset + 6] = (uint8_t)(val >> 16);
            data_out[out_offset + 7] = (uint8_t)(val >> 24);
            val = hca_regs->AES_OUT;
            data_out[out_offset + 8] = (uint8_t)val;
            data_out[out_offset + 9] = (uint8_t)(val >> 8);
            data_out[out_offset + 10] = (uint8_t)(val >> 16);
            data_out[out_offset + 11] = (uint8_t)(val >> 24);
            val = hca_regs->AES_OUT;
            data_out[out_offset + 12] = (uint8_t)val;
            data_out[out_offset + 13] = (uint8_t)(val >> 8);
            data_out[out_offset + 14] = (uint8_t)(val >> 16);
            data_out[out_offset + 15] = (uint8_t)(val >> 24);
        } else {
            i = out_offset / sizeof(uint32_t);
            out32[i] = hca_regs->AES_OUT;
            out32[i + 1] = hca_regs->AES_OUT;
            out32[i + 2] = hca_regs->AES_OUT;
            out32[i + 3] = hca_regs->AES_OUT;
        }
#endif /* __riscv_xlen */
        out_offset += BLOCK128_NB_BYTE;
    }

    in_offset += (k * BLOCK128_NB_BYTE);

    // sanity check
    if (in_offset > payload_len) {
        return METAL_CRYPTO_ERROR;
    }

    ctx->buf[0] = 0;
    ctx->buf[1] = 0;
    ctx->buf_len = payload_len - in_offset;

    // check rest
    if (in_offset < payload_len) {
        if (ctx->buf_len < sizeof(uint64_t)) {
            for (i = 0; i < ctx->buf_len; i++) {
                ctx->buf[1] +=
                    ((uint64_t)(*((const uint8_t *)(data_in + in_offset + i))))
                    << (i * CHAR_BIT);
            }
        } else {
            ctx->buf[1] = GET_64BITS(data_in, in_offset);
            in_offset += sizeof(uint64_t);

            if (ctx->buf_len > sizeof(uint64_t)) {
                for (i = 0; i < (ctx->buf_len - sizeof(uint64_t)); i++) {
                    ctx->buf[0] +=
                        ((uint64_t)(
                            *((const uint8_t *)(data_in + in_offset + i))))
                        << (i * CHAR_BIT);
                }
            }
        }
    }

    *len_out = out_offset;

    return METAL_CRYPTO_OK;
}

int32_t sifive_hca0_aes_auth_finish(struct sifive_hca0 hca,
                                    hca0_aes_auth_ctx_t *const ctx,
                                    uint8_t *const data_out,
                                    uint64_t *const tag) {
    volatile uint32_t reg32;
#if __riscv_xlen == 64
    uint64_t *in64 = (uint64_t *)(ctx->buf);
    uint64_t *out64 = (uint64_t *)(ctx->buf);
#elif __riscv_xlen == 32
    uint32_t *in32 = (uint32_t *)(ctx->buf);
    uint32_t *out32 = (uint32_t *)(ctx->buf);
#endif /* __riscv_xlen */
    uint8_t *tmp = (uint8_t *)(ctx->buf);
    size_t i;
    HCA_Type *hca_regs;

    if (get_index(hca) == METAL_HCA_INVALID_INDEX)
        return METAL_CRYPTO_ERROR;

    hca_regs = (HCA_Type *)dt_hca_data[get_index(hca)].hca_regs;

    if (0 == (hca_regs->AES_REV)) {
        // revision of AES is Zero so the AES is not present.
        return METAL_CRYPTO_ERROR;
    }

    if (0 != ctx->buf_len) {
        if (NULL == data_out) {
            return METAL_CRYPTO_INVALID_INPUT;
        }
#if __riscv_xlen == 64
        if (METAL_CRYPTO_LITTLE_ENDIAN_MODE == ctx->data_endianness) {
            hca_regs->FIFO_IN = in64[0];
            hca_regs->FIFO_IN = in64[1];
        } else {
            hca_regs->FIFO_IN = in64[1];
            hca_regs->FIFO_IN = in64[0];
        }
#elif __riscv_xlen == 32
        if (METAL_CRYPTO_LITTLE_ENDIAN_MODE == ctx->data_endianness) {
            hca_regs->FIFO_IN = in32[0];
            hca_regs->FIFO_IN = in32[1];
            hca_regs->FIFO_IN = in32[2];
            hca_regs->FIFO_IN = in32[3];
        } else {
            hca_regs->FIFO_IN = in32[2];
            hca_regs->FIFO_IN = in32[3];
            hca_regs->FIFO_IN = in32[0];
            hca_regs->FIFO_IN = in32[1];
        }
#endif /* __riscv_xlen */

        /* Wait for OFIFOEMPTY is cleared */
        while (hca_regs->CR & HCA_CR_OFIFOEMPTY_Msk) {
            __asm__ volatile("nop");
        }

        // use ctx->buf for the output result
        ctx->buf[0] = 0;
        ctx->buf[1] = 0;

        // Read output result
#if __riscv_xlen == 64
        if (METAL_CRYPTO_LITTLE_ENDIAN_MODE == ctx->data_endianness) {
            out64[1] = hca_regs->AES_OUT;
            out64[0] = hca_regs->AES_OUT;
        } else {
            out64[0] = hca_regs->AES_OUT;
            out64[1] = hca_regs->AES_OUT;
        }
#elif __riscv_xlen == 32
        if (METAL_CRYPTO_LITTLE_ENDIAN_MODE == ctx->data_endianness) {
            out32[2] = hca_regs->AES_OUT;
            out32[3] = hca_regs->AES_OUT;
            out32[0] = hca_regs->AES_OUT;
            out32[1] = hca_regs->AES_OUT;
        } else {
            out32[0] = hca_regs->AES_OUT;
            out32[1] = hca_regs->AES_OUT;
            out32[2] = hca_regs->AES_OUT;
            out32[3] = hca_regs->AES_OUT;
        }
#endif /* __riscv_xlen */

        // Copy result to output
        for (i = 0; i < ctx->buf_len; i++) {
            data_out[i] = tmp[i];
        }
    }

    /* Wait for AES BUSY is cleared */
    while (hca_regs->AES_CR & HCA_AES_CR_BUSY_Msk) {
        __asm__ volatile("nop");
    }

    // Get tag
    tag[0] = hca_regs->AES_AUTH[0];
    tag[1] = hca_regs->AES_AUTH[1];

    ctx->pld_len = 0;
    ctx->buf[0] = 0;
    ctx->buf[1] = 0;

    return METAL_CRYPTO_OK;
}

#else /* METAL_SIFIVE_HCA0 */

/* Stubs for when no HCA AES driver is present */
uint32_t sifive_hca0_aes_getrev(struct sifive_hca0 hca) { return 0; }

int32_t sifive_hca0_aes_setkey(struct sifive_hca0 hca,
                               metal_crypto_aes_key_type_t type,
                               const uint64_t *const key,
                               metal_crypto_process_t aes_process) {
    return METAL_CRYPTO_ERROR;
}

int32_t sifive_hca0_aes_setiv(struct sifive_hca0 hca,
                              const uint64_t *const iv) {
    return METAL_CRYPTO_ERROR;
}

int32_t sifive_hca0_aes_cipher(struct sifive_hca0 hca,
                               metal_crypto_aes_mode_t aes_mode,
                               metal_crypto_process_t aes_process,
                               metal_crypto_endianness_t data_endianness,
                               const uint8_t *const data_in, size_t data_len,
                               uint8_t *const data_out) {
    return METAL_CRYPTO_ERROR;
}

int32_t sifive_hca0_aes_auth_init(
    struct sifive_hca0 hca, hca0_aes_auth_ctx_t *const ctx,
    metal_crypto_aes_mode_t aes_mode, metal_crypto_process_t aes_process,
    metal_crypto_endianness_t data_endianness, uint32_t auth_option,
    const uint8_t *const aad, size_t aad_byte_len, size_t payload_len) {
    return METAL_CRYPTO_ERROR;
}

int32_t sifive_hca0_aes_auth_core(struct sifive_hca0 hca,
                                  hca0_aes_auth_ctx_t *const ctx,
                                  const uint8_t *const payload,
                                  size_t payload_len, uint8_t *const data_out,
                                  size_t *const out_len) {
    return METAL_CRYPTO_ERROR;
}

int32_t hca_aes_auth_finish(struct sifive_hca0 hca,
                            hca0_aes_auth_ctx_t *const ctx,
                            uint8_t *const data_out, uint64_t *const tag) {
    return METAL_CRYPTO_ERROR;
}

#endif /* METAL_SIFIVE_HCA0 */
