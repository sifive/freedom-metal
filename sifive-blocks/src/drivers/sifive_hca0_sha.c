/* Copyright 2020 SiFive, Inc */
/* SPDX-License-Identifier: Apache-2.0 */

#include <metal/platform.h>
#include <metal/crypto.h>
#include <metal/drivers/sifive_hca0.h>
#include <metal/drivers/sifive_hca0_sha.h>

#ifdef METAL_SIFIVE_HCA0

#include <metal/io.h>
#include <metal/private/metal_private_sifive_hca0.h>
#include <metal/private/metal_private_sifive_hca0_sha.h>
#include <string.h>
#include <limits.h>

#define get_index(hca) ((hca).__hca_index)

/*
 * sha xxx append_bit_len functions
 */
static void sifive_hca0_sha256_append_bit_len(uint8_t *const buffer, uint64_t *const length)
{
    size_t i;
    uint8_t *p_length = (uint8_t *)length;

    for (i = 0; i < sizeof(*length); i++)
    {
        buffer[SHA256_BYTE_SIZE_BLOCKSIZE - i - 1] = p_length[i];
    }
}

static void sifive_hca0_sha512_append_bit_len(uint8_t *const buffer, uint64_t *const length)
{
    size_t i;
    uint8_t *p_length = (uint8_t *)length;

    for (i = 0; i < sizeof(*length); i++)
    {
        buffer[SHA512_BYTE_SIZE_BLOCKSIZE - i - 1] = p_length[i];
    }
}

/*
 * sha block function
 */
static int32_t hca_sha_block(struct sifive_hca0 hca, metal_crypto_hash_mode_t hash_mode,
                      size_t NbBlocks512, const uint8_t *const data_in)
{
    volatile uint32_t reg32;
    size_t i;
    HCA_Type *hca_regs;

    if (get_index(hca) == METAL_HCA_INVALID_INDEX)
        return METAL_CRYPTO_ERROR;

    hca_regs = (HCA_Type *)dt_hca_data[get_index(hca)].hca_regs;

    if ( 0 == (hca_regs->SHA_REV) )
    {
        /* revision of SHA is Zero so the SHA is not present. */
        return METAL_CRYPTO_ERROR;
    }

    if (NbBlocks512 == 0)
    {
        return METAL_CRYPTO_INVALID_INPUT;
    }

    if ((NbBlocks512 & 0x1) && (hash_mode >= METAL_CRYPTO_HASH_SHA384))
    {
        /* nb block should be even to have 1024bits */
        return METAL_CRYPTO_INVALID_INPUT;
    }

    for (size_t k = 0; k < NbBlocks512; k++)
    {
        /* Put data in the FIFO */
        /* Wait for IFIFOEMPTY is cleared */
        while ( hca_regs->CR & HCA_CR_IFIFOFULL_Msk )
        {
            __asm__ volatile("nop");
        }
#if __riscv_xlen == 64
        if ((uint64_t)data_in & 0x7)
        {
            i = k << 6;
            hca_regs->FIFO_IN = GET_64BITS(data_in, i);
            hca_regs->FIFO_IN = GET_64BITS(data_in, (i + 8));
            hca_regs->FIFO_IN = GET_64BITS(data_in, (i + 16));
            hca_regs->FIFO_IN = GET_64BITS(data_in, (i + 24));
            hca_regs->FIFO_IN = GET_64BITS(data_in, (i + 32));
            hca_regs->FIFO_IN = GET_64BITS(data_in, (i + 40));
            hca_regs->FIFO_IN = GET_64BITS(data_in, (i + 48));
            hca_regs->FIFO_IN = GET_64BITS(data_in, (i + 56));
        }
        else
        {
            #pragma GCC diagnostic push
            /* data_in is known to be aligned on uint64_t */
            #pragma GCC diagnostic ignored "-Wcast-align"
            const uint64_t *in64 = (const uint64_t *)data_in;
            #pragma GCC diagnostic pop
            i = k << 3;
            hca_regs->FIFO_IN = in64[i];
            hca_regs->FIFO_IN = in64[i + 1];
            hca_regs->FIFO_IN = in64[i + 2];
            hca_regs->FIFO_IN = in64[i + 3];
            hca_regs->FIFO_IN = in64[i + 4];
            hca_regs->FIFO_IN = in64[i + 5];
            hca_regs->FIFO_IN = in64[i + 6];
            hca_regs->FIFO_IN = in64[i + 7];
        }
#elif __riscv_xlen == 32
        if ((uint32_t)data_in & 0x3)
        {
            i = k << 6;
            hca_regs->FIFO_IN = GET_32BITS(data_in, i);
            hca_regs->FIFO_IN = GET_32BITS(data_in, (i + 4));
            hca_regs->FIFO_IN = GET_32BITS(data_in, (i + 8));
            hca_regs->FIFO_IN = GET_32BITS(data_in, (i + 12));
            hca_regs->FIFO_IN = GET_32BITS(data_in, (i + 16));
            hca_regs->FIFO_IN = GET_32BITS(data_in, (i + 20));
            hca_regs->FIFO_IN = GET_32BITS(data_in, (i + 24));
            hca_regs->FIFO_IN = GET_32BITS(data_in, (i + 28));
            hca_regs->FIFO_IN = GET_32BITS(data_in, (i + 32));
            hca_regs->FIFO_IN = GET_32BITS(data_in, (i + 36));
            hca_regs->FIFO_IN = GET_32BITS(data_in, (i + 40));
            hca_regs->FIFO_IN = GET_32BITS(data_in, (i + 44));
            hca_regs->FIFO_IN = GET_32BITS(data_in, (i + 48));
            hca_regs->FIFO_IN = GET_32BITS(data_in, (i + 52));
            hca_regs->FIFO_IN = GET_32BITS(data_in, (i + 56));
            hca_regs->FIFO_IN = GET_32BITS(data_in, (i + 60));
        }
        else
        {
            #pragma GCC diagnostic push
            /* data_in is known to be aligned on uint32_t */
            #pragma GCC diagnostic ignored "-Wcast-align"
            const uint32_t *in32 = (const uint32_t *)data_in;
            #pragma GCC diagnostic pop
            i = k << 4;
            hca_regs->FIFO_IN = in32[i];
            hca_regs->FIFO_IN = in32[i + 1];
            hca_regs->FIFO_IN = in32[i + 2];
            hca_regs->FIFO_IN = in32[i + 3];
            hca_regs->FIFO_IN = in32[i + 4];
            hca_regs->FIFO_IN = in32[i + 5];
            hca_regs->FIFO_IN = in32[i + 6];
            hca_regs->FIFO_IN = in32[i + 7];
            hca_regs->FIFO_IN = in32[i + 8];
            hca_regs->FIFO_IN = in32[i + 9];
            hca_regs->FIFO_IN = in32[i + 10];
            hca_regs->FIFO_IN = in32[i + 11];
            hca_regs->FIFO_IN = in32[i + 12];
            hca_regs->FIFO_IN = in32[i + 13];
            hca_regs->FIFO_IN = in32[i + 14];
            hca_regs->FIFO_IN = in32[i + 15];
        }
#endif

        if (hash_mode >= METAL_CRYPTO_HASH_SHA384)
        {
            /* Need to have 1024bits before SHA end performing. */
            if (k & 0x1)
            {
                /* Wait for SHA BUSY is cleared */
                while ( hca_regs->SHA_CR & HCA_SHA_CR_BUSY_Msk )
                {
                    __asm__ volatile("nop");
                }
            }
        }
        else
        {
            /* Wait for SHA BUSY is cleared */
            while ( hca_regs->SHA_CR & HCA_SHA_CR_BUSY_Msk )
            {
                __asm__ volatile("nop");
            }
        }
    }
    return METAL_CRYPTO_OK;
}

