/* Copyright 2020 SiFive, Inc */
/* SPDX-License-Identifier: Apache-2.0 */

#ifndef METAL__DRIVERS__SIFIVE_HCA0_REGS_H
#define METAL__DRIVERS__SIFIVE_HCA0_REGS_H

#include <metal/drivers/sifive_defs.h>
#include <stdint.h>

/*!
 * @file sifive_hca0_regs.h
 * @brief registers definition of HCA
 */

/**
 * @addtogroup HCA
 * @addtogroup HCA_REGISTERS
 * @ingroup HCA
 *  @{
 */

/* Want to keep this file as it is, even if line exceed 80 chars */
/* clang-format off */

#ifdef __cplusplus
extern "C" {
#endif

typedef struct _HCA {
    __IOM   uint32_t   CR;                 /**< Offset: 0x000 (R/W) Control */
            uint32_t   _reserved0[3U];
    __IOM   uint32_t   AES_CR;             /**< Offset: 0x010 (R/W) AES control */
            uint32_t   _reserved1[3U];
    __IOM   uint64_t   AES_ALEN;           /**< Offset: 0x020 (R/W) AES GCM/CCM AAD Length */
    __IOM   uint64_t   AES_PLDLEN;         /**< Offset: 0x028 (R/W) AES GCM/CCM Payload Length */
    __OM    uint64_t   AES_KEY[4U];        /**< Offset: 0x030 ( /W) AES Key */
    __IOM   uint64_t   AES_INITV[2U];      /**< Offset: 0x050 (R/W) AES Initialization Vector */
    __IOM   uint32_t   SHA_CR;             /**< Offset: 0x060 (R/W) SHA control */
            uint32_t   _reserved2[3U];
    __OM    uint64_t   FIFO_IN;            /**< Offset: 0x070 ( /W) AES/SHA FIFO Input */
            uint32_t   _reserved3[2U];
    __IM    uint64_t   AES_OUT;            /**< Offset: 0x080 (R/ ) AES FIFO Output */
            uint32_t   _reserved4[2U];
    __IM    uint64_t   AES_AUTH[2U];       /**< Offset: 0x090 (R/ ) AES Authentication Tag */
    __IM    uint64_t   HASH[8U];           /**< Offset: 0x0A0 (R/ ) HASH result from SHA */
    __IOM   uint32_t   TRNG_CR;            /**< Offset: 0x0E0 (R/W) TRNG control */
    __IOM   uint32_t   TRNG_SR;            /**< Offset: 0x0E4 (R/W) TRNG status */
    __IM    uint32_t   TRNG_DATA;          /**< Offset: 0x0E8 (R/ ) TRNG data */
    __IOM   uint32_t   TRNG_TRIM;          /**< Offset: 0x0EC (R/W) TRNG trim */
    __IM    uint32_t   TRNG_OSC1_CNT;      /**< Offset: 0x0F0 (R/ ) TRNG osc1 counter */
    __IM    uint32_t   TRNG_OSC2_CNT;      /**< Offset: 0x0F4 (R/ ) TRNG osc1 counter */
    __IM    uint64_t   TRNG_OSC1_SNAP;     /**< Offset: 0x0F8 (R/ ) TRNG osc1 snapshot */
    __IM    uint64_t   TRNG_OSC2_SNAP;     /**< Offset: 0x100 (R/ ) TRNG osc2 snapshot */
            uint32_t   _reserved5[2U];
    __IOM   uint32_t   DMA_CR;             /**< Offset: 0x110 (R/W) DMA control */
    __IOM   uint32_t   DMA_LEN;            /**< Offset: 0x114 (R/W) DMA length */
    __IOM   uint32_t   DMA_SRC;            /**< Offset: 0x118 (R/W) DMA source address */
            uint32_t   _reserved6;
    __IOM   uint32_t   DMA_DEST;           /**< Offset: 0x120 (R/W) DMA destination address */
            uint32_t   _reserved7[0x37U];
    __IM    uint32_t   HCA_REV;            /**< Offset: 0x200 (R/ ) HCA Revision */
    __IM    uint32_t   AES_REV;            /**< Offset: 0x204 (R/ ) AES Revision */
    __IM    uint32_t   SHA_REV;            /**< Offset: 0x208 (R/ ) SHA Revision */
    __IM    uint32_t   TRNG_REV;           /**< Offset: 0x20C (R/ ) TRNG Revision */
} HCA_Type;

/**
 * Structure type to access Control (CR)
 */
typedef union _HCA_CR {
    struct {
        uint32_t IFIFOTGT:1;       /**< bit:      0  Input FIFO target, 0: AES, 1: SHA */
        uint32_t IFIFOEMPTY:1;     /**< bit:      1  Input FIFO Empty */
        uint32_t IFIFOFULL:1;      /**< bit:      2  Input FIFO Full */
        uint32_t OFIFOEMPTY:1;     /**< bit:      3  Output FIFO Empty */
        uint32_t OFIFOFULL:1;      /**< bit:      4  Output FIFO Full */
        uint32_t ENDIANNESS:1;     /**< bit:      5  FIFOs endianness, 0: Little Endian, 1: Big Endian */
        uint32_t _reserved0:2;     /**< bit:   6..7  (reserved) */
        uint32_t CRYPTODIE:1;      /**< bit:      8  AES/SHA completion interrupt enable */
        uint32_t OFIFOIE:1;        /**< bit:      9  Output FIFO not empty interrupt enable */
        uint32_t DMADIE:1;         /**< bit:     10  DMA completion interrupt enable */
        uint32_t _reserved1:1;     /**< bit:     11  (reserved) */
        uint32_t CRYPTODIS:1;      /**< bit:     12  AES/SHA completion interrupt status */
        uint32_t OFIFOIS:1;        /**< bit:     13  Output FIFO not empty interrupt status */
        uint32_t DMADIS:1;         /**< bit:     14  DMA completion interrupt status */
        uint32_t _reserved2:1;     /**< bit:     15  (reserved) */
        uint32_t IFIFOCNT:6;       /**< bit: 16..21  Input FIFO count */
        uint32_t _reserved3:2;     /**< bit: 22..23  (reserved) */
        uint32_t AESFIFOCNT:6;     /**< bit: 24..29  AES output FIFO count */
        uint32_t _reserved4:2;     /**< bit: 30..31  (reserved) */
    } b;                           /**< Structure used for bit access */
    uint32_t w;                    /**< Structure used for word access */
} HCA_CR_Type;

/* HCA Control: Input FIFO target, 0: AES, 1: SHA */
#define HCA_CR_IFIFOTGT_Pos               0U
#define HCA_CR_IFIFOTGT_Msk               1U

/* HCA Control: Input FIFO Empty */
#define HCA_CR_IFIFOEMPTY_Pos             1U
#define HCA_CR_IFIFOEMPTY_Msk             (1U << HCA_CR_IFIFOEMPTY_Pos)

/* HCA Control: Input FIFO Full */
#define HCA_CR_IFIFOFULL_Pos              2U
#define HCA_CR_IFIFOFULL_Msk              (1U << HCA_CR_IFIFOFULL_Pos)

/* HCA Control: Output FIFO Empty */
#define HCA_CR_OFIFOEMPTY_Pos             3U
#define HCA_CR_OFIFOEMPTY_Msk             (1U << HCA_CR_OFIFOEMPTY_Pos)

/* HCA Control: Output FIFO Full */
#define HCA_CR_OFIFOFULL_Pos              4U
#define HCA_CR_OFIFOFULL_Msk              (1U << HCA_CR_OFIFOFULL_Pos)

/* HCA Control: FIFOs endianness, 0: Little Endian, 1: Big Endian */
#define HCA_CR_ENDIANNESS_Pos             5U
#define HCA_CR_ENDIANNESS_Msk             (1U << HCA_CR_ENDIANNESS_Pos)

/* HCA Control: AES/SHA completion interrupt enable */
#define HCA_CR_CRYPTODIE_Pos              8U
#define HCA_CR_CRYPTODIE_Msk              (1U << HCA_CR_CRYPTODIE_Pos)

/* HCA Control: Output FIFO not empty interrupt enable */
#define HCA_CR_OFIFOIE_Pos                9U
#define HCA_CR_OFIFOIE_Msk                (1U << HCA_CR_OFIFOIE_Pos)

/* HCA Control: DMA completion interrupt enable */
#define HCA_CR_DMADIE_Pos                 10U
#define HCA_CR_DMADIE_Msk                 (1U << HCA_CR_DMADIE_Pos)

/* HCA Control: AES/SHA completion interrupt status */
#define HCA_CR_CRYPTODIS_Pos              12U
#define HCA_CR_CRYPTODIS_Msk              (1U << HCA_CR_CRYPTODIS_Pos)

/* HCA Control: Output FIFO not empty interrupt status */
#define HCA_CR_OFIFOIS_Pos                13U
#define HCA_CR_OFIFOIS_Msk                (1U << HCA_CR_OFIFOIS_Pos)

/* HCA Control: DMA completion interrupt status */
#define HCA_CR_DMADIS_Pos                 14U
#define HCA_CR_DMADIS_Msk                 (1U << HCA_CR_DMADIS_Pos)

/* HCA Control: Input FIFO count */
#define HCA_CR_IFIFOCNT_Pos               16U
#define HCA_CR_IFIFOCNT_Msk               (0x3FU << HCA_CR_IFIFOCNT_Pos)

/* HCA Control: AES output FIFO count */
#define HCA_CR_AESFIFOCNT_Pos             24U
#define HCA_CR_AESFIFOCNT_Msk             (0x3FU << HCA_CR_AESFIFOCNT_Pos)

/**
 * Structure type to access AES control (AES_CR)
 */
typedef union _HCA_AES_CR {
    struct {
        uint32_t MODE:3;           /**< bit:   0..2  AES mode: ECB, CBC, CFB, OFB, CTR, GCM, CCM */
        uint32_t _reserved0:1;     /**< bit:      3  (reserved) */
        uint32_t KEYSZ:2;          /**< bit:   4..5  AES key size */
        uint32_t PROCESS:1;        /**< bit:      6  AES process type (encryption or decryption) */
        uint32_t INIT:1;           /**< bit:      7  AES Chain Initialization */
        uint32_t DTYPE:1;          /**< bit:      8  AES GCM/CCM data type */
        uint32_t CCMT:3;           /**< bit:  9..11  Encoded tag length parameter for CCM */
        uint32_t CCMQ:3;           /**< bit: 12..14  Encoded q parameter for CCM */
        uint32_t _reserved1:1;     /**< bit:     15  (reserved) */
        uint32_t BUSY:1;           /**< bit:     16  AES Busy bit */
        uint32_t _reserved2:15;    /**< bit: 17..31  (reserved) */
    } b;                           /**< Structure used for bit access */
    uint32_t w;                    /**< Structure used for word access */
} HCA_AES_CR_Type;

/* HCA AES control: Mode: ECB, CBC, CFB, OFB, CTR, GCM, CCM */
#define HCA_AES_CR_MODE_Pos               0U
#define HCA_AES_CR_MODE_Msk               7U

/* HCA AES control: Key size */
#define HCA_AES_CR_KEYSZ_Pos              4U
#define HCA_AES_CR_KEYSZ_Msk              (3U << HCA_AES_CR_KEYSZ_Pos)

/* HCA AES control: Process type (encryption or decryption) */
#define HCA_AES_CR_PROCESS_Pos            6U
#define HCA_AES_CR_PROCESS_Msk            (1U << HCA_AES_CR_PROCESS_Pos)

/* HCA AES control: Chain Initialization */
#define HCA_AES_CR_INIT_Pos               7U
#define HCA_AES_CR_INIT_Msk               (1U << HCA_AES_CR_INIT_Pos)

/* HCA AES control: Abort GCM/CCM operation */
#define HCA_AES_CR_DTYPE_Pos              8U
#define HCA_AES_CR_DTYPE_Msk              (1U << HCA_AES_CR_DTYPE_Pos)

/* HCA AES control: Encoded tag length parameter for CCM */
#define HCA_AES_CR_CCMT_Pos               9U
#define HCA_AES_CR_CCMT_Msk               (7U << HCA_AES_CR_CCMT_Pos)

/* HCA AES control: Encoded q parameter for CCM */
#define HCA_AES_CR_CCMQ_Pos               12U
#define HCA_AES_CR_CCMQ_Msk               (7U << HCA_AES_CR_CCMQ_Pos)

/* HCA AES control: Busy bit */
#define HCA_AES_CR_BUSY_Pos               16U
#define HCA_AES_CR_BUSY_Msk               (1U << HCA_AES_CR_BUSY_Pos)

/**
 * Structure type to access AES GCM/CCM AAD Length (AES_ALEN)
 */
typedef union _HCA_AES_ALEN {
    struct {
        uint64_t ALEN:61;          /**< bit:  0..60  AAD Length */
        uint64_t _reserved0:3;     /**< bit: 61..63  (reserved) */
    } b;                           /**< Structure used for bit access */
    uint64_t w;                    /**< Structure used for word access */
} HCA_AES_ALEN_Type;

/* HCA AES GCM/CCM AAD Length: AAD Length */
#define HCA_AES_ALEN_ALEN_Pos             0U
#define HCA_AES_ALEN_ALEN_Msk             0x1FFFFFFFFFFFFFFFU

/**
 * Structure type to access AES GCM/CCM Payload Length (AES_PLDLEN)
 */
typedef union _HCA_AES_PLDLEN {
    struct {
        uint64_t PLDLEN:61;        /**< bit:  0..60  Payload Length */
        uint64_t _reserved0:3;     /**< bit: 61..63  (reserved) */
    } b;                           /**< Structure used for bit access */
    uint64_t w;                    /**< Structure used for word access */
} HCA_AES_PLDLEN_Type;

/* HCA AES GCM/CCM Payload Length: Payload Length */
#define HCA_AES_PLDLEN_PLDLEN_Pos         0U
#define HCA_AES_PLDLEN_PLDLEN_Msk         0x1FFFFFFFFFFFFFFFU

/* HCA AES Key: 256-bits key 0 */
#define HCA_AES_KEY_KEY_0_Pos             0U
#define HCA_AES_KEY_KEY_0_Msk             0xFFFFFFFFFFFFFFFFU

/* HCA AES Key: 256-bits key 1 */
#define HCA_AES_KEY_KEY_1_Pos             0U
#define HCA_AES_KEY_KEY_1_Msk             0xFFFFFFFFFFFFFFFFU

/* HCA AES Key: 256-bits key 2 */
#define HCA_AES_KEY_KEY_2_Pos             0U
#define HCA_AES_KEY_KEY_2_Msk             0xFFFFFFFFFFFFFFFFU

/* HCA AES Key: 256-bits key 3 */
#define HCA_AES_KEY_KEY_3_Pos             0U
#define HCA_AES_KEY_KEY_3_Msk             0xFFFFFFFFFFFFFFFFU

/* HCA AES Initialization Vector: Initialization Vector 0 */
#define HCA_AES_INITV_INITV_0_Pos         0U
#define HCA_AES_INITV_INITV_0_Msk         0xFFFFFFFFFFFFFFFFU

/* HCA AES Initialization Vector: Initialization Vector 1 */
#define HCA_AES_INITV_INITV_1_Pos         0U
#define HCA_AES_INITV_INITV_1_Msk         0xFFFFFFFFFFFFFFFFU

/**
 * Structure type to access SHA control (SHA_CR)
 */
typedef union _HCA_SHA_CR {
    struct {
        uint32_t MODE:2;           /**< bit:   0..1  SHA mode: 224, 256, 384, 512 */
        uint32_t INIT:1;           /**< bit:      2  Initialize the SHA engine */
        uint32_t _reserved0:13;    /**< bit:  3..15  (reserved) */
        uint32_t BUSY:1;           /**< bit:     16  SHA Busy bit */
        uint32_t _reserved1:15;    /**< bit: 17..31  (reserved) */
    } b;                           /**< Structure used for bit access */
    uint32_t w;                    /**< Structure used for word access */
} HCA_SHA_CR_Type;

/* HCA SHA control: Mode: 224, 256, 384, 512 */
#define HCA_SHA_CR_MODE_Pos               0U
#define HCA_SHA_CR_MODE_Msk               3U

/* HCA SHA control: Initialize the SHA engine */
#define HCA_SHA_CR_INIT_Pos               2U
#define HCA_SHA_CR_INIT_Msk               (1U << HCA_SHA_CR_INIT_Pos)

/* HCA SHA control: Busy bit */
#define HCA_SHA_CR_BUSY_Pos               16U
#define HCA_SHA_CR_BUSY_Msk               (1U << HCA_SHA_CR_BUSY_Pos)

/* HCA AES/SHA FIFO Input: Data */
#define HCA_FIFO_IN_DATA_Pos              0U
#define HCA_FIFO_IN_DATA_Msk              0xFFFFFFFFFFFFFFFFU

/* HCA AES FIFO Output: FIFO Output */
#define HCA_AES_OUT_AES_OUT_Pos           0U
#define HCA_AES_OUT_AES_OUT_Msk           0xFFFFFFFFFFFFFFFFU

/* HCA AES Authentication Tag: Authentication Tag 0 */
#define HCA_AES_AUTH_AUTH_0_Pos           0U
#define HCA_AES_AUTH_AUTH_0_Msk           0xFFFFFFFFFFFFFFFFU

/* HCA AES Authentication Tag: Authentication Tag 1 */
#define HCA_AES_AUTH_AUTH_1_Pos           0U
#define HCA_AES_AUTH_AUTH_1_Msk           0xFFFFFFFFFFFFFFFFU

/* HCA HASH result from SHA: Result form SHA 0 */
#define HCA_HASH_HASH_0_Pos               0U
#define HCA_HASH_HASH_0_Msk               0xFFFFFFFFFFFFFFFFU

/* HCA HASH result from SHA: Result form SHA 1 */
#define HCA_HASH_HASH_1_Pos               0U
#define HCA_HASH_HASH_1_Msk               0xFFFFFFFFFFFFFFFFU

/* HCA HASH result from SHA: Result form SHA 2 */
#define HCA_HASH_HASH_2_Pos               0U
#define HCA_HASH_HASH_2_Msk               0xFFFFFFFFFFFFFFFFU

/* HCA HASH result from SHA: Result form SHA 3 */
#define HCA_HASH_HASH_3_Pos               0U
#define HCA_HASH_HASH_3_Msk               0xFFFFFFFFFFFFFFFFU

/* HCA HASH result from SHA: Result form SHA 4 */
#define HCA_HASH_HASH_4_Pos               0U
#define HCA_HASH_HASH_4_Msk               0xFFFFFFFFFFFFFFFFU

/* HCA HASH result from SHA: Result form SHA 5 */
#define HCA_HASH_HASH_5_Pos               0U
#define HCA_HASH_HASH_5_Msk               0xFFFFFFFFFFFFFFFFU

/* HCA HASH result from SHA: Result form SHA 6 */
#define HCA_HASH_HASH_6_Pos               0U
#define HCA_HASH_HASH_6_Msk               0xFFFFFFFFFFFFFFFFU

/* HCA HASH result from SHA: Result form SHA 7 */
#define HCA_HASH_HASH_7_Pos               0U
#define HCA_HASH_HASH_7_Msk               0xFFFFFFFFFFFFFFFFU

/**
 * Structure type to access TRNG control (TRNG_CR)
 */
typedef union _HCA_TRNG_CR {
    struct {
        uint32_t HTSTART:1;        /**< bit:      0  Health tests start */
        uint32_t RNDIRQEN:1;       /**< bit:      1  Random IRQ enable */
        uint32_t HTIRQEN:1;        /**< bit:      2  Health Tests IRQ enable */
        uint32_t BURSTEN:1;        /**< bit:      3  Burst mode enable */
        uint32_t _reserved0:4;     /**< bit:   4..7  (reserved) */
        uint32_t STARTOSCTEST:1;   /**< bit:      8  Oscillator test start */
        uint32_t _reserved1:7;     /**< bit:  9..15  (reserved) */
        uint32_t OSCTESTCNT:4;     /**< bit: 16..19  Oscillator test counter */
        uint32_t _reserved2:12;    /**< bit: 20..31  (reserved) */
    } b;                           /**< Structure used for bit access */
    uint32_t w;                    /**< Structure used for word access */
} HCA_TRNG_CR_Type;

/* HCA TRNG control: Health tests start */
#define HCA_TRNG_CR_HTSTART_Pos           0U
#define HCA_TRNG_CR_HTSTART_Msk           1U

/* HCA TRNG control: Random IRQ enable */
#define HCA_TRNG_CR_RNDIRQEN_Pos          1U
#define HCA_TRNG_CR_RNDIRQEN_Msk          (1U << HCA_TRNG_CR_RNDIRQEN_Pos)

/* HCA TRNG control: Health Tests IRQ enable */
#define HCA_TRNG_CR_HTIRQEN_Pos           2U
#define HCA_TRNG_CR_HTIRQEN_Msk           (1U << HCA_TRNG_CR_HTIRQEN_Pos)

/* HCA TRNG control: Burst mode enable */
#define HCA_TRNG_CR_BURSTEN_Pos           3U
#define HCA_TRNG_CR_BURSTEN_Msk           (1U << HCA_TRNG_CR_BURSTEN_Pos)

/* HCA TRNG control: Oscillator test start */
#define HCA_TRNG_CR_STARTOSCTEST_Pos      8U
#define HCA_TRNG_CR_STARTOSCTEST_Msk      (1U << HCA_TRNG_CR_STARTOSCTEST_Pos)

/* HCA TRNG control: Oscillator test counter */
#define HCA_TRNG_CR_OSCTESTCNT_Pos        16U
#define HCA_TRNG_CR_OSCTESTCNT_Msk        (0xFU << HCA_TRNG_CR_OSCTESTCNT_Pos)

/**
 * Structure type to access TRNG status (TRNG_SR)
 */
typedef union _HCA_TRNG_SR {
    struct {
        uint32_t RNDRDY:1;         /**< bit:      0  32-bit random data is ready to read from TRNG_DATA register */
        uint32_t SRCS:1;           /**< bit:      1  Entropy source status (0:running, 1:failed) */
        uint32_t HTR:1;            /**< bit:      2  Health test ready (0:done, 1:on-going) */
        uint32_t HTS:1;            /**< bit:      3  Health test status (0:pass, 1:fail) */
        uint32_t _reserved0:4;     /**< bit:   4..7  (reserved) */
        uint32_t ADAPFAIL:1;       /**< bit:      8  Adaptative test failing bit */
        uint32_t REPFAIL:1;        /**< bit:      9  Repetition test failing bit */
        uint32_t OSCTESTDONE:1;    /**< bit:     10  Osc Test Done */
        uint32_t _reserved1:21;    /**< bit: 11..31  (reserved) */
    } b;                           /**< Structure used for bit access */
    uint32_t w;                    /**< Structure used for word access */
} HCA_TRNG_SR_Type;

/* HCA TRNG status: 32-bit random data is ready to read from TRNG_DATA register */
#define HCA_TRNG_SR_RNDRDY_Pos            0U
#define HCA_TRNG_SR_RNDRDY_Msk            1U

/* HCA TRNG status: Entropy source status (0:running, 1:failed) */
#define HCA_TRNG_SR_SRCS_Pos              1U
#define HCA_TRNG_SR_SRCS_Msk              (1U << HCA_TRNG_SR_SRCS_Pos)

/* HCA TRNG status: Health test ready (0:done, 1:on-going) */
#define HCA_TRNG_SR_HTR_Pos               2U
#define HCA_TRNG_SR_HTR_Msk               (1U << HCA_TRNG_SR_HTR_Pos)

/* HCA TRNG status: Health test status (0:pass, 1:fail) */
#define HCA_TRNG_SR_HTS_Pos               3U
#define HCA_TRNG_SR_HTS_Msk               (1U << HCA_TRNG_SR_HTS_Pos)

/* HCA TRNG status: Adaptative test failing bit */
#define HCA_TRNG_SR_ADAPFAIL_Pos          8U
#define HCA_TRNG_SR_ADAPFAIL_Msk          (1U << HCA_TRNG_SR_ADAPFAIL_Pos)

/* HCA TRNG status: Repetition test failing bit */
#define HCA_TRNG_SR_REPFAIL_Pos           9U
#define HCA_TRNG_SR_REPFAIL_Msk           (1U << HCA_TRNG_SR_REPFAIL_Pos)

/* HCA TRNG status: Osc Test Done */
#define HCA_TRNG_SR_OSCTESTDONE_Pos       10U
#define HCA_TRNG_SR_OSCTESTDONE_Msk       (1U << HCA_TRNG_SR_OSCTESTDONE_Pos)

/* HCA TRNG data: Data register */
#define HCA_TRNG_DATA_DATA_Pos            0U
#define HCA_TRNG_DATA_DATA_Msk            0xFFFFFFFFU

/**
 * Structure type to access TRNG trim (TRNG_TRIM)
 */
typedef union _HCA_TRNG_TRIM {
    struct {
        uint32_t TRIM:27;          /**< bit:  0..26  TRNG trim value */
        uint32_t _reserved0:4;     /**< bit: 27..30  (reserved) */
        uint32_t LOCK:1;           /**< bit:     31  TRNG trim lock (0:RW, 1:R) */
    } b;                           /**< Structure used for bit access */
    uint32_t w;                    /**< Structure used for word access */
} HCA_TRNG_TRIM_Type;

/* HCA TRNG trim: Trim value */
#define HCA_TRNG_TRIM_TRIM_Pos            0U
#define HCA_TRNG_TRIM_TRIM_Msk            0x7FFFFFFU

/* HCA TRNG trim: Trim lock (0:RW, 1:R) */
#define HCA_TRNG_TRIM_LOCK_Pos            31U
#define HCA_TRNG_TRIM_LOCK_Msk            (1U << HCA_TRNG_TRIM_LOCK_Pos)

/* HCA TRNG osc1 counter: OSC1 counter register */
#define HCA_TRNG_OSC1_CNT_OSC1CNT_Pos     0U
#define HCA_TRNG_OSC1_CNT_OSC1CNT_Msk     0xFFFFFFFFU

/* HCA TRNG osc1 counter: OSC2 counter register */
#define HCA_TRNG_OSC2_CNT_OSC2CNT_Pos     0U
#define HCA_TRNG_OSC2_CNT_OSC2CNT_Msk     0xFFFFFFFFU

/* HCA TRNG osc1 snapshot: OSC1 snapshot register */
#define HCA_TRNG_OSC1_SNAP_OSC1SNAP_Pos   0U
#define HCA_TRNG_OSC1_SNAP_OSC1SNAP_Msk   0xFFFFFFFFFFFFFFFFU

/* HCA TRNG osc2 snapshot: OSC2 snapshot register */
#define HCA_TRNG_OSC2_SNAP_OSC2SNAP_Pos   0U
#define HCA_TRNG_OSC2_SNAP_OSC2SNAP_Msk   0xFFFFFFFFFFFFFFFFU

/**
 * Structure type to access DMA control (DMA_CR)
 */
typedef union _HCA_DMA_CR {
    struct {
        uint32_t START:1;          /**< bit:      0  DMA Start */
        uint32_t _reserved0:7;     /**< bit:   1..7  (reserved) */
        uint32_t BUSY:1;           /**< bit:      8  DMA Busy */
        uint32_t RDALIGNERR:1;     /**< bit:      9  Read alignment error */
        uint32_t WRALIGNERR:1;     /**< bit:     10  Write alignment error */
        uint32_t RESPERR:1;        /**< bit:     11  DMA Response error */
        uint32_t LEGALERR:1;       /**< bit:     12  DMA Legal error */
        uint32_t _reserved1:19;    /**< bit: 13..31  (reserved) */
    } b;                           /**< Structure used for bit access */
    uint32_t w;                    /**< Structure used for word access */
} HCA_DMA_CR_Type;

/* HCA DMA control: Start */
#define HCA_DMA_CR_START_Pos              0U
#define HCA_DMA_CR_START_Msk              1U

/* HCA DMA control: Busy */
#define HCA_DMA_CR_BUSY_Pos               8U
#define HCA_DMA_CR_BUSY_Msk               (1U << HCA_DMA_CR_BUSY_Pos)

/* HCA DMA control: Read alignment error */
#define HCA_DMA_CR_RDALIGNERR_Pos         9U
#define HCA_DMA_CR_RDALIGNERR_Msk         (1U << HCA_DMA_CR_RDALIGNERR_Pos)

/* HCA DMA control: Write alignment error */
#define HCA_DMA_CR_WRALIGNERR_Pos         10U
#define HCA_DMA_CR_WRALIGNERR_Msk         (1U << HCA_DMA_CR_WRALIGNERR_Pos)

/* HCA DMA control: Response error */
#define HCA_DMA_CR_RESPERR_Pos            11U
#define HCA_DMA_CR_RESPERR_Msk            (1U << HCA_DMA_CR_RESPERR_Pos)

/* HCA DMA control: Legal error */
#define HCA_DMA_CR_LEGALERR_Pos           12U
#define HCA_DMA_CR_LEGALERR_Msk           (1U << HCA_DMA_CR_LEGALERR_Pos)

/**
 * Structure type to access DMA length (DMA_LEN)
 */
typedef union _HCA_DMA_LEN {
    struct {
        uint32_t LEN:28;           /**< bit:  0..27  DMA Length (number of 128-bits blocks) */
        uint32_t _reserved0:4;     /**< bit: 28..31  (reserved) */
    } b;                           /**< Structure used for bit access */
    uint32_t w;                    /**< Structure used for word access */
} HCA_DMA_LEN_Type;

/* HCA DMA length: Length (number of 128-bits blocks) */
#define HCA_DMA_LEN_LEN_Pos               0U
#define HCA_DMA_LEN_LEN_Msk               0xFFFFFFFU

/* HCA DMA source address: Source Address */
#define HCA_DMA_SRC_SRC_Pos               0U
#define HCA_DMA_SRC_SRC_Msk               0xFFFFFFFFU

/* HCA DMA destination address: Destination Address */
#define HCA_DMA_DEST_DEST_Pos             0U
#define HCA_DMA_DEST_DEST_Msk             0xFFFFFFFFU

/**
 * Structure type to access HCA Revision (HCA_REV)
 */
typedef union _HCA_HCA_REV {
    struct {
        uint32_t PATCHREV:4;       /**< bit:   0..3  HCA Patch Revision */
        uint32_t MINORREV:4;       /**< bit:   4..7  HCA Minor Revision */
        uint32_t MAJORREV:4;       /**< bit:  8..11  HCA Major Revision */
        uint32_t _reserved0:20;    /**< bit: 12..31  (reserved) */
    } b;                           /**< Structure used for bit access */
    uint32_t w;                    /**< Structure used for word access */
} HCA_HCA_REV_Type;

/* HCA HCA Revision: Patch Revision */
#define HCA_HCA_REV_PATCHREV_Pos          0U
#define HCA_HCA_REV_PATCHREV_Msk          0xFU

/* HCA HCA Revision: Minor Revision */
#define HCA_HCA_REV_MINORREV_Pos          4U
#define HCA_HCA_REV_MINORREV_Msk          (0xFU << HCA_HCA_REV_MINORREV_Pos)

/* HCA HCA Revision: Major Revision */
#define HCA_HCA_REV_MAJORREV_Pos          8U
#define HCA_HCA_REV_MAJORREV_Msk          (0xFU << HCA_HCA_REV_MAJORREV_Pos)

/**
 * Structure type to access AES Revision (AES_REV)
 */
typedef union _HCA_AES_REV {
    struct {
        uint32_t PATCHREV:4;       /**< bit:   0..3  AES Patch Revision */
        uint32_t MINORREV:4;       /**< bit:   4..7  AES Minor Revision */
        uint32_t MAJORREV:4;       /**< bit:  8..11  AES Major Revision */
        uint32_t TYPE:7;           /**< bit: 12..18  AES Type */
        uint32_t _reserved0:13;    /**< bit: 19..31  (reserved) */
    } b;                           /**< Structure used for bit access */
    uint32_t w;                    /**< Structure used for word access */
} HCA_AES_REV_Type;

/* HCA AES Revision: Patch Revision */
#define HCA_AES_REV_PATCHREV_Pos          0U
#define HCA_AES_REV_PATCHREV_Msk          0xFU

/* HCA AES Revision: Minor Revision */
#define HCA_AES_REV_MINORREV_Pos          4U
#define HCA_AES_REV_MINORREV_Msk          (0xFU << HCA_AES_REV_MINORREV_Pos)

/* HCA AES Revision: Major Revision */
#define HCA_AES_REV_MAJORREV_Pos          8U
#define HCA_AES_REV_MAJORREV_Msk          (0xFU << HCA_AES_REV_MAJORREV_Pos)

/* HCA AES Revision: Type */
#define HCA_AES_REV_TYPE_Pos              12U
#define HCA_AES_REV_TYPE_Msk              (0x7FU << HCA_AES_REV_TYPE_Pos)

/**
 * Structure type to access SHA Revision (SHA_REV)
 */
typedef union _HCA_SHA_REV {
    struct {
        uint32_t PATCHREV:4;       /**< bit:   0..3  SHA Patch Revision */
        uint32_t MINORREV:4;       /**< bit:   4..7  SHA Minor Revision */
        uint32_t MAJORREV:4;       /**< bit:  8..11  SHA Major Revision */
        uint32_t TYPE:4;           /**< bit: 12..15  SHA Type */
        uint16_t _reserved0;       /**< bit: 16..31  (reserved) */
    } b;                           /**< Structure used for bit access */
    uint32_t w;                    /**< Structure used for word access */
} HCA_SHA_REV_Type;

/* HCA SHA Revision: Patch Revision */
#define HCA_SHA_REV_PATCHREV_Pos          0U
#define HCA_SHA_REV_PATCHREV_Msk          0xFU

/* HCA SHA Revision: Minor Revision */
#define HCA_SHA_REV_MINORREV_Pos          4U
#define HCA_SHA_REV_MINORREV_Msk          (0xFU << HCA_SHA_REV_MINORREV_Pos)

/* HCA SHA Revision: Major Revision */
#define HCA_SHA_REV_MAJORREV_Pos          8U
#define HCA_SHA_REV_MAJORREV_Msk          (0xFU << HCA_SHA_REV_MAJORREV_Pos)

/* HCA SHA Revision: Type */
#define HCA_SHA_REV_TYPE_Pos              12U
#define HCA_SHA_REV_TYPE_Msk              (0xFU << HCA_SHA_REV_TYPE_Pos)

/**
 * Structure type to access TRNG Revision (TRNG_REV)
 */
typedef union _HCA_TRNG_REV {
    struct {
        uint32_t PATCHREV:4;       /**< bit:   0..3  TRNG Patch Revision */
        uint32_t MINORREV:4;       /**< bit:   4..7  TRNG Minor Revision */
        uint32_t MAJORREV:4;       /**< bit:  8..11  TRNG Major Revision */
        uint32_t TYPE:4;           /**< bit: 12..15  TRNG Type */
        uint16_t _reserved0;       /**< bit: 16..31  (reserved) */
    } b;                           /**< Structure used for bit access */
    uint32_t w;                    /**< Structure used for word access */
} HCA_TRNG_REV_Type;

/* HCA TRNG Revision: Patch Revision */
#define HCA_TRNG_REV_PATCHREV_Pos         0U
#define HCA_TRNG_REV_PATCHREV_Msk         0xFU

/* HCA TRNG Revision: Minor Revision */
#define HCA_TRNG_REV_MINORREV_Pos         4U
#define HCA_TRNG_REV_MINORREV_Msk         (0xFU << HCA_TRNG_REV_MINORREV_Pos)

/* HCA TRNG Revision: Major Revision */
#define HCA_TRNG_REV_MAJORREV_Pos         8U
#define HCA_TRNG_REV_MAJORREV_Msk         (0xFU << HCA_TRNG_REV_MAJORREV_Pos)

/* HCA TRNG Revision: Type */
#define HCA_TRNG_REV_TYPE_Pos             12U
#define HCA_TRNG_REV_TYPE_Msk             (0xFU << HCA_TRNG_REV_TYPE_Pos)

#ifdef __cplusplus
}
#endif

/** @}*/
#endif /* METAL__DRIVERS__SIFIVE_HCA0_REGS_H */
