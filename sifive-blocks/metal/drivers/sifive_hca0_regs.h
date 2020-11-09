/* Copyright 2020 SiFive, Inc */
/* SPDX-License-Identifier: Apache-2.0 */

#ifndef SIFIVE_BLOCKS__METAL__DRIVERS__SIFIVE_HCA0_REGS_H
#define SIFIVE_BLOCKS__METAL__DRIVERS__SIFIVE_HCA0_REGS_H

#include <metal/drivers/sifive_defs.h>
#include <stdint.h>

/*!
 * @file sifive_hca0_sha.h
 * @brief API for AES crypto operation through HCA
 */

/**
 * @addtogroup HCA
 * @addtogroup HCA_REGISTERS
 * @ingroup HCA
 *  @{
 */

typedef struct _HCA {
    __IOM uint32_t CR; /**< Offset: 0x000 (R/W) Control */
    uint32_t _reserved0[3U];
    __IOM uint32_t AES_CR; /**< Offset: 0x010 (R/W) AES control */
    uint32_t _reserved1[3U];
    __IOM uint64_t AES_ALEN; /**< Offset: 0x020 (R/W) AES GCM/CCM AAD Length */
    __IOM uint64_t
        AES_PLDLEN; /**< Offset: 0x028 (R/W) AES GCM/CCM Payload Length */
    __OM uint64_t AES_KEY[4U]; /**< Offset: 0x030 ( /W) AES Key */
    __IOM uint64_t
        AES_INITV[2U];     /**< Offset: 0x050 (R/W) AES Initialization Vector */
    __IOM uint32_t SHA_CR; /**< Offset: 0x060 (R/W) SHA control */
    uint32_t _reserved2[3U];
    __OM uint64_t FIFO_IN; /**< Offset: 0x070 ( /W) AES/SHA FIFO Input */
    uint32_t _reserved3[2U];
    __IM uint64_t AES_OUT; /**< Offset: 0x080 (R/ ) AES FIFO Output */
    uint32_t _reserved4[2U];
    __IM uint64_t
        AES_AUTH[2U];         /**< Offset: 0x090 (R/ ) AES Authentication Tag */
    __IM uint64_t HASH[8U];   /**< Offset: 0x0A0 (R/ ) HASH result from SHA */
    __IOM uint32_t TRNG_CR;   /**< Offset: 0x0E0 (R/W) TRNG control */
    __IOM uint32_t TRNG_SR;   /**< Offset: 0x0E4 (R/W) TRNG status */
    __IM uint32_t TRNG_DATA;  /**< Offset: 0x0E8 (R/ ) TRNG data */
    __IOM uint32_t TRNG_TRIM; /**< Offset: 0x0EC (R/W) TRNG trim */
    __IM uint32_t TRNG_OSC1_CNT;  /**< Offset: 0x0F0 (R/ ) TRNG osc1 counter */
    __IM uint32_t TRNG_OSC2_CNT;  /**< Offset: 0x0F4 (R/ ) TRNG osc1 counter */
    __IM uint64_t TRNG_OSC1_SNAP; /**< Offset: 0x0F8 (R/ ) TRNG osc1 snapshot */
    __IM uint64_t TRNG_OSC2_SNAP; /**< Offset: 0x100 (R/ ) TRNG osc2 snapshot */
    uint32_t _reserved5[2U];
    __IOM uint32_t DMA_CR;  /**< Offset: 0x110 (R/W) DMA control */
    __IOM uint32_t DMA_LEN; /**< Offset: 0x114 (R/W) DMA length */
    __IOM uint32_t DMA_SRC; /**< Offset: 0x118 (R/W) DMA source address */
    uint32_t _reserved6;
    __IOM uint32_t DMA_DEST; /**< Offset: 0x120 (R/W) DMA destination address */
    uint32_t _reserved7[0x37U];
    __IM uint32_t HCA_REV;  /**< Offset: 0x200 (R/ ) HCA Revision */
    __IM uint32_t AES_REV;  /**< Offset: 0x204 (R/ ) AES Revision */
    __IM uint32_t SHA_REV;  /**< Offset: 0x208 (R/ ) SHA Revision */
    __IM uint32_t TRNG_REV; /**< Offset: 0x20C (R/ ) TRNG Revision */
} HCA_Type;

/**
 * Structure type to access Control (CR)
 */
typedef union _HCA_CR {
    struct {
        uint32_t
            IFIFOTGT : 1; /**< bit:      0  Input FIFO target, 0: AES, 1: SHA */
        uint32_t IFIFOEMPTY : 1; /**< bit:      1  Input FIFO Empty */
        uint32_t IFIFOFULL : 1;  /**< bit:      2  Input FIFO Full */
        uint32_t OFIFOEMPTY : 1; /**< bit:      3  Output FIFO Empty */
        uint32_t OFIFOFULL : 1;  /**< bit:      4  Output FIFO Full */
        uint32_t ENDIANNESS : 1; /**< bit:      5  FIFOs endianness, 0: Little
                                    Endian, 1: Big Endian */
        uint32_t _reserved0 : 2; /**< bit:   6..7  (reserved) */
        uint32_t CRYPTODIE : 1;  /**< bit:      8  AES/SHA completion interrupt
                                    enable */
        uint32_t OFIFOIE : 1; /**< bit:      9  Output FIFO not empty interrupt
                                 enable */
        uint32_t
            DMADIE : 1; /**< bit:     10  DMA completion interrupt enable */
        uint32_t _reserved1 : 1; /**< bit:     11  (reserved) */
        uint32_t CRYPTODIS : 1;  /**< bit:     12  AES/SHA completion interrupt
                                    status */
        uint32_t OFIFOIS : 1; /**< bit:     13  Output FIFO not empty interrupt
                                 status */
        uint32_t
            DMADIS : 1; /**< bit:     14  DMA completion interrupt status */
        uint32_t _reserved2 : 1; /**< bit:     15  (reserved) */
        uint32_t IFIFOCNT : 6;   /**< bit: 16..21  Input FIFO count */
        uint32_t _reserved3 : 2; /**< bit: 22..23  (reserved) */
        uint32_t AESFIFOCNT : 6; /**< bit: 24..29  AES output FIFO count */
        uint32_t _reserved4 : 2; /**< bit: 30..31  (reserved) */
    } b;                         /**< Structure used for bit access */
    uint32_t w;                  /**< Structure used for word access */
} HCA_CR_Type;