/*
 * sha xxx core function
 */
static int32_t sifive_hca0_sha256_core(struct sifive_hca0 hca, hca0_sha_ctx_t *const ctx,
                        const uint8_t *const data, size_t data_byte_len)
{
    size_t block_buffer_index;
    size_t block_remain;
    size_t nb_blocks;
    size_t data_index = 0;
    int32_t result;

    if (NULL == ctx || NULL == data)
    {
        return (METAL_CRYPTO_INVALID_INPUT);
    }

    /* currently used nb of bytes in the block buffer */
    block_buffer_index =
        (size_t)((ctx->ctx.sha256.bitlen >> 3) % SHA256_BYTE_BLOCKSIZE);

    /* compute the free remaining space in the block buffer (64-byte long) */
    block_remain = SHA256_BYTE_BLOCKSIZE - block_buffer_index;

    ctx->ctx.sha256.bitlen += (data_byte_len << 3);

    /*
     * if the input data size is larger than the block remaining size we'll be
     * able to process at least one block
     */
    if (data_byte_len >= block_remain)
    {
        /*
         * we can add data, starting at the first available position in the
         * block buffer
         */
        memcpy(&ctx->ctx.sha256.block_buffer[block_buffer_index], data,
               block_remain);

        /* this block is now complete,so it can be processed */
        result = hca_sha_block(hca, ctx->mode, 1, ctx->ctx.sha256.block_buffer);
        if (METAL_CRYPTO_OK != result)
        {
            return (result);
        }

        /* block has been fully processed,so block buffer is empty */
        block_buffer_index = 0;

        nb_blocks = (data_byte_len - block_remain) / SHA256_BYTE_BLOCKSIZE;

        if(0 != nb_blocks) {
            /* processing full blocks as long as data are available */
            result = hca_sha_block(hca, ctx->mode, nb_blocks, &data[block_remain]);
            if (METAL_CRYPTO_OK != result)
            {
                return (result);
            }
        }
        data_index = (nb_blocks * SHA256_BYTE_BLOCKSIZE) + block_remain;
    }

    /* copying the remaining 'data' bytes to the block buffer */
    memcpy(&ctx->ctx.sha256.block_buffer[block_buffer_index], &data[data_index],
           data_byte_len - data_index);

    return (METAL_CRYPTO_OK);
}

static int32_t sifive_hca0_sha512_core(struct sifive_hca0 hca, hca0_sha_ctx_t *const ctx,
                        const uint8_t *const data, size_t data_byte_len)
{
    size_t block_buffer_index;
    size_t block_remain;
    size_t nb_blocks;
    size_t data_index = 0;
    int32_t result;

    if (NULL == ctx || NULL == data)
    {
        return (METAL_CRYPTO_INVALID_INPUT);
    }

    /* currently used nb of bytes in the block buffer */
    block_buffer_index =
        (size_t)((ctx->ctx.sha512.bitlen >> 3) % SHA512_BYTE_BLOCKSIZE);

    /* compute the free remaining space in the block buffer (64-byte long) */
    block_remain = SHA512_BYTE_BLOCKSIZE - block_buffer_index;

    ctx->ctx.sha512.bitlen += (data_byte_len << 3);

    /*
     * if the input data size is larger than the block remaining size we'll be
     * able to process at least one block
     */
    if (data_byte_len >= block_remain)
    {
        /*
         * we can add data,starting at the first available position in the block
         * buffer
         */
        memcpy(&ctx->ctx.sha512.block_buffer[block_buffer_index], data,
               block_remain);

        /* this block is now complete,so it can be processed */
        result = hca_sha_block(hca, ctx->mode, 2, ctx->ctx.sha512.block_buffer);
        if (METAL_CRYPTO_OK != result)
        {
            return (result);
        }

        /* block has been fully processed,so block buffer is empty */
        block_buffer_index = 0;

        /**
         * Compute the number of 512 bits block (sha256 has 512 bits block),
         * sha 384 and 512 have an 1024, which result in an even number of block
        */
        nb_blocks = ((data_byte_len - block_remain) / SHA512_BYTE_BLOCKSIZE) * 2;

        if(0 != nb_blocks) {
            /* processing full blocks as long as data are available */
            result = hca_sha_block(hca, ctx->mode, nb_blocks, &data[block_remain]);
            if (METAL_CRYPTO_OK != result)
            {
                return (result);
            }
        }
        data_index = (nb_blocks * SHA256_BYTE_BLOCKSIZE) + block_remain;
    }

    /* copying the remaining 'data' bytes to the block buffer */
    memcpy(&ctx->ctx.sha512.block_buffer[block_buffer_index], &data[data_index],
           data_byte_len - data_index);

    return (METAL_CRYPTO_OK);
}

/*
 * sha xxx finish function
 */
static int32_t sifive_hca0_sha224_finish(struct sifive_hca0 hca, hca0_sha_ctx_t *const ctx,
                          uint8_t *const hash, size_t *const hash_len)
{
    volatile uint64_t val;
    size_t block_buffer_index;
    size_t block_remain;
    int32_t result;
    HCA_Type *hca_regs;

    if (get_index(hca) == METAL_HCA_INVALID_INDEX)
        return (METAL_CRYPTO_ERROR);

    hca_regs = (HCA_Type *)dt_hca_data[get_index(hca)].hca_regs;

    if ((NULL == hash) || (NULL == hash_len))
    {
        return (METAL_CRYPTO_INVALID_OUTPUT);
    }

    if (NULL == ctx)
    {
        return (METAL_CRYPTO_INVALID_INPUT);
    }

    if (*hash_len < METAL_CRYPTO_HASH_SHA224_BYTE_SIZE)
    {
        return (METAL_CRYPTO_INVALID_OUTPUT);
    }

    /* currently used nb of bytes in the block buffer */
    block_buffer_index =
        (size_t)((ctx->ctx.sha256.bitlen >> 3) % SHA256_BYTE_BLOCKSIZE);

    /* add end of message */
    ctx->ctx.sha256.block_buffer[block_buffer_index] = 0x80;

    block_buffer_index++;

    /* compute the free remaining space in the block buffer (64-byte long) */
    block_remain = SHA256_BYTE_BLOCKSIZE - block_buffer_index;

    if (block_remain >= SHA256_BYTE_SIZE_BLOCKSIZE)
    {
        memset(&ctx->ctx.sha256.block_buffer[block_buffer_index], 0,
               block_remain);
        block_buffer_index += block_remain - SHA256_BYTE_SIZE_BLOCKSIZE;
        sifive_hca0_sha256_append_bit_len(
            &ctx->ctx.sha256.block_buffer[block_buffer_index],
            &ctx->ctx.sha256.bitlen);
        /* this block is now complete,so it can be processed */
        result = hca_sha_block(hca, ctx->mode, 1, ctx->ctx.sha256.block_buffer);
        if (METAL_CRYPTO_OK != result)
        {
            return (result);
        }
    }
    else
    {
        if (block_remain != 0)
        {
            memset(&ctx->ctx.sha256.block_buffer[block_buffer_index], 0,
                   block_remain);
        }
        block_buffer_index = 0;
        block_remain = SHA256_BYTE_BLOCKSIZE;
        /* this block is now complete,so it can be processed */
        result = hca_sha_block(hca, ctx->mode, 1, ctx->ctx.sha256.block_buffer);
        if (METAL_CRYPTO_OK != result)
        {
            return (result);
        }

        memset(&ctx->ctx.sha256.block_buffer[block_buffer_index], 0,
               block_remain);

        block_buffer_index += block_remain - SHA256_BYTE_SIZE_BLOCKSIZE;
        sifive_hca0_sha256_append_bit_len(
            &ctx->ctx.sha256.block_buffer[block_buffer_index],
            &ctx->ctx.sha256.bitlen);
        /* this block is now complete,so it can be processed */
        result = hca_sha_block(hca, ctx->mode, 1, ctx->ctx.sha256.block_buffer);
        if (METAL_CRYPTO_OK != result)
        {
            return (result);
        }
    }

    /* retrieving the hash result */
    if ( ! IS_ALIGNED_4_BYTES(hash) )
    {
        val = hca_regs->HASH[0];
        hash[27] = (uint8_t)val;
        hash[26] = (uint8_t)(val >> 8);
        hash[25] = (uint8_t)(val >> 16);
        hash[24] = (uint8_t)(val >> 24);
        hash[23] = (uint8_t)(val >> 32);
        hash[22] = (uint8_t)(val >> 40);
        hash[21] = (uint8_t)(val >> 48);
        hash[20] = (uint8_t)(val >> 56);
        val = hca_regs->HASH[1];
        hash[19] = (uint8_t)val;
        hash[18] = (uint8_t)(val >> 8);
        hash[17] = (uint8_t)(val >> 16);
        hash[16] = (uint8_t)(val >> 24);
        hash[15] = (uint8_t)(val >> 32);
        hash[14] = (uint8_t)(val >> 40);
        hash[13] = (uint8_t)(val >> 48);
        hash[12] = (uint8_t)(val >> 56);
        val = hca_regs->HASH[2];
        hash[11] = (uint8_t)val;
        hash[10] = (uint8_t)(val >> 8);
        hash[9] = (uint8_t)(val >> 16);
        hash[8] = (uint8_t)(val >> 24);
        hash[7] = (uint8_t)(val >> 32);
        hash[6] = (uint8_t)(val >> 40);
        hash[5] = (uint8_t)(val >> 48);
        hash[4] = (uint8_t)(val >> 56);
        val = hca_regs->HASH[3];
        hash[3] = (uint8_t)val;
        hash[2] = (uint8_t)(val >> 8);
        hash[1] = (uint8_t)(val >> 16);
        hash[0] = (uint8_t)(val >> 24);
    }
    else
    {
        #pragma GCC diagnostic push
        /* hash is known to be aligned on uint32_t */
        #pragma GCC diagnostic ignored "-Wcast-align"
        uint32_t *out32 = (uint32_t *)hash;
        #pragma GCC diagnostic pop

        val = hca_regs->HASH[3];
        out32[0] = bswap32((uint32_t)val);
        val = bswap64(hca_regs->HASH[2]);
        out32[1] = (uint32_t)val;
        out32[2] = (uint32_t)(val >> 32);
        val = bswap64(hca_regs->HASH[1]);
        out32[3] = (uint32_t)val;
        out32[4] = (uint32_t)(val >> 32);
        val = bswap64(hca_regs->HASH[0]);
        out32[5] = (uint32_t)val;
        out32[6] = (uint32_t)(val >> 32);
    }

    *hash_len = METAL_CRYPTO_HASH_SHA224_BYTE_SIZE;

    return (METAL_CRYPTO_OK);
}