/* HCA Control */
#define HCA_CR_IFIFOTGT_Pos                                                    \
    0U /**< HCA CR: Input FIFO target, 0: AES, 1: SHA Position */
#define HCA_CR_IFIFOTGT_Msk                                                    \
    (1U << HCA_CR_IFIFOTGT_Pos) /**< HCA CR: Input FIFO target, 0: AES, 1: SHA \
                                   Mask */

#define HCA_CR_IFIFOEMPTY_Pos 1U /**< HCA CR: Input FIFO Empty Position */
#define HCA_CR_IFIFOEMPTY_Msk                                                  \
    (1U << HCA_CR_IFIFOEMPTY_Pos) /**< HCA CR: Input FIFO Empty Mask */

#define HCA_CR_IFIFOFULL_Pos 2U /**< HCA CR: Input FIFO Full Position */
#define HCA_CR_IFIFOFULL_Msk                                                   \
    (1U << HCA_CR_IFIFOFULL_Pos) /**< HCA CR: Input FIFO Full Mask */

#define HCA_CR_OFIFOEMPTY_Pos 3U /**< HCA CR: Output FIFO Empty Position */
#define HCA_CR_OFIFOEMPTY_Msk                                                  \
    (1U << HCA_CR_OFIFOEMPTY_Pos) /**< HCA CR: Output FIFO Empty Mask */

#define HCA_CR_OFIFOFULL_Pos 4U /**< HCA CR: Output FIFO Full Position */
#define HCA_CR_OFIFOFULL_Msk                                                   \
    (1U << HCA_CR_OFIFOFULL_Pos) /**< HCA CR: Output FIFO Full Mask */

#define HCA_CR_ENDIANNESS_Pos                                                  \
    5U /**< HCA CR: FIFOs endianness, 0: Little Endian, 1: Big Endian Position \
        */
#define HCA_CR_ENDIANNESS_Msk                                                  \
    (1U << HCA_CR_ENDIANNESS_Pos) /**< HCA CR: FIFOs endianness, 0: Little     \
                                     Endian, 1: Big Endian Mask */

#define HCA_CR_CRYPTODIE_Pos                                                   \
    8U /**< HCA CR: AES/SHA completion interrupt enable Position */
#define HCA_CR_CRYPTODIE_Msk                                                   \
    (1U << HCA_CR_CRYPTODIE_Pos) /**< HCA CR: AES/SHA completion interrupt     \
                                    enable Mask */

#define HCA_CR_OFIFOIE_Pos                                                     \
    9U /**< HCA CR: Output FIFO not empty interrupt enable Position */
#define HCA_CR_OFIFOIE_Msk                                                     \
    (1U << HCA_CR_OFIFOIE_Pos) /**< HCA CR: Output FIFO not empty interrupt    \
                                  enable Mask */

#define HCA_CR_DMADIE_Pos                                                      \
    10U /**< HCA CR: DMA completion interrupt enable Position */
#define HCA_CR_DMADIE_Msk                                                      \
    (1U << HCA_CR_DMADIE_Pos) /**< HCA CR: DMA completion interrupt enable     \
                                 Mask */

#define HCA_CR_CRYPTODIS_Pos                                                   \
    12U /**< HCA CR: AES/SHA completion interrupt status Position */
#define HCA_CR_CRYPTODIS_Msk                                                   \
    (1U << HCA_CR_CRYPTODIS_Pos) /**< HCA CR: AES/SHA completion interrupt     \
                                    status Mask */

#define HCA_CR_OFIFOIS_Pos                                                     \
    13U /**< HCA CR: Output FIFO not empty interrupt status Position */
#define HCA_CR_OFIFOIS_Msk                                                     \
    (1U << HCA_CR_OFIFOIS_Pos) /**< HCA CR: Output FIFO not empty interrupt    \
                                  status Mask */

#define HCA_CR_DMADIS_Pos                                                      \
    14U /**< HCA CR: DMA completion interrupt status Position */
#define HCA_CR_DMADIS_Msk                                                      \
    (1U << HCA_CR_DMADIS_Pos) /**< HCA CR: DMA completion interrupt status     \
                                 Mask */

#define HCA_CR_IFIFOCNT_Pos 16U /**< HCA CR: Input FIFO count Position */
#define HCA_CR_IFIFOCNT_Msk                                                    \
    (0x3FU << HCA_CR_IFIFOCNT_Pos) /**< HCA CR: Input FIFO count Mask */

#define HCA_CR_AESFIFOCNT_Pos 24U /**< HCA CR: AES output FIFO count Position  \
                                   */
#define HCA_CR_AESFIFOCNT_Msk                                                  \
    (0x3FU << HCA_CR_AESFIFOCNT_Pos) /**< HCA CR: AES output FIFO count Mask   \
                                      */

/**
 * Structure type to access AES control (AES_CR)
 */
typedef union _HCA_AES_CR {
    struct {
        uint32_t MODE : 3; /**< bit:   0..2  AES mode: ECB, CBC, CFB, OFB, CTR,
                              GCM, CCM */
        uint32_t _reserved0 : 1; /**< bit:      3  (reserved) */
        uint32_t KEYSZ : 2;      /**< bit:   4..5  AES key size */
        uint32_t PROCESS : 1; /**< bit:      6  AES process type (encryption or
                                 decryption) */
        uint32_t INIT : 1;    /**< bit:      7  AES Chain Initialization */
        uint32_t DTYPE : 1;   /**< bit:      8  Abort GCM/CCM operation */
        uint32_t
            CCMT : 3; /**< bit:  9..11  Encoded tag length parameter for CCM */
        uint32_t CCMQ : 3; /**< bit: 12..14  Encoded q parameter for CCM */
        uint32_t _reserved1 : 1;  /**< bit:     15  (reserved) */
        uint32_t BUSY : 1;        /**< bit:     16  AES Busy bit */
        uint32_t _reserved2 : 15; /**< bit: 17..31  (reserved) */
    } b;                          /**< Structure used for bit access */
    uint32_t w;                   /**< Structure used for word access */
} HCA_AES_CR_Type;

/* HCA AES control */
#define HCA_AES_CR_MODE_Pos                                                    \
    0U /**< HCA AES_CR: Mode: ECB, CBC, CFB, OFB, CTR, GCM, CCM Position */
#define HCA_AES_CR_MODE_Msk                                                    \
    (7U << HCA_AES_CR_MODE_Pos) /**< HCA AES_CR: Mode: ECB, CBC, CFB, OFB,     \
                                   CTR, GCM, CCM Mask */

#define HCA_AES_CR_KEYSZ_Pos 4U /**< HCA AES_CR: Key size Position */
#define HCA_AES_CR_KEYSZ_Msk                                                   \
    (3U << HCA_AES_CR_KEYSZ_Pos) /**< HCA AES_CR: Key size Mask */

#define HCA_AES_CR_PROCESS_Pos                                                 \
    6U /**< HCA AES_CR: Process type (encryption or decryption) Position */
#define HCA_AES_CR_PROCESS_Msk                                                 \
    (1U << HCA_AES_CR_PROCESS_Pos) /**< HCA AES_CR: Process type (encryption   \
                                      or decryption) Mask */

#define HCA_AES_CR_INIT_Pos 7U /**< HCA AES_CR: Chain Initialization Position  \
                                */
#define HCA_AES_CR_INIT_Msk                                                    \
    (1U << HCA_AES_CR_INIT_Pos) /**< HCA AES_CR: Chain Initialization Mask */

#define HCA_AES_CR_DTYPE_Pos                                                   \
    8U /**< HCA AES_CR: Data Type for GCM/CCM operation Position */
#define HCA_AES_CR_DTYPE_Msk                                                   \
    (1U << HCA_AES_CR_DTYPE_Pos) /**< HCA AES_CR: Data Type for GCM/CCM        \
                                    operation Mask */

#define HCA_AES_CR_CCMT_Pos                                                    \
    9U /**< HCA AES_CR: Encoded tag length parameter for CCM Position */
#define HCA_AES_CR_CCMT_Msk                                                    \
    (7U << HCA_AES_CR_CCMT_Pos) /**< HCA AES_CR: Encoded tag length parameter  \
                                   for CCM Mask */

#define HCA_AES_CR_CCMQ_Pos                                                    \
    12U /**< HCA AES_CR: Encoded q parameter for CCM Position */
#define HCA_AES_CR_CCMQ_Msk                                                    \
    (7U << HCA_AES_CR_CCMQ_Pos) /**< HCA AES_CR: Encoded q parameter for CCM   \
                                   Mask */

#define HCA_AES_CR_BUSY_Pos 16U /**< HCA AES_CR: Busy bit Position */
#define HCA_AES_CR_BUSY_Msk                                                    \
    (1U << HCA_AES_CR_BUSY_Pos) /**< HCA AES_CR: Busy bit Mask */

/**
 * Structure type to access AES GCM/CCM AAD Length (AES_ALEN)
 */
typedef union _HCA_AES_ALEN {
    struct {
        uint64_t ALEN : 61;      /**< bit:  0..60  AAD Length */
        uint64_t _reserved0 : 3; /**< bit: 61..63  (reserved) */
    } b;                         /**< Structure used for bit access */
    uint64_t w;                  /**< Structure used for word access */
} HCA_AES_ALEN_Type;

/* HCA AES GCM/CCM AAD Length */
#define HCA_AES_ALEN_ALEN_Pos 0U /**< HCA AES_ALEN: AAD Length Position */
#define HCA_AES_ALEN_ALEN_Msk                                                  \
    0x1FFFFFFFFFFFFFFFU /**< HCA AES_ALEN: AAD Length Mask */

/**
 * Structure type to access AES GCM/CCM Payload Length (AES_PLDLEN)
 */
typedef union _HCA_AES_PLDLEN {
    struct {
        uint64_t PLDLEN : 61;    /**< bit:  0..60  Payload Length */
        uint64_t _reserved0 : 3; /**< bit: 61..63  (reserved) */
    } b;                         /**< Structure used for bit access */
    uint64_t w;                  /**< Structure used for word access */
} HCA_AES_PLDLEN_Type;

/* HCA AES GCM/CCM Payload Length */
#define HCA_AES_PLDLEN_PLDLEN_Pos                                              \
    0U /**< HCA AES_PLDLEN: Payload Length Position */
#define HCA_AES_PLDLEN_PLDLEN_Msk                                              \
    0x1FFFFFFFFFFFFFFFU /**< HCA AES_PLDLEN: Payload Length Mask */

/* HCA AES Key */
#define HCA_AES_KEY_KEY_0_Pos 0U /**< HCA AES_KEY: 256-bits key 0 Position */
#define HCA_AES_KEY_KEY_0_Msk                                                  \
    0xFFFFFFFFFFFFFFFFU /**< HCA AES_KEY: 256-bits key 0 Mask */

#define HCA_AES_KEY_KEY_1_Pos 0U /**< HCA AES_KEY: 256-bits key 1 Position */
#define HCA_AES_KEY_KEY_1_Msk                                                  \
    0xFFFFFFFFFFFFFFFFU /**< HCA AES_KEY: 256-bits key 1 Mask */

#define HCA_AES_KEY_KEY_2_Pos 0U /**< HCA AES_KEY: 256-bits key 2 Position */
#define HCA_AES_KEY_KEY_2_Pos 0U /**< HCA AES_KEY: 256-bits key 2 Position */
#define HCA_AES_KEY_KEY_2_Msk                                                  \
    0xFFFFFFFFFFFFFFFFU /**< HCA AES_KEY: 256-bits key 2 Mask */