static int32_t sifive_hca0_sha256_finish(struct sifive_hca0 hca, hca0_sha_ctx_t *const ctx,
                          uint8_t *const hash, size_t *hash_len)
{
    size_t block_buffer_index;
    size_t block_remain;
    int32_t result;
    HCA_Type *hca_regs;

    if (get_index(hca) == METAL_HCA_INVALID_INDEX)
        return (METAL_CRYPTO_ERROR);

    hca_regs = (HCA_Type *)dt_hca_data[get_index(hca)].hca_regs;

    if ((NULL == hash) || (NULL == hash_len))
    {
        return (METAL_CRYPTO_INVALID_OUTPUT);
    }

    if (NULL == ctx)
    {
        return (METAL_CRYPTO_INVALID_INPUT);
    }

    if (*hash_len < METAL_CRYPTO_HASH_SHA256_BYTE_SIZE)
    {
        return (METAL_CRYPTO_INVALID_OUTPUT);
    }

    /* currently used nb of bytes in the block buffer */
    block_buffer_index =
        (size_t)((ctx->ctx.sha256.bitlen >> 3) % SHA256_BYTE_BLOCKSIZE);

    /* add end of message */
    ctx->ctx.sha256.block_buffer[block_buffer_index] = 0x80;

    block_buffer_index++;

    /* compute the free remaining space in the block buffer (64-byte long) */
    block_remain = SHA256_BYTE_BLOCKSIZE - block_buffer_index;

    if (block_remain >= SHA256_BYTE_SIZE_BLOCKSIZE)
    {
        memset(&ctx->ctx.sha256.block_buffer[block_buffer_index], 0,
               block_remain);
        block_buffer_index += block_remain - SHA256_BYTE_SIZE_BLOCKSIZE;
        sifive_hca0_sha256_append_bit_len(
            &ctx->ctx.sha256.block_buffer[block_buffer_index],
            &ctx->ctx.sha256.bitlen);
        /* this block is now complete,so it can be processed */
        result = hca_sha_block(hca, ctx->mode, 1, ctx->ctx.sha256.block_buffer);
        if (METAL_CRYPTO_OK != result)
        {
            return (result);
        }
    }
    else
    {
        if (block_remain != 0)
        {
            memset(&ctx->ctx.sha256.block_buffer[block_buffer_index], 0,
                   block_remain);
        }
        block_buffer_index = 0;
        block_remain = SHA256_BYTE_BLOCKSIZE;
        /* this block is now complete,so it can be processed */
        result = hca_sha_block(hca, ctx->mode, 1, ctx->ctx.sha256.block_buffer);
        if (METAL_CRYPTO_OK != result)
        {
            return (result);
        }

        memset(&ctx->ctx.sha256.block_buffer[block_buffer_index], 0,
               block_remain);

        block_buffer_index += block_remain - SHA256_BYTE_SIZE_BLOCKSIZE;
        sifive_hca0_sha256_append_bit_len(
            &ctx->ctx.sha256.block_buffer[block_buffer_index],
            &ctx->ctx.sha256.bitlen);
        /* this block is now complete,so it can be processed */
        result = hca_sha_block(hca, ctx->mode, 1, ctx->ctx.sha256.block_buffer);
        if (METAL_CRYPTO_OK != result)
        {
            return (result);
        }
    }

    /* retrieving the hash result */
    if ( ! IS_ALIGNED_8_BYTES(hash) )
    {
        volatile uint64_t val;
        val = hca_regs->HASH[0];
        hash[31] = (uint8_t)val;
        hash[30] = (uint8_t)(val >> 8);
        hash[29] = (uint8_t)(val >> 16);
        hash[28] = (uint8_t)(val >> 24);
        hash[27] = (uint8_t)(val >> 32);
        hash[26] = (uint8_t)(val >> 40);
        hash[25] = (uint8_t)(val >> 48);
        hash[24] = (uint8_t)(val >> 56);
        val = hca_regs->HASH[1];
        hash[23] = (uint8_t)val;
        hash[22] = (uint8_t)(val >> 8);
        hash[21] = (uint8_t)(val >> 16);
        hash[20] = (uint8_t)(val >> 24);
        hash[19] = (uint8_t)(val >> 32);
        hash[18] = (uint8_t)(val >> 40);
        hash[17] = (uint8_t)(val >> 48);
        hash[16] = (uint8_t)(val >> 56);
        val = hca_regs->HASH[2];
        hash[15] = (uint8_t)val;
        hash[14] = (uint8_t)(val >> 8);
        hash[13] = (uint8_t)(val >> 16);
        hash[12] = (uint8_t)(val >> 24);
        hash[11] = (uint8_t)(val >> 32);
        hash[10] = (uint8_t)(val >> 40);
        hash[9] = (uint8_t)(val >> 48);
        hash[8] = (uint8_t)(val >> 56);
        val = hca_regs->HASH[3];
        hash[7] = (uint8_t)val;
        hash[6] = (uint8_t)(val >> 8);
        hash[5] = (uint8_t)(val >> 16);
        hash[4] = (uint8_t)(val >> 24);
        hash[3] = (uint8_t)(val >> 32);
        hash[2] = (uint8_t)(val >> 40);
        hash[1] = (uint8_t)(val >> 48);
        hash[0] = (uint8_t)(val >> 56);
    }
    else
    {
        #pragma GCC diagnostic push
        /*  hash is known to be aligned on uint64_t */
        #pragma GCC diagnostic ignored "-Wcast-align"
        uint64_t *out64 = (uint64_t *)hash;
        #pragma GCC diagnostic pop
        out64[0] = bswap64(hca_regs->HASH[3]);
        out64[1] = bswap64(hca_regs->HASH[2]);
        out64[2] = bswap64(hca_regs->HASH[1]);
        out64[3] = bswap64(hca_regs->HASH[0]);
    }

    *hash_len = METAL_CRYPTO_HASH_SHA256_BYTE_SIZE;

    return (METAL_CRYPTO_OK);
}