#define HCA_AES_KEY_KEY_3_Pos 0U /**< HCA AES_KEY: 256-bits key 3 Position */
#define HCA_AES_KEY_KEY_3_Msk                                                  \
    0xFFFFFFFFFFFFFFFFU /**< HCA AES_KEY: 256-bits key 3 Mask */

/* HCA AES Initialization Vector */
#define HCA_AES_INITV_INITV_0_Pos                                              \
    0U /**< HCA AES_INITV: Initialization Vector 0 Position */
#define HCA_AES_INITV_INITV_0_Msk                                              \
    0xFFFFFFFFFFFFFFFFU /**< HCA AES_INITV: Initialization Vector 0 Mask */

#define HCA_AES_INITV_INITV_1_Pos                                              \
    0U /**< HCA AES_INITV: Initialization Vector 1 Position */
#define HCA_AES_INITV_INITV_1_Msk                                              \
    0xFFFFFFFFFFFFFFFFU /**< HCA AES_INITV: Initialization Vector 1 Mask */

/**
 * Structure type to access SHA control (SHA_CR)
 */
typedef union _HCA_SHA_CR {
    struct {
        uint32_t MODE : 2; /**< bit:   0..1  SHA mode: 224, 256, 384, 512 */
        uint32_t INIT : 1; /**< bit:      2  Initialize the SHA engine */
        uint32_t _reserved0 : 13; /**< bit:  3..15  (reserved) */
        uint32_t BUSY : 1;        /**< bit:     16  SHA Busy bit */
        uint32_t _reserved1 : 15; /**< bit: 17..31  (reserved) */
    } b;                          /**< Structure used for bit access */
    uint32_t w;                   /**< Structure used for word access */
} HCA_SHA_CR_Type;

/* HCA SHA control */
#define HCA_SHA_CR_MODE_Pos                                                    \
    0U /**< HCA SHA_CR: Mode: 224, 256, 384, 512 Position */
#define HCA_SHA_CR_MODE_Msk                                                    \
    (3U                                                                        \
     << HCA_SHA_CR_MODE_Pos) /**< HCA SHA_CR: Mode: 224, 256, 384, 512 Mask */

#define HCA_SHA_CR_INIT_Pos                                                    \
    2U /**< HCA SHA_CR: Initialize the SHA engine Position */
#define HCA_SHA_CR_INIT_Msk                                                    \
    (1U << HCA_SHA_CR_INIT_Pos) /**< HCA SHA_CR: Initialize the SHA engine     \
                                   Mask */

#define HCA_SHA_CR_BUSY_Pos 16U /**< HCA SHA_CR: Busy bit Position */
#define HCA_SHA_CR_BUSY_Msk                                                    \
    (1U << HCA_SHA_CR_BUSY_Pos) /**< HCA SHA_CR: Busy bit Mask */

/* HCA AES/SHA FIFO Input */
#define HCA_FIFO_IN_DATA_Pos 0U /**< HCA FIFO_IN: Data Position */
#define HCA_FIFO_IN_DATA_Msk 0xFFFFFFFFFFFFFFFFU /**< HCA FIFO_IN: Data Mask   \
                                                  */

/* HCA AES FIFO Output */
#define HCA_AES_OUT_AES_OUT_Pos 0U /**< HCA AES_OUT: FIFO Output Position */
#define HCA_AES_OUT_AES_OUT_Msk                                                \
    0xFFFFFFFFFFFFFFFFU /**< HCA AES_OUT: FIFO Output Mask */

/* HCA AES Authentication Tag */
#define HCA_AES_AUTH_AUTH_0_Pos                                                \
    0U /**< HCA AES_AUTH: Authentication Tag 0 Position */
#define HCA_AES_AUTH_AUTH_0_Msk                                                \
    0xFFFFFFFFFFFFFFFFU /**< HCA AES_AUTH: Authentication Tag 0 Mask */

#define HCA_AES_AUTH_AUTH_1_Pos                                                \
    0U /**< HCA AES_AUTH: Authentication Tag 1 Position */
#define HCA_AES_AUTH_AUTH_1_Msk                                                \
    0xFFFFFFFFFFFFFFFFU /**< HCA AES_AUTH: Authentication Tag 1 Mask */

/* HCA HASH result from SHA */
#define HCA_HASH_HASH_0_Pos 0U /**< HCA HASH: Result form SHA 0 Position */
#define HCA_HASH_HASH_0_Msk                                                    \
    0xFFFFFFFFFFFFFFFFU /**< HCA HASH: Result form SHA 0 Mask */

#define HCA_HASH_HASH_1_Pos 0U /**< HCA HASH: Result form SHA 1 Position */
#define HCA_HASH_HASH_1_Msk                                                    \
    0xFFFFFFFFFFFFFFFFU /**< HCA HASH: Result form SHA 1 Mask */

#define HCA_HASH_HASH_2_Pos 0U /**< HCA HASH: Result form SHA 2 Position */
#define HCA_HASH_HASH_2_Msk                                                    \
    0xFFFFFFFFFFFFFFFFU /**< HCA HASH: Result form SHA 2 Mask */

#define HCA_HASH_HASH_3_Pos 0U /**< HCA HASH: Result form SHA 3 Position */
#define HCA_HASH_HASH_3_Msk                                                    \
    0xFFFFFFFFFFFFFFFFU /**< HCA HASH: Result form SHA 3 Mask */

#define HCA_HASH_HASH_4_Pos 0U /**< HCA HASH: Result form SHA 4 Position */
#define HCA_HASH_HASH_4_Msk                                                    \
    0xFFFFFFFFFFFFFFFFU /**< HCA HASH: Result form SHA 4 Mask */

#define HCA_HASH_HASH_5_Pos 0U /**< HCA HASH: Result form SHA 5 Position */
#define HCA_HASH_HASH_5_Msk                                                    \
    0xFFFFFFFFFFFFFFFFU /**< HCA HASH: Result form SHA 5 Mask */

#define HCA_HASH_HASH_6_Pos 0U /**< HCA HASH: Result form SHA 6 Position */
#define HCA_HASH_HASH_6_Msk                                                    \
    0xFFFFFFFFFFFFFFFFU /**< HCA HASH: Result form SHA 6 Mask */

#define HCA_HASH_HASH_7_Pos 0U /**< HCA HASH: Result form SHA 7 Position */
#define HCA_HASH_HASH_7_Msk                                                    \
    0xFFFFFFFFFFFFFFFFU /**< HCA HASH: Result form SHA 7 Mask */

/**
 * Structure type to access TRNG control (TRNG_CR)
 */
typedef union _HCA_TRNG_CR {
    struct {
        uint32_t HTSTART : 1;      /**< bit:      0  Health tests start */
        uint32_t RNDIRQEN : 1;     /**< bit:      1  Random IRQ enable */
        uint32_t HTIRQEN : 1;      /**< bit:      2  Health Tests IRQ enable */
        uint32_t BURSTEN : 1;      /**< bit:      3  Burst mode enable */
        uint32_t _reserved0 : 4;   /**< bit:   4..7  (reserved) */
        uint32_t STARTOSCTEST : 1; /**< bit:      8  Oscillator test start */
        uint32_t _reserved1 : 7;   /**< bit:  9..15  (reserved) */
        uint32_t OSCTESTCNT : 4;   /**< bit: 16..19  Oscillator test counter */
        uint32_t _reserved2 : 12;  /**< bit: 20..31  (reserved) */
    } b;                           /**< Structure used for bit access */
    uint32_t w;                    /**< Structure used for word access */
} HCA_TRNG_CR_Type;

/* HCA TRNG control */
#define HCA_TRNG_CR_HTSTART_Pos                                                \
    0U /**< HCA TRNG_CR: Health tests start Position */
#define HCA_TRNG_CR_HTSTART_Msk                                                \
    (1U << HCA_TRNG_CR_HTSTART_Pos) /**< HCA TRNG_CR: Health tests start Mask  \
                                     */

#define HCA_TRNG_CR_RNDIRQEN_Pos                                               \
    1U /**< HCA TRNG_CR: Random IRQ enable Position */
#define HCA_TRNG_CR_RNDIRQEN_Msk                                               \
    (1U << HCA_TRNG_CR_RNDIRQEN_Pos) /**< HCA TRNG_CR: Random IRQ enable Mask  \
                                      */

#define HCA_TRNG_CR_HTIRQEN_Pos                                                \
    2U /**< HCA TRNG_CR: Health Tests IRQ enable Position */
#define HCA_TRNG_CR_HTIRQEN_Msk                                                \
    (1U << HCA_TRNG_CR_HTIRQEN_Pos) /**< HCA TRNG_CR: Health Tests IRQ enable  \
                                       Mask */

#define HCA_TRNG_CR_BURSTEN_Pos                                                \
    3U /**< HCA TRNG_CR: Burst mode enable Position */
#define HCA_TRNG_CR_BURSTEN_Msk                                                \
    (1U << HCA_TRNG_CR_BURSTEN_Pos) /**< HCA TRNG_CR: Burst mode enable Mask   \
                                     */

#define HCA_TRNG_CR_STARTOSCTEST_Pos                                           \
    8U /**< HCA TRNG_CR: Oscillator test start Position */
#define HCA_TRNG_CR_STARTOSCTEST_Msk                                           \
    (1U << HCA_TRNG_CR_STARTOSCTEST_Pos) /**< HCA TRNG_CR: Oscillator test     \
                                            start Mask */

#define HCA_TRNG_CR_OSCTESTCNT_Pos                                             \
    16U /**< HCA TRNG_CR: Oscillator test counter Position */
#define HCA_TRNG_CR_OSCTESTCNT_Msk                                             \
    (0xFU << HCA_TRNG_CR_OSCTESTCNT_Pos) /**< HCA TRNG_CR: Oscillator test     \
                                            counter Mask */

/**
 * Structure type to access TRNG status (TRNG_SR)
 */
typedef union _HCA_TRNG_SR {
    struct {
        uint32_t RNDRDY : 1; /**< bit:      0  32-bit random data is ready to
                                read from TRNG_DATA register */
        uint32_t SRCS : 1;   /**< bit:      1  Entropy source status (0:running,
                                1:failed) */
        uint32_t
            HTR : 1; /**< bit:      2  Health test ready (0:done, 1:on-going) */
        uint32_t
            HTS : 1; /**< bit:      3  Health test status (0:pass, 1:fail) */
        uint32_t _reserved0 : 4; /**< bit:   4..7  (reserved) */
        uint32_t ADAPFAIL : 1; /**< bit:      8  Adaptative test failing bit */
        uint32_t REPFAIL : 1;  /**< bit:      9  Repetition test failing bit */
        uint32_t OSCTESTDONE : 1; /**< bit:     10  Osc Test Done */
        uint32_t _reserved1 : 21; /**< bit: 11..31  (reserved) */
    } b;                          /**< Structure used for bit access */
    uint32_t w;                   /**< Structure used for word access */
} HCA_TRNG_SR_Type;

/* HCA TRNG status */
#define HCA_TRNG_SR_RNDRDY_Pos                                                 \
    0U /**< HCA TRNG_SR: 32-bit random data is ready to read from TRNG_DATA    \
          register Position */
#define HCA_TRNG_SR_RNDRDY_Msk                                                 \
    (1U                                                                        \
     << HCA_TRNG_SR_RNDRDY_Pos) /**< HCA TRNG_SR: 32-bit random data is ready  \
                                   to read from TRNG_DATA register Mask */

#define HCA_TRNG_SR_SRCS_Pos                                                   \
    1U /**< HCA TRNG_SR: Entropy source status (0:running, 1:failed) Position  \
        */
#define HCA_TRNG_SR_SRCS_Msk                                                   \
    (1U << HCA_TRNG_SR_SRCS_Pos) /**< HCA TRNG_SR: Entropy source status       \
                                    (0:running, 1:failed) Mask */