static int32_t sifive_hca0_sha384_finish(struct sifive_hca0 hca, hca0_sha_ctx_t *const ctx,
                          uint8_t *const hash, size_t *const hash_len)
{
    size_t block_buffer_index;
    size_t block_remain;
    int32_t result;
    HCA_Type *hca_regs;

    if (get_index(hca) == METAL_HCA_INVALID_INDEX)
        return (METAL_CRYPTO_ERROR);

    hca_regs = (HCA_Type *)dt_hca_data[get_index(hca)].hca_regs;

    if ((NULL == hash) || (NULL == hash_len))
    {
        return (METAL_CRYPTO_INVALID_OUTPUT);
    }

    if (NULL == ctx)
    {
        return (METAL_CRYPTO_INVALID_INPUT);
    }

    if (*hash_len < METAL_CRYPTO_HASH_SHA384_BYTE_SIZE)
    {
        return (METAL_CRYPTO_INVALID_OUTPUT);
    }

    /* currently used nb of bytes in the block buffer */
    block_buffer_index =
        (size_t)((ctx->ctx.sha512.bitlen >> 3) % SHA512_BYTE_BLOCKSIZE);

    /* add end of message */
    ctx->ctx.sha512.block_buffer[block_buffer_index] = 0x80;

    block_buffer_index++;

    /* compute the free remaining space in the block buffer (64-byte long) */
    block_remain = SHA512_BYTE_BLOCKSIZE - block_buffer_index;

    if (block_remain >= SHA512_BYTE_SIZE_BLOCKSIZE)
    {
        memset(&ctx->ctx.sha512.block_buffer[block_buffer_index], 0,
               block_remain);
        block_buffer_index += block_remain - SHA512_BYTE_SIZE_BLOCKSIZE;
        sifive_hca0_sha512_append_bit_len(
            &ctx->ctx.sha512.block_buffer[block_buffer_index],
            &ctx->ctx.sha512.bitlen);
        /* this block is now complete,so it can be processed */
        result = hca_sha_block(hca, ctx->mode, 2, ctx->ctx.sha512.block_buffer);
        if (METAL_CRYPTO_OK != result)
        {
            return (result);
        }
    }
    else
    {
        if (block_remain != 0)
        {
            memset(&ctx->ctx.sha512.block_buffer[block_buffer_index], 0,
                   block_remain);
        }
        block_buffer_index = 0;
        block_remain = SHA512_BYTE_BLOCKSIZE;
        /* this block is now complete,so it can be processed */
        result = hca_sha_block(hca, ctx->mode, 2, ctx->ctx.sha512.block_buffer);
        if (METAL_CRYPTO_OK != result)
        {
            return (result);
        }

        memset(&ctx->ctx.sha512.block_buffer[block_buffer_index], 0,
               block_remain);

        block_buffer_index += block_remain - SHA512_BYTE_SIZE_BLOCKSIZE;
        sifive_hca0_sha512_append_bit_len(
            &ctx->ctx.sha512.block_buffer[block_buffer_index],
            &ctx->ctx.sha512.bitlen);
        /* this block is now complete,so it can be processed */
        result = hca_sha_block(hca, ctx->mode, 2, ctx->ctx.sha512.block_buffer);
        if (METAL_CRYPTO_OK != result)
        {
            return (result);
        }
    }

    /* retrieving the hash result */
    if ( ! IS_ALIGNED_8_BYTES(hash) )
    {
        volatile uint64_t val;
        val = hca_regs->HASH[0];
        hash[47] = (uint8_t)val;
        hash[46] = (uint8_t)(val >> 8);
        hash[45] = (uint8_t)(val >> 16);
        hash[44] = (uint8_t)(val >> 24);
        hash[43] = (uint8_t)(val >> 32);
        hash[42] = (uint8_t)(val >> 40);
        hash[41] = (uint8_t)(val >> 48);
        hash[40] = (uint8_t)(val >> 56);
        val = hca_regs->HASH[1];
        hash[39] = (uint8_t)val;
        hash[38] = (uint8_t)(val >> 8);
        hash[37] = (uint8_t)(val >> 16);
        hash[36] = (uint8_t)(val >> 24);
        hash[35] = (uint8_t)(val >> 32);
        hash[34] = (uint8_t)(val >> 40);
        hash[33] = (uint8_t)(val >> 48);
        hash[32] = (uint8_t)(val >> 56);
        val = hca_regs->HASH[2];
        hash[31] = (uint8_t)val;
        hash[30] = (uint8_t)(val >> 8);
        hash[29] = (uint8_t)(val >> 16);
        hash[28] = (uint8_t)(val >> 24);
        hash[27] = (uint8_t)(val >> 32);
        hash[26] = (uint8_t)(val >> 40);
        hash[25] = (uint8_t)(val >> 48);
        hash[24] = (uint8_t)(val >> 56);
        val = hca_regs->HASH[3];
        hash[23] = (uint8_t)val;
        hash[22] = (uint8_t)(val >> 8);
        hash[21] = (uint8_t)(val >> 16);
        hash[20] = (uint8_t)(val >> 24);
        hash[19] = (uint8_t)(val >> 32);
        hash[18] = (uint8_t)(val >> 40);
        hash[17] = (uint8_t)(val >> 48);
        hash[16] = (uint8_t)(val >> 56);
        val = hca_regs->HASH[4];
        hash[15] = (uint8_t)val;
        hash[14] = (uint8_t)(val >> 8);
        hash[13] = (uint8_t)(val >> 16);
        hash[12] = (uint8_t)(val >> 24);
        hash[11] = (uint8_t)(val >> 32);
        hash[10] = (uint8_t)(val >> 40);
        hash[9] = (uint8_t)(val >> 48);
        hash[8] = (uint8_t)(val >> 56);
        val = hca_regs->HASH[5];
        hash[7] = (uint8_t)val;
        hash[6] = (uint8_t)(val >> 8);
        hash[5] = (uint8_t)(val >> 16);
        hash[4] = (uint8_t)(val >> 24);
        hash[3] = (uint8_t)(val >> 32);
        hash[2] = (uint8_t)(val >> 40);
        hash[1] = (uint8_t)(val >> 48);
        hash[0] = (uint8_t)(val >> 56);
    }
    else
    {
        #pragma GCC diagnostic push
        /* hash is known to be aligned on uint64_t */
        #pragma GCC diagnostic ignored "-Wcast-align"
        uint64_t *out64 = (uint64_t *)hash;
        #pragma GCC diagnostic pop
        out64[0] = bswap64(hca_regs->HASH[5]);
        out64[1] = bswap64(hca_regs->HASH[4]);
        out64[2] = bswap64(hca_regs->HASH[3]);
        out64[3] = bswap64(hca_regs->HASH[2]);
        out64[4] = bswap64(hca_regs->HASH[1]);
        out64[5] = bswap64(hca_regs->HASH[0]);
    }

    *hash_len = METAL_CRYPTO_HASH_SHA384_BYTE_SIZE;

    return (METAL_CRYPTO_OK);
}