#define HCA_TRNG_SR_HTR_Pos                                                    \
    2U /**< HCA TRNG_SR: Health test ready (0:done, 1:on-going) Position */
#define HCA_TRNG_SR_HTR_Msk                                                    \
    (1U << HCA_TRNG_SR_HTR_Pos) /**< HCA TRNG_SR: Health test ready (0:done,   \
                                   1:on-going) Mask */

#define HCA_TRNG_SR_HTS_Pos                                                    \
    3U /**< HCA TRNG_SR: Health test status (0:pass, 1:fail) Position */
#define HCA_TRNG_SR_HTS_Msk                                                    \
    (1U << HCA_TRNG_SR_HTS_Pos) /**< HCA TRNG_SR: Health test status (0:pass,  \
                                   1:fail) Mask */

#define HCA_TRNG_SR_ADAPFAIL_Pos                                               \
    8U /**< HCA TRNG_SR: Adaptative test failing bit Position */
#define HCA_TRNG_SR_ADAPFAIL_Msk                                               \
    (1U << HCA_TRNG_SR_ADAPFAIL_Pos) /**< HCA TRNG_SR: Adaptative test failing \
                                        bit Mask */

#define HCA_TRNG_SR_REPFAIL_Pos                                                \
    9U /**< HCA TRNG_SR: Repetition test failing bit Position */
#define HCA_TRNG_SR_REPFAIL_Msk                                                \
    (1U << HCA_TRNG_SR_REPFAIL_Pos) /**< HCA TRNG_SR: Repetition test failing  \
                                       bit Mask */

#define HCA_TRNG_SR_OSCTESTDONE_Pos                                            \
    10U /**< HCA TRNG_SR: Osc Test Done Position */
#define HCA_TRNG_SR_OSCTESTDONE_Msk                                            \
    (1U << HCA_TRNG_SR_OSCTESTDONE_Pos) /**< HCA TRNG_SR: Osc Test Done Mask   \
                                         */

/* HCA TRNG data */
#define HCA_TRNG_DATA_DATA_Pos 0U /**< HCA TRNG_DATA: Data register Position   \
                                   */
#define HCA_TRNG_DATA_DATA_Msk                                                 \
    0xFFFFFFFFU /**< HCA TRNG_DATA: Data register Mask */

/**
 * Structure type to access TRNG trim (TRNG_TRIM)
 */
typedef union _HCA_TRNG_TRIM {
    struct {
        uint32_t TRIM : 27;      /**< bit:  0..26  TRNG trim value */
        uint32_t _reserved0 : 4; /**< bit: 27..30  (reserved) */
        uint32_t LOCK : 1;       /**< bit:     31  TRNG trim lock (0:RW, 1:R) */
    } b;                         /**< Structure used for bit access */
    uint32_t w;                  /**< Structure used for word access */
} HCA_TRNG_TRIM_Type;

/* HCA TRNG trim */
#define HCA_TRNG_TRIM_TRIM_Pos 0U /**< HCA TRNG_TRIM: Trim value Position */
#define HCA_TRNG_TRIM_TRIM_Msk                                                 \
    (0x7FFFFFFU                                                                \
     << HCA_TRNG_TRIM_TRIM_Pos) /**< HCA TRNG_TRIM: Trim value Mask */

#define HCA_TRNG_TRIM_LOCK_Pos                                                 \
    31U /**< HCA TRNG_TRIM: Trim lock (0:RW, 1:R) Position */
#define HCA_TRNG_TRIM_LOCK_Msk                                                 \
    (1U << HCA_TRNG_TRIM_LOCK_Pos) /**< HCA TRNG_TRIM: Trim lock (0:RW, 1:R)   \
                                      Mask */

/* HCA TRNG osc1 counter */
#define HCA_TRNG_OSC1_CNT_OSC1CNT_Pos                                          \
    0U /**< HCA TRNG_OSC1_CNT: OSC1 counter register Position */
#define HCA_TRNG_OSC1_CNT_OSC1CNT_Msk                                          \
    0xFFFFFFFFU /**< HCA TRNG_OSC1_CNT: OSC1 counter register Mask */

/* HCA TRNG osc1 counter */
#define HCA_TRNG_OSC2_CNT_OSC2CNT_Pos                                          \
    0U /**< HCA TRNG_OSC2_CNT: OSC2 counter register Position */
#define HCA_TRNG_OSC2_CNT_OSC2CNT_Msk                                          \
    0xFFFFFFFFU /**< HCA TRNG_OSC2_CNT: OSC2 counter register Mask */

/* HCA TRNG osc1 snapshot */
#define HCA_TRNG_OSC1_SNAP_OSC1SNAP_Pos                                        \
    0U /**< HCA TRNG_OSC1_SNAP: OSC1 snapshot register Position */
#define HCA_TRNG_OSC1_SNAP_OSC1SNAP_Msk                                        \
    0xFFFFFFFFFFFFFFFFU /**< HCA TRNG_OSC1_SNAP: OSC1 snapshot register Mask   \
                         */

/* HCA TRNG osc2 snapshot */
#define HCA_TRNG_OSC2_SNAP_OSC2SNAP_Pos                                        \
    0U /**< HCA TRNG_OSC2_SNAP: OSC2 snapshot register Position */
#define HCA_TRNG_OSC2_SNAP_OSC2SNAP_Msk                                        \
    0xFFFFFFFFFFFFFFFFU /**< HCA TRNG_OSC2_SNAP: OSC2 snapshot register Mask   \
                         */

/**
 * Structure type to access DMA control (DMA_CR)
 */