static int32_t sifive_hca0_sha512_finish(struct sifive_hca0 hca, hca0_sha_ctx_t *const ctx,
                          uint8_t *const hash, size_t *hash_len)
{
    size_t block_buffer_index;
    size_t block_remain;
    int32_t result;
    HCA_Type *hca_regs;

    if (get_index(hca) == METAL_HCA_INVALID_INDEX)
        return (METAL_CRYPTO_ERROR);

    hca_regs = (HCA_Type *)dt_hca_data[get_index(hca)].hca_regs;

    if ((NULL == hash) || (NULL == hash_len))
    {
        return (METAL_CRYPTO_INVALID_OUTPUT);
    }

    if (NULL == ctx)
    {
        return (METAL_CRYPTO_INVALID_INPUT);
    }

    if (*hash_len < METAL_CRYPTO_HASH_SHA512_BYTE_SIZE)
    {
        return (METAL_CRYPTO_INVALID_OUTPUT);
    }

    /* currently used nb of bytes in the block buffer */
    block_buffer_index =
        (size_t)((ctx->ctx.sha512.bitlen >> 3) % SHA512_BYTE_BLOCKSIZE);

    /* add end of message */
    ctx->ctx.sha512.block_buffer[block_buffer_index] = 0x80;

    block_buffer_index++;

    /* compute the free remaining space in the block buffer (64-byte long) */
    block_remain = SHA512_BYTE_BLOCKSIZE - block_buffer_index;

    if (block_remain >= SHA512_BYTE_SIZE_BLOCKSIZE)
    {
        memset(&ctx->ctx.sha512.block_buffer[block_buffer_index], 0,
               block_remain);
        block_buffer_index += block_remain - SHA512_BYTE_SIZE_BLOCKSIZE;
        sifive_hca0_sha512_append_bit_len(
            &ctx->ctx.sha512.block_buffer[block_buffer_index],
            &ctx->ctx.sha512.bitlen);
        /* this block is now complete,so it can be processed */
        result = hca_sha_block(hca, ctx->mode, 2, ctx->ctx.sha512.block_buffer);
        if (METAL_CRYPTO_OK != result)
        {
            return (result);
        }
    }
    else
    {
        if (block_remain != 0)
        {
            memset(&ctx->ctx.sha512.block_buffer[block_buffer_index], 0,
                   block_remain);
        }
        block_buffer_index = 0;
        block_remain = SHA512_BYTE_BLOCKSIZE;
        /* this block is now complete,so it can be processed */
        result = hca_sha_block(hca, ctx->mode, 2, ctx->ctx.sha512.block_buffer);
        if (METAL_CRYPTO_OK != result)
        {
            return (result);
        }

        memset(&ctx->ctx.sha512.block_buffer[block_buffer_index], 0,
               block_remain);

        block_buffer_index += block_remain - SHA512_BYTE_SIZE_BLOCKSIZE;
        sifive_hca0_sha512_append_bit_len(
            &ctx->ctx.sha512.block_buffer[block_buffer_index],
            &ctx->ctx.sha512.bitlen);
        /* this block is now complete,so it can be processed */
        result = hca_sha_block(hca, ctx->mode, 2, ctx->ctx.sha512.block_buffer);
        if (METAL_CRYPTO_OK != result)
        {
            return (result);
        }
    }

    /* retrieving the hash result */
    if ( ! IS_ALIGNED_8_BYTES(hash) )
    {
        volatile uint64_t val;
        val = hca_regs->HASH[0];
        hash[63] = (uint8_t)val;
        hash[62] = (uint8_t)(val >> 8);
        hash[61] = (uint8_t)(val >> 16);
        hash[60] = (uint8_t)(val >> 24);
        hash[59] = (uint8_t)(val >> 32);
        hash[58] = (uint8_t)(val >> 40);
        hash[57] = (uint8_t)(val >> 48);
        hash[56] = (uint8_t)(val >> 56);
        val = hca_regs->HASH[1];
        hash[55] = (uint8_t)val;
        hash[54] = (uint8_t)(val >> 8);
        hash[53] = (uint8_t)(val >> 16);
        hash[52] = (uint8_t)(val >> 24);
        hash[51] = (uint8_t)(val >> 32);
        hash[50] = (uint8_t)(val >> 40);
        hash[49] = (uint8_t)(val >> 48);
        hash[48] = (uint8_t)(val >> 56);
        val = hca_regs->HASH[2];
        hash[47] = (uint8_t)val;
        hash[46] = (uint8_t)(val >> 8);
        hash[45] = (uint8_t)(val >> 16);
        hash[44] = (uint8_t)(val >> 24);
        hash[43] = (uint8_t)(val >> 32);
        hash[42] = (uint8_t)(val >> 40);
        hash[41] = (uint8_t)(val >> 48);
        hash[40] = (uint8_t)(val >> 56);
        val = hca_regs->HASH[3];
        hash[39] = (uint8_t)val;
        hash[38] = (uint8_t)(val >> 8);
        hash[37] = (uint8_t)(val >> 16);
        hash[36] = (uint8_t)(val >> 24);
        hash[35] = (uint8_t)(val >> 32);
        hash[34] = (uint8_t)(val >> 40);
        hash[33] = (uint8_t)(val >> 48);
        hash[32] = (uint8_t)(val >> 56);
        val = hca_regs->HASH[4];
        hash[31] = (uint8_t)val;
        hash[30] = (uint8_t)(val >> 8);
        hash[29] = (uint8_t)(val >> 16);
        hash[28] = (uint8_t)(val >> 24);
        hash[27] = (uint8_t)(val >> 32);
        hash[26] = (uint8_t)(val >> 40);
        hash[25] = (uint8_t)(val >> 48);
        hash[24] = (uint8_t)(val >> 56);
        val = hca_regs->HASH[5];
        hash[23] = (uint8_t)val;
        hash[22] = (uint8_t)(val >> 8);
        hash[21] = (uint8_t)(val >> 16);
        hash[20] = (uint8_t)(val >> 24);
        hash[19] = (uint8_t)(val >> 32);
        hash[18] = (uint8_t)(val >> 40);
        hash[17] = (uint8_t)(val >> 48);
        hash[16] = (uint8_t)(val >> 56);
        val = hca_regs->HASH[6];
        hash[15] = (uint8_t)val;
        hash[14] = (uint8_t)(val >> 8);
        hash[13] = (uint8_t)(val >> 16);
        hash[12] = (uint8_t)(val >> 24);
        hash[11] = (uint8_t)(val >> 32);
        hash[10] = (uint8_t)(val >> 40);
        hash[9] = (uint8_t)(val >> 48);
        hash[8] = (uint8_t)(val >> 56);
        val = hca_regs->HASH[7];
        hash[7] = (uint8_t)val;
        hash[6] = (uint8_t)(val >> 8);
        hash[5] = (uint8_t)(val >> 16);
        hash[4] = (uint8_t)(val >> 24);
        hash[3] = (uint8_t)(val >> 32);
        hash[2] = (uint8_t)(val >> 40);
        hash[1] = (uint8_t)(val >> 48);
        hash[0] = (uint8_t)(val >> 56);
    }
    else
    {
        #pragma GCC diagnostic push
        /* hash is known to be aligned on uint64_t */
        #pragma GCC diagnostic ignored "-Wcast-align"
        uint64_t *out64 = (uint64_t *)hash;
        #pragma GCC diagnostic pop
        out64[0] = bswap64(hca_regs->HASH[7]);
        out64[1] = bswap64(hca_regs->HASH[6]);
        out64[2] = bswap64(hca_regs->HASH[5]);
        out64[3] = bswap64(hca_regs->HASH[4]);
        out64[4] = bswap64(hca_regs->HASH[3]);
        out64[5] = bswap64(hca_regs->HASH[2]);
        out64[6] = bswap64(hca_regs->HASH[1]);
        out64[7] = bswap64(hca_regs->HASH[0]);
    }

    *hash_len = METAL_CRYPTO_HASH_SHA512_BYTE_SIZE;

    return (METAL_CRYPTO_OK);
}


uint32_t sifive_hca0_sha_getrev(struct sifive_hca0 hca)
{
    HCA_Type *hca_regs;

    if (get_index(hca) == METAL_HCA_INVALID_INDEX)
        return (METAL_CRYPTO_ERROR);

    hca_regs = (HCA_Type *)dt_hca_data[get_index(hca)].hca_regs;

    return (hca_regs->SHA_REV);
}

int32_t sifive_hca0_sha_init(struct sifive_hca0 hca, hca0_sha_ctx_t *const ctx,
                     metal_crypto_hash_mode_t hash_mode,
                     metal_crypto_endianness_t data_endianness)
{
    volatile uint32_t reg32;
    HCA_Type *hca_regs;

    if (get_index(hca) == METAL_HCA_INVALID_INDEX)
        return (METAL_CRYPTO_ERROR);

    hca_regs = (HCA_Type *)dt_hca_data[get_index(hca)].hca_regs;

    if ( 0 == (hca_regs->SHA_REV) )
    {
        /* revision of SHA is Zero so the SHA is not present. */
        return METAL_CRYPTO_ERROR;
    }

    ctx->mode = hash_mode;

    switch (ctx->mode)
    {
    case METAL_CRYPTO_HASH_SHA224:
        /* same context than sha256 */
        ctx->ctx.sha256.bitlen = 0;
        break;
    case METAL_CRYPTO_HASH_SHA256:
        ctx->ctx.sha256.bitlen = 0;
        break;
    case METAL_CRYPTO_HASH_SHA384:
        /* same context than sha512 */
        ctx->ctx.sha512.bitlen = 0;
        break;
    case METAL_CRYPTO_HASH_SHA512:
        ctx->ctx.sha512.bitlen = 0;
        break;
    default:
        return (METAL_CRYPTO_ERROR);
    }

    /* Set MODE */
    reg32 = hca_regs->CR;
    reg32 &= ~(HCA_CR_IFIFOTGT_Msk);
    reg32 |= ((HCA_SHA_MODE << HCA_CR_IFIFOTGT_Pos) & HCA_CR_IFIFOTGT_Msk);
    hca_regs->CR = reg32;

    /* Set endianness */
    reg32 = hca_regs->CR;
    reg32 &= ~(HCA_CR_ENDIANNESS_Msk);
    reg32 |= ((data_endianness << HCA_CR_ENDIANNESS_Pos) & HCA_CR_ENDIANNESS_Msk);
    hca_regs->CR = reg32;

    /* Set SHA mode */
    reg32 = hca_regs->SHA_CR;
    reg32 &= ~(HCA_SHA_CR_MODE_Msk);
    reg32 |= ((hash_mode << HCA_SHA_CR_MODE_Pos) & HCA_SHA_CR_MODE_Msk);
    hca_regs->SHA_CR = reg32;

    /* Init SHA */
    reg32 = hca_regs->SHA_CR;
    reg32 &= ~(HCA_SHA_CR_INIT_Msk);
    reg32 |= ((1 << HCA_SHA_CR_INIT_Pos) & HCA_SHA_CR_INIT_Msk);
    hca_regs->SHA_CR = reg32;

    return (METAL_CRYPTO_OK);
}