typedef union _HCA_DMA_CR {
    struct {
        uint32_t START : 1;       /**< bit:      0  DMA Start */
        uint32_t _reserved0 : 7;  /**< bit:   1..7  (reserved) */
        uint32_t BUSY : 1;        /**< bit:      8  DMA Busy */
        uint32_t RDALIGNERR : 1;  /**< bit:      9  Read alignment error */
        uint32_t WRALIGNERR : 1;  /**< bit:     10  Write alignment error */
        uint32_t RESPERR : 1;     /**< bit:     11  DMA Response error */
        uint32_t LEGALERR : 1;    /**< bit:     12  DMA Legal error */
        uint32_t _reserved1 : 19; /**< bit: 13..31  (reserved) */
    } b;                          /**< Structure used for bit access */
    uint32_t w;                   /**< Structure used for word access */
} HCA_DMA_CR_Type;

/* HCA DMA control */
#define HCA_DMA_CR_START_Pos 0U /**< HCA DMA_CR: Start Position */
#define HCA_DMA_CR_START_Msk                                                   \
    (1U << HCA_DMA_CR_START_Pos) /**< HCA DMA_CR: Start Mask */

#define HCA_DMA_CR_BUSY_Pos 8U /**< HCA DMA_CR: Busy Position */
#define HCA_DMA_CR_BUSY_Msk                                                    \
    (1U << HCA_DMA_CR_BUSY_Pos) /**< HCA DMA_CR: Busy Mask */

#define HCA_DMA_CR_RDALIGNERR_Pos                                              \
    9U /**< HCA DMA_CR: Read alignment error Position */
#define HCA_DMA_CR_RDALIGNERR_Msk                                              \
    (1U << HCA_DMA_CR_RDALIGNERR_Pos) /**< HCA DMA_CR: Read alignment error    \
                                         Mask */

#define HCA_DMA_CR_WRALIGNERR_Pos                                              \
    10U /**< HCA DMA_CR: Write alignment error Position */
#define HCA_DMA_CR_WRALIGNERR_Msk                                              \
    (1U << HCA_DMA_CR_WRALIGNERR_Pos) /**< HCA DMA_CR: Write alignment error   \
                                         Mask */

#define HCA_DMA_CR_RESPERR_Pos 11U /**< HCA DMA_CR: Response error Position */
#define HCA_DMA_CR_RESPERR_Msk                                                 \
    (1U << HCA_DMA_CR_RESPERR_Pos) /**< HCA DMA_CR: Response error Mask */

#define HCA_DMA_CR_LEGALERR_Pos 12U /**< HCA DMA_CR: Legal error Position */
#define HCA_DMA_CR_LEGALERR_Msk                                                \
    (1U << HCA_DMA_CR_LEGALERR_Pos) /**< HCA DMA_CR: Legal error Mask */

/**
 * Structure type to access DMA length (DMA_LEN)
 */
typedef union _HCA_DMA_LEN {
    struct {
        uint32_t LEN : 28; /**< bit:  0..27  DMA Length (number of 128-bits
                              blocks) */
        uint32_t _reserved0 : 4; /**< bit: 28..31  (reserved) */
    } b;                         /**< Structure used for bit access */
    uint32_t w;                  /**< Structure used for word access */
} HCA_DMA_LEN_Type;

/* HCA DMA length */
#define HCA_DMA_LEN_LEN_Pos                                                    \
    0U /**< HCA DMA_LEN: Length (number of 128-bits blocks) Position */
#define HCA_DMA_LEN_LEN_Msk                                                    \
    0xFFFFFFFU /**< HCA DMA_LEN: Length (number of 128-bits blocks) Mask */

/* HCA DMA source address */
#define HCA_DMA_SRC_SRC_Pos 0U /**< HCA DMA_SRC: Source Address Position */
#define HCA_DMA_SRC_SRC_Msk 0xFFFFFFFFU /**< HCA DMA_SRC: Source Address Mask  \
                                         */

/* HCA DMA destination address */
#define HCA_DMA_DEST_DEST_Pos                                                  \
    0U /**< HCA DMA_DEST: Destination Address Position */
#define HCA_DMA_DEST_DEST_Msk                                                  \
    0xFFFFFFFFU /**< HCA DMA_DEST: Destination Address Mask */

/**
 * Structure type to access HCA Revision (HCA_REV)
 */
typedef union _HCA_HCA_REV {
    struct {
        uint32_t PATCHREV : 4;    /**< bit:   0..3  HCA Patch Revision */
        uint32_t MINORREV : 4;    /**< bit:   4..7  HCA Minor Revision */
        uint32_t MAJORREV : 4;    /**< bit:  8..11  HCA Major Revision */
        uint32_t _reserved0 : 20; /**< bit: 12..31  (reserved) */
    } b;                          /**< Structure used for bit access */
    uint32_t w;                   /**< Structure used for word access */
} HCA_HCA_REV_Type;

/* HCA HCA Revision */
#define HCA_HCA_REV_PATCHREV_Pos 0U /**< HCA HCA_REV: Patch Revision Position  \
                                     */
#define HCA_HCA_REV_PATCHREV_Msk                                               \
    (0xFU << HCA_HCA_REV_PATCHREV_Pos) /**< HCA HCA_REV: Patch Revision Mask   \
                                        */

#define HCA_HCA_REV_MINORREV_Pos 4U /**< HCA HCA_REV: Minor Revision Position  \
                                     */
#define HCA_HCA_REV_MINORREV_Msk                                               \
    (0xFU << HCA_HCA_REV_MINORREV_Pos) /**< HCA HCA_REV: Minor Revision Mask   \
                                        */

#define HCA_HCA_REV_MAJORREV_Pos 8U /**< HCA HCA_REV: Major Revision Position  \
                                     */
#define HCA_HCA_REV_MAJORREV_Msk                                               \
    (0xFU << HCA_HCA_REV_MAJORREV_Pos) /**< HCA HCA_REV: Major Revision Mask   \
                                        */

/**
 * Structure type to access AES Revision (AES_REV)
 */
typedef union _HCA_AES_REV {
    struct {
        uint32_t PATCHREV : 4;    /**< bit:   0..3  AES Patch Revision */
        uint32_t MINORREV : 4;    /**< bit:   4..7  AES Minor Revision */
        uint32_t MAJORREV : 4;    /**< bit:  8..11  AES Major Revision */
        uint32_t TYPE : 7;        /**< bit: 12..18  AES Type */
        uint32_t _reserved0 : 13; /**< bit: 19..31  (reserved) */
    } b;                          /**< Structure used for bit access */
    uint32_t w;                   /**< Structure used for word access */
} HCA_AES_REV_Type;

/* HCA AES Revision */
#define HCA_AES_REV_PATCHREV_Pos 0U /**< HCA AES_REV: Patch Revision Position  \
                                     */
#define HCA_AES_REV_PATCHREV_Msk                                               \
    (0xFU << HCA_AES_REV_PATCHREV_Pos) /**< HCA AES_REV: Patch Revision Mask   \
                                        */

#define HCA_AES_REV_MINORREV_Pos 4U /**< HCA AES_REV: Minor Revision Position  \
                                     */
#define HCA_AES_REV_MINORREV_Msk                                               \
    (0xFU << HCA_AES_REV_MINORREV_Pos) /**< HCA AES_REV: Minor Revision Mask   \
                                        */

#define HCA_AES_REV_MAJORREV_Pos 8U /**< HCA AES_REV: Major Revision Position  \
                                     */
#define HCA_AES_REV_MAJORREV_Msk                                               \
    (0xFU << HCA_AES_REV_MAJORREV_Pos) /**< HCA AES_REV: Major Revision Mask   \
                                        */

#define HCA_AES_REV_TYPE_Pos 12U /**< HCA AES_REV: Type Position */
#define HCA_AES_REV_TYPE_Msk                                                   \
    (0x7FU << HCA_AES_REV_TYPE_Pos) /**< HCA AES_REV: Type Mask */

/**
 * Structure type to access SHA Revision (SHA_REV)
 */
typedef union _HCA_SHA_REV {
    struct {
        uint32_t PATCHREV : 4; /**< bit:   0..3  SHA Patch Revision */
        uint32_t MINORREV : 4; /**< bit:   4..7  SHA Minor Revision */
        uint32_t MAJORREV : 4; /**< bit:  8..11  SHA Major Revision */
        uint32_t TYPE : 4;     /**< bit: 12..15  SHA Type */
        uint16_t _reserved0;   /**< bit: 16..31  (reserved) */
    } b;                       /**< Structure used for bit access */
    uint32_t w;                /**< Structure used for word access */
} HCA_SHA_REV_Type;

/* HCA SHA Revision */
#define HCA_SHA_REV_PATCHREV_Pos 0U /**< HCA SHA_REV: Patch Revision Position  \
                                     */
#define HCA_SHA_REV_PATCHREV_Msk                                               \
    (0xFU << HCA_SHA_REV_PATCHREV_Pos) /**< HCA SHA_REV: Patch Revision Mask   \
                                        */

#define HCA_SHA_REV_MINORREV_Pos 4U /**< HCA SHA_REV: Minor Revision Position  \
                                     */
#define HCA_SHA_REV_MINORREV_Msk                                               \
    (0xFU << HCA_SHA_REV_MINORREV_Pos) /**< HCA SHA_REV: Minor Revision Mask   \
                                        */

#define HCA_SHA_REV_MAJORREV_Pos 8U /**< HCA SHA_REV: Major Revision Position  \
                                     */
#define HCA_SHA_REV_MAJORREV_Msk                                               \
    (0xFU << HCA_SHA_REV_MAJORREV_Pos) /**< HCA SHA_REV: Major Revision Mask   \
                                        */

#define HCA_SHA_REV_TYPE_Pos 12U /**< HCA SHA_REV: Type Position */
#define HCA_SHA_REV_TYPE_Msk                                                   \
    (0xFU << HCA_SHA_REV_TYPE_Pos) /**< HCA SHA_REV: Type Mask */

/**
 * Structure type to access TRNG Revision (TRNG_REV)
 */
typedef union _HCA_TRNG_REV {
    struct {
        uint32_t PATCHREV : 4; /**< bit:   0..3  TRNG Patch Revision */
        uint32_t MINORREV : 4; /**< bit:   4..7  TRNG Minor Revision */
        uint32_t MAJORREV : 4; /**< bit:  8..11  TRNG Major Revision */
        uint32_t TYPE : 4;     /**< bit: 12..15  TRNG Type */
        uint16_t _reserved0;   /**< bit: 16..31  (reserved) */
    } b;                       /**< Structure used for bit access */
    uint32_t w;                /**< Structure used for word access */
} HCA_TRNG_REV_Type;

/* HCA TRNG Revision */
#define HCA_TRNG_REV_PATCHREV_Pos                                              \
    0U /**< HCA TRNG_REV: Patch Revision Position */
#define HCA_TRNG_REV_PATCHREV_Msk                                              \
    (0xFU                                                                      \
     << HCA_TRNG_REV_PATCHREV_Pos) /**< HCA TRNG_REV: Patch Revision Mask */

#define HCA_TRNG_REV_MINORREV_Pos                                              \
    4U /**< HCA TRNG_REV: Minor Revision Position */
#define HCA_TRNG_REV_MINORREV_Msk                                              \
    (0xFU                                                                      \
     << HCA_TRNG_REV_MINORREV_Pos) /**< HCA TRNG_REV: Minor Revision Mask */

#define HCA_TRNG_REV_MAJORREV_Pos                                              \
    8U /**< HCA TRNG_REV: Major Revision Position */
#define HCA_TRNG_REV_MAJORREV_Msk                                              \
    (0xFU                                                                      \
     << HCA_TRNG_REV_MAJORREV_Pos) /**< HCA TRNG_REV: Major Revision Mask */

#define HCA_TRNG_REV_TYPE_Pos 12U /**< HCA TRNG_REV: Type Position */
#define HCA_TRNG_REV_TYPE_Msk                                                  \
    (0xFU << HCA_TRNG_REV_TYPE_Pos) /**< HCA TRNG_REV: Type Mask */

/** @}*/
#endif /* SIFIVE_BLOCKS__METAL__DRIVERS__SIFIVE_HCA0_REGS_H */