int32_t sifive_hca0_sha_core(struct sifive_hca0 hca, hca0_sha_ctx_t *const ctx,
                     const uint8_t *const data, size_t data_byte_len)
{
    if (NULL == ctx)
    {
        return (METAL_CRYPTO_INVALID_INPUT);
    }

    switch (ctx->mode)
    {
    case METAL_CRYPTO_HASH_SHA224:
        /* same core than sha256 */
        return (sifive_hca0_sha256_core(hca, ctx, data, data_byte_len));
    case METAL_CRYPTO_HASH_SHA256:
        return (sifive_hca0_sha256_core(hca, ctx, data, data_byte_len));
    case METAL_CRYPTO_HASH_SHA384:
        /* same core than sha384 */
        return (sifive_hca0_sha512_core(hca, ctx, data, data_byte_len));
    case METAL_CRYPTO_HASH_SHA512:
        return (sifive_hca0_sha512_core(hca, ctx, data, data_byte_len));
    default:
        break;
    }

    return (METAL_CRYPTO_ERROR);
}

int32_t sifive_hca0_sha_finish(struct sifive_hca0 hca, hca0_sha_ctx_t *const ctx,
                       uint8_t *const hash, size_t *const hash_len)
{
    if (NULL == ctx)
    {
        return (METAL_CRYPTO_INVALID_INPUT);
    }

    switch (ctx->mode)
    {
    case METAL_CRYPTO_HASH_SHA224:
        return (sifive_hca0_sha224_finish(hca, ctx, hash, hash_len));
    case METAL_CRYPTO_HASH_SHA256:
        return (sifive_hca0_sha256_finish(hca, ctx, hash, hash_len));
    case METAL_CRYPTO_HASH_SHA384:
        return (sifive_hca0_sha384_finish(hca, ctx, hash, hash_len));
    case METAL_CRYPTO_HASH_SHA512:
        return (sifive_hca0_sha512_finish(hca, ctx, hash, hash_len));
    default:
        break;
    }

    return (METAL_CRYPTO_ERROR);
}

#else /* METAL_SIFIVE_HCA0 */

/* Stubs for when no HCA SHA is present */
uint32_t sifive_hca0_sha_getrev(struct sifive_hca0 hca)
{ return 0;}
int32_t sifive_hca0_sha_init(struct sifive_hca0 hca,
    hca0_sha_ctx_t *const ctx,
    metal_crypto_hash_mode_t hash_mode,
    metal_crypto_endianness_t data_endianness)
{ return METAL_CRYPTO_ERROR; }

int32_t sifive_hca0_sha_core(struct sifive_hca0 hca,
    hca0_sha_ctx_t *const ctx,
    const uint8_t *const data,
    size_t data_byte_len)
{ return METAL_CRYPTO_ERROR; }

int32_t sifive_hca0_sha_finish(struct sifive_hca0 hca,
    hca0_sha_ctx_t *const ctx,
    uint8_t *const hash,
    size_t *const hash_len)
{ return METAL_CRYPTO_ERROR; }

#endif /* METAL_SIFIVE_HCA0 */
