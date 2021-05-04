/**
 * Copyright 2021 SiFive, Inc
 * SPDX-License-Identifier: Apache-2.0
 *
 * HCA registers
 * @file sifive_hca1_regs.h
 * @brief registers definition of HCA revision 1.0.0
 */

#ifndef SIFIVE_HCA1_REGS_H_
#define SIFIVE_HCA1_REGS_H_

#include <stdint.h>

/* clang-format off */

/* following defines should be used for structure members */
#define __IM  volatile const    /**< Defines 'read only' structure member permissions */
#define __OM  volatile          /**< Defines 'write only' structure member permissions */
#define __IOM volatile          /**< Defines 'read / write' structure member permissions */

typedef struct _HCA {
    __IM    uint32_t   REV;               /**< Offset: 0x000 (R/ ) HCA Revision */
            uint32_t   _reserved0;
    __IOM   uint32_t   CR;                /**< Offset: 0x008 (R/W) HCA Control */
            uint32_t   _reserved1;
    __IM    uint32_t   SR;                /**< Offset: 0x010 (R/ ) HCA Status */
            uint32_t   _reserved2;
    __OM    uint32_t   FIFO_IN;           /**< Offset: 0x018 ( /W) AES/SHA/PKA FIFO Input */
            uint32_t   _reserved3[7U];
    __IM    uint32_t   FIFO_OUT;          /**< Offset: 0x038 (R/ ) FIFO Output */
            uint32_t   _reserved4[13U];
    __IOM   uint32_t   DMA_CR;            /**< Offset: 0x070 (R/W) DMA control */
            uint32_t   _reserved5;
    __IOM   uint32_t   DMA_LEN;           /**< Offset: 0x078 (R/W) DMA length */
            uint32_t   _reserved6;
    __IOM   uint64_t   DMA_SRC;           /**< Offset: 0x080 (R/W) DMA source address */
    __IOM   uint64_t   DMA_DEST;          /**< Offset: 0x088 (R/W) DMA destination address */
} HCA_Type;

/**
 * Structure type to access HCA Revision (HCA_REV)
 */
typedef union _HCA_REV {
    struct {
        uint32_t PATCHREV:4;       /**< bit:   0..3  HCA Patch Revision */
        uint32_t MINORREV:4;       /**< bit:   4..7  HCA Minor Revision */
        uint32_t MAJORREV:4;       /**< bit:  8..11  HCA Major Revision */
        uint32_t _reserved0:4;     /**< bit: 12..15  (reserved) */
        uint32_t CONFIG:5;         /**< bit: 16..20  HCA Configuration */
        uint32_t _reserved1:11;    /**< bit: 21..31  (reserved) */
    } b;                           /**< Structure used for bit access */
    uint32_t w;                    /**< Structure used for word access */
} HCA_REV_Type;


/* HCA Revision: Patch Revision */
#define HCA_REV_PATCHREV_Pos        0U
#define HCA_REV_PATCHREV_Msk        (0xFU << HCA_REV_PATCHREV_Pos)

/* HCA Revision: Minor Revision */
#define HCA_REV_MINORREV_Pos        4U
#define HCA_REV_MINORREV_Msk        (0xFU << HCA_REV_MINORREV_Pos)

/* HCA Revision: Major Revision */
#define HCA_REV_MAJORREV_Pos        8U
#define HCA_REV_MAJORREV_Msk        (0xFU << HCA_REV_MAJORREV_Pos)

/* HCA Revision: Configuration */
#define HCA_REV_CONFIG_Pos          16U
#define HCA_REV_CONFIG_Msk          (0x1FU << HCA_REV_CONFIG_Pos)

/**
 * Structure type to access HCA Control (HCA_CR)
 */
typedef union _HCA_CR {
    struct {
        uint32_t LE:1;             /**< bit:      0  Little endian enable, 0: Big Endian, 1: Little Endian */
        uint32_t INVLDFIFOS:1;     /**< bit:      1  Invalidate FIFOs */
        uint32_t _reserved0:14;    /**< bit:  2..15  (reserved) */
        uint32_t CRYPTODIE:1;      /**< bit:     16  AES/SHA/PKA completion interrupt enable */
        uint32_t OFIFOIE:1;        /**< bit:     17  Output FIFO not empty interrupt enable */
        uint32_t DMADIE:1;         /**< bit:     18  DMA completion interrupt enable */
        uint32_t DMAERRIE:1;       /**< bit:     19  DMA error interrupt enable */
        uint32_t _reserved1:4;     /**< bit: 20..23  (reserved) */
        uint32_t CRYPTODIS:1;      /**< bit:     24  AES/SHA/PKA completion interrupt status */
        uint32_t OFIFOIS:1;        /**< bit:     25  Output FIFO not empty interrupt status */
        uint32_t DMADIS:1;         /**< bit:     26  DMA completion interrupt status */
        uint32_t DMAERRIS:1;       /**< bit:     27  DMA error interrupt status */
        uint32_t _reserved2:4;     /**< bit: 28..31  (reserved) */
    } b;                           /**< Structure used for bit access */
    uint32_t w;                    /**< Structure used for word access */
} HCA_CR_Type;

/* HCA Control: Little endian enable, 0: Big Endian, 1: Little Endian */
#define HCA_CR_LE_Pos               0U
#define HCA_CR_LE_Msk               (1U << HCA_CR_LE_Pos)

/* HCA Control: Invalidate FIFOs */
#define HCA_CR_INVLDFIFOS_Pos       1U
#define HCA_CR_INVLDFIFOS_Msk       (1U << HCA_CR_INVLDFIFOS_Pos)

/* HCA Control: AES/SHA/PKA completion interrupt enable */
#define HCA_CR_CRYPTODIE_Pos        16U
#define HCA_CR_CRYPTODIE_Msk        (1U << HCA_CR_CRYPTODIE_Pos)

/* HCA Control: Output FIFO not empty interrupt enable */
#define HCA_CR_OFIFOIE_Pos          17U
#define HCA_CR_OFIFOIE_Msk          (1U << HCA_CR_OFIFOIE_Pos)

/* HCA Control: DMA completion interrupt enable */
#define HCA_CR_DMADIE_Pos           18U
#define HCA_CR_DMADIE_Msk           (1U << HCA_CR_DMADIE_Pos)

/* HCA Control: DMA error interrupt enable */
#define HCA_CR_DMAERRIE_Pos         19U
#define HCA_CR_DMAERRIE_Msk         (1U << HCA_CR_DMAERRIE_Pos)

/* HCA Control: AES/SHA/PKA completion interrupt status */
#define HCA_CR_CRYPTODIS_Pos        24U
#define HCA_CR_CRYPTODIS_Msk        (1U << HCA_CR_CRYPTODIS_Pos)

/* HCA Control: Output FIFO not empty interrupt status */
#define HCA_CR_OFIFOIS_Pos          25U
#define HCA_CR_OFIFOIS_Msk          (1U << HCA_CR_OFIFOIS_Pos)

/* HCA Control: DMA completion interrupt status */
#define HCA_CR_DMADIS_Pos           26U
#define HCA_CR_DMADIS_Msk           (1U << HCA_CR_DMADIS_Pos)

/* HCA Control: DMA error interrupt status */
#define HCA_CR_DMAERRIS_Pos         27U
#define HCA_CR_DMAERRIS_Msk         (1U << HCA_CR_DMAERRIS_Pos)

/**
 * Structure type to access HCA Status (HCA_SR)
 */
typedef union _HCA_SR {
    struct {
        uint32_t IFIFOEMPTY:1;     /**< bit:      0  Input FIFO Empty */
        uint32_t IFIFOFULL:1;      /**< bit:      1  Input FIFO Full */
        uint32_t OFIFOEMPTY:1;     /**< bit:      2  Output FIFO Empty */
        uint32_t OFIFOFULL:1;      /**< bit:      3  Output FIFO Full */
        uint32_t _reserved0:4;     /**< bit:   4..7  (reserved) */
        uint32_t IFIFOCNT:6;       /**< bit:  8..13  Input FIFO count */
        uint32_t _reserved1:2;     /**< bit: 14..15  (reserved) */
        uint32_t OFIFOCNT:6;       /**< bit: 16..21  Output FIFO count */
        uint32_t _reserved2:9;     /**< bit: 22..30  (reserved) */
        uint32_t BUSY:1;           /**< bit:     31  AES or SHA or PKA is busy */
    } b;                           /**< Structure used for bit access */
    uint32_t w;                    /**< Structure used for word access */
} HCA_SR_Type;

/* HCA Status: Input FIFO Empty */
#define HCA_SR_IFIFOEMPTY_Pos       0U
#define HCA_SR_IFIFOEMPTY_Msk       (1U << HCA_SR_IFIFOEMPTY_Pos)

/* HCA Status: Input FIFO Full */
#define HCA_SR_IFIFOFULL_Pos        1U
#define HCA_SR_IFIFOFULL_Msk        (1U << HCA_SR_IFIFOFULL_Pos)

/* HCA Status: Output FIFO Empty */
#define HCA_SR_OFIFOEMPTY_Pos       2U
#define HCA_SR_OFIFOEMPTY_Msk       (1U << HCA_SR_OFIFOEMPTY_Pos)

/* HCA Status: Output FIFO Full */
#define HCA_SR_OFIFOFULL_Pos        3U
#define HCA_SR_OFIFOFULL_Msk        (1U << HCA_SR_OFIFOFULL_Pos)

/* HCA Status: Input FIFO count */
#define HCA_SR_IFIFOCNT_Pos         8U
#define HCA_SR_IFIFOCNT_Msk         (0x3FU << HCA_SR_IFIFOCNT_Pos)

/* HCA Status: Output FIFO count */
#define HCA_SR_OFIFOCNT_Pos         16U
#define HCA_SR_OFIFOCNT_Msk         (0x3FU << HCA_SR_OFIFOCNT_Pos)

/* HCA Status: AES or SHA or PKA is busy */
#define HCA_SR_BUSY_Pos             24U
#define HCA_SR_BUSY_Msk             (1U << HCA_SR_BUSY_Pos)

/* HCA AES/SHA/PKA FIFO Input: Data */
#define HCA_FIFO_IN_DATA_Pos            0U
#define HCA_FIFO_IN_DATA_Msk            0xFFFFFFFFU

/* HCA FIFO Output: Output */
#define HCA_FIFO_OUT_DATA_Pos           0U
#define HCA_FIFO_OUT_DATA_Msk           0xFFFFFFFFU

/**
 * Structure type to access DMA control (DMA_CR)
 */
typedef union _HCA_DMA_CR {
    struct {
        uint32_t START:1;          /**< bit:      0  DMA Start */
        uint32_t _reserved0:7;     /**< bit:   1..7  (reserved) */
        uint32_t RRESPERR:1;       /**< bit:      8  DMA Read Response error */
        uint32_t WRESPERR:1;       /**< bit:      9  DMA Write Response error */
        uint32_t RLEGALERR:1;      /**< bit:     10  DMA Read Legal error */
        uint32_t WLEGALERR:1;      /**< bit:     11  DMA Write Legal error */
        uint32_t _reserved1:19;    /**< bit: 12..30  (reserved) */
        uint32_t BUSY:1;           /**< bit:     31  DMA Busy */
    } b;                           /**< Structure used for bit access */
    uint32_t w;                    /**< Structure used for word access */
} HCA_DMA_CR_Type;

/* HCA DMA control: Start */
#define HCA_DMA_CR_START_Pos            0U
#define HCA_DMA_CR_START_Msk            (1U << HCA_DMA_CR_START_Pos)

/* HCA DMA control: Read Response error */
#define HCA_DMA_CR_RRESPERR_Pos         8U
#define HCA_DMA_CR_RRESPERR_Msk         (1U << HCA_DMA_CR_RRESPERR_Pos)

/* HCA DMA control: Write Response error */
#define HCA_DMA_CR_WRESPERR_Pos         9U
#define HCA_DMA_CR_WRESPERR_Msk         (1U << HCA_DMA_CR_WRESPERR_Pos)

/* HCA DMA control: Read Legal error */
#define HCA_DMA_CR_RLEGALERR_Pos        10U
#define HCA_DMA_CR_RLEGALERR_Msk        (1U << HCA_DMA_CR_RLEGALERR_Pos)

/* HCA DMA control: Write Legal error */
#define HCA_DMA_CR_WLEGALERR_Pos        11U
#define HCA_DMA_CR_WLEGALERR_Msk        (1U << HCA_DMA_CR_WLEGALERR_Pos)

/* HCA DMA control: Busy */
#define HCA_DMA_CR_BUSY_Pos             31U
#define HCA_DMA_CR_BUSY_Msk             (1U << HCA_DMA_CR_BUSY_Pos)

/* HCA DMA length: Length (number of bytes) */
#define HCA_DMA_LEN_LENGTH_Pos          0U
#define HCA_DMA_LEN_LENGTH_Msk          0xFFFFFFFFU

/* HCA DMA source address: Source Address */
#define HCA_DMA_SRC_SRCADDR_Pos         0U
#define HCA_DMA_SRC_SRCADDR_Msk         0xFFFFFFFFU

/* HCA DMA destination address: Destination Address */
#define HCA_DMA_DEST_DESTADDR_Pos       0U
#define HCA_DMA_DEST_DESTADDR_Msk       0xFFFFFFFFU

typedef struct _HCA_AES {
    __IM    uint32_t   REV;           /**< Offset: 0x00 (R/ ) AES Revision */
            uint32_t   _reserved0;
    __IOM   uint32_t   CR;            /**< Offset: 0x08 (R/W) AES control */
            uint32_t   _reserved1;
    __OM    uint64_t   KEY[4U];       /**< Offset: 0x10 ( /W) AES Key */
            uint32_t   _reserved2[8U];
    __IOM   uint64_t   INITV[2U];     /**< Offset: 0x50 (R/W) AES Initialization Vector */
            uint32_t   _reserved3[4U];
    __IOM   uint64_t   ALEN;          /**< Offset: 0x70 (R/W) AES GCM/CCM AAD Length */
    __IOM   uint64_t   PLDLEN;        /**< Offset: 0x78 (R/W) AES GCM/CCM Payload Length */
    __IM    uint64_t   AUTH[2U];      /**< Offset: 0x80 (R/ ) AES Authentication Tag */
} HCA_AES_Type;

/**
 * Structure type to access AES Revision (AES_REV)
 */
typedef union _HCA_AES_REV {
    struct {
        uint32_t PATCHREV:4;       /**< bit:   0..3  AES Patch Revision */
        uint32_t MINORREV:4;       /**< bit:   4..7  AES Minor Revision */
        uint32_t MAJORREV:4;       /**< bit:  8..11  AES Major Revision */
        uint32_t _reserved0:4;     /**< bit: 12..15  (reserved) */
        uint32_t CONFIG:6;         /**< bit: 16..21  AES Configuration */
        uint32_t _reserved1:10;    /**< bit: 22..31  (reserved) */
    } b;                           /**< Structure used for bit access */
    uint32_t w;                    /**< Structure used for word access */
} HCA_AES_REV_Type;

/* HCA AES Revision: Patch Revision */
#define HCA_AES_REV_PATCHREV_Pos        0U
#define HCA_AES_REV_PATCHREV_Msk        (0xFU << HCA_AES_REV_PATCHREV_Pos)

/* HCA AES Revision: Minor Revision */
#define HCA_AES_REV_MINORREV_Pos        4U
#define HCA_AES_REV_MINORREV_Msk        (0xFU << HCA_AES_REV_MINORREV_Pos)

/* HCA AES Revision: Major Revision */
#define HCA_AES_REV_MAJORREV_Pos        8U
#define HCA_AES_REV_MAJORREV_Msk        (0xFU << HCA_AES_REV_MAJORREV_Pos)

/* HCA AES Revision: Configuration */
#define HCA_AES_REV_CONFIG_Pos          16U
#define HCA_AES_REV_CONFIG_Msk          (0x3FU << HCA_AES_REV_CONFIG_Pos)

/**
 * Structure type to access AES control (AES_CR)
 */
typedef union _HCA_AES_CR {
    struct {
        uint32_t EN:1;             /**< bit:      0  AES Enable */
        uint32_t MODE:3;           /**< bit:   1..3  AES mode: ECB, CBC, CFB, OFB, CTR, GCM, CCM */
        uint32_t _reserved0:2;     /**< bit:   4..5  (reserved) */
        uint32_t KEYSZ:2;          /**< bit:   6..7  AES key size */
        uint32_t _reserved1:1;     /**< bit:      8  (reserved) */
        uint32_t PROCESS:1;        /**< bit:      9  AES process type (encryption or decryption) */
        uint32_t INIT:1;           /**< bit:     10  AES Chain Initialization */
        uint32_t ABORT:1;          /**< bit:     11  Abort GCM/CCM operation */
        uint32_t CCMT:3;           /**< bit: 12..14  Encoded tag length parameter for CCM */
        uint32_t CCMQ:3;           /**< bit: 15..17  Encoded q parameter for CCM */
        uint32_t _reserved2:13;    /**< bit: 18..30  (reserved) */
        uint32_t BUSY:1;           /**< bit:     31  AES Busy bit */
    } b;                           /**< Structure used for bit access */
    uint32_t w;                    /**< Structure used for word access */
} HCA_AES_CR_Type;

/* HCA AES control: Enable */
#define HCA_AES_CR_EN_Pos               0U
#define HCA_AES_CR_EN_Msk               (1U << HCA_AES_CR_EN_Pos)

/* HCA AES control: Mode: ECB, CBC, CFB, OFB, CTR, GCM, CCM */
#define HCA_AES_CR_MODE_Pos             1U
#define HCA_AES_CR_MODE_Msk             (7U << HCA_AES_CR_MODE_Pos)

/* HCA AES control: Key size */
#define HCA_AES_CR_KEYSZ_Pos            6U
#define HCA_AES_CR_KEYSZ_Msk            (3U << HCA_AES_CR_KEYSZ_Pos)

/* HCA AES control: Process type (encryption or decryption) */
#define HCA_AES_CR_PROCESS_Pos          9U
#define HCA_AES_CR_PROCESS_Msk          (1U << HCA_AES_CR_PROCESS_Pos)

/* HCA AES control: Chain Initialization */
#define HCA_AES_CR_INIT_Pos             10U
#define HCA_AES_CR_INIT_Msk             (1U << HCA_AES_CR_INIT_Pos)

/* HCA AES control: Abort GCM/CCM operation */
#define HCA_AES_CR_ABORT_Pos            11U
#define HCA_AES_CR_ABORT_Msk            (1U << HCA_AES_CR_ABORT_Pos)

/* HCA AES control: Encoded tag length parameter for CCM */
#define HCA_AES_CR_CCMT_Pos             12U
#define HCA_AES_CR_CCMT_Msk             (7U << HCA_AES_CR_CCMT_Pos)

/* HCA AES control: Encoded q parameter for CCM */
#define HCA_AES_CR_CCMQ_Pos             15U
#define HCA_AES_CR_CCMQ_Msk             (7U << HCA_AES_CR_CCMQ_Pos)

/* HCA AES control: Busy bit */
#define HCA_AES_CR_BUSY_Pos             31U
#define HCA_AES_CR_BUSY_Msk             (1U << HCA_AES_CR_BUSY_Pos)

/* HCA AES Key: 256-bits key 0 */
#define HCA_AES_KEY_KEY_0_Pos           0U
#define HCA_AES_KEY_KEY_0_Msk           0xFFFFFFFFFFFFFFFFU

/* HCA AES Key: 256-bits key 1 */
#define HCA_AES_KEY_KEY_1_Pos           0U
#define HCA_AES_KEY_KEY_1_Msk           0xFFFFFFFFFFFFFFFFU

/* HCA AES Key: 256-bits key 2 */
#define HCA_AES_KEY_KEY_2_Pos           0U
#define HCA_AES_KEY_KEY_2_Msk           0xFFFFFFFFFFFFFFFFU

/* HCA AES Key: 256-bits key 3 */
#define HCA_AES_KEY_KEY_3_Pos           0U
#define HCA_AES_KEY_KEY_3_Msk           0xFFFFFFFFFFFFFFFFU

/* HCA AES Initialization Vector: Initialization Vector 0 */
#define HCA_AES_INITV_INITV_0_Pos       0U
#define HCA_AES_INITV_INITV_0_Msk       0xFFFFFFFFFFFFFFFFU

/* HCA AES Initialization Vector: Initialization Vector 1 */
#define HCA_AES_INITV_INITV_1_Pos       0U
#define HCA_AES_INITV_INITV_1_Msk       0xFFFFFFFFFFFFFFFFU

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
#define HCA_AES_ALEN_ALEN_Pos           0U
#define HCA_AES_ALEN_ALEN_Msk           0x1FFFFFFFFFFFFFFFU

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
#define HCA_AES_PLDLEN_PLDLEN_Pos       0U
#define HCA_AES_PLDLEN_PLDLEN_Msk       0x1FFFFFFFFFFFFFFFU

/* HCA AES Authentication Tag: Authentication Tag 0 */
#define HCA_AES_AUTH_AUTH_0_Pos         0U
#define HCA_AES_AUTH_AUTH_0_Msk         0xFFFFFFFFFFFFFFFFU

/* HCA AES Authentication Tag: Authentication Tag 1 */
#define HCA_AES_AUTH_AUTH_1_Pos         0U
#define HCA_AES_AUTH_AUTH_1_Msk         0xFFFFFFFFFFFFFFFFU

typedef struct _HCA_SHA {
    __IM    uint32_t   REV;           /**< Offset: 0x00 (R/ ) SHA Revision */
            uint32_t   _reserved14;
    __IOM   uint32_t   CR;            /**< Offset: 0x08 (R/W) SHA control */
            uint32_t   _reserved15;
    __IM    uint64_t   HASH[8U];      /**< Offset: 0x10 (R/ ) HASH result from SHA */
} HCA_SHA_Type;

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
#define HCA_SHA_REV_PATCHREV_Pos        0U
#define HCA_SHA_REV_PATCHREV_Msk        (0xFU << HCA_SHA_REV_PATCHREV_Pos)

/* HCA SHA Revision: Minor Revision */
#define HCA_SHA_REV_MINORREV_Pos        4U
#define HCA_SHA_REV_MINORREV_Msk        (0xFU << HCA_SHA_REV_MINORREV_Pos)

/* HCA SHA Revision: Major Revision */
#define HCA_SHA_REV_MAJORREV_Pos        8U
#define HCA_SHA_REV_MAJORREV_Msk        (0xFU << HCA_SHA_REV_MAJORREV_Pos)

/* HCA SHA Revision: Type */
#define HCA_SHA_REV_TYPE_Pos            12U
#define HCA_SHA_REV_TYPE_Msk            (0xFU << HCA_SHA_REV_TYPE_Pos)

/**
 * Structure type to access SHA control (SHA_CR)
 */
typedef union _HCA_SHA_CR {
    struct {
        uint32_t EN:1;             /**< bit:      0  SHA Enable */
        uint32_t MODE:2;           /**< bit:   1..2  SHA mode: 224, 256, 384, 512 */
        uint32_t _reserved0:1;     /**< bit:      3  (reserved) */
        uint32_t INIT:1;           /**< bit:      4  Initialize the SHA engine */
        uint32_t LAST:1;           /**< bit:      5  Last block flag */
        uint32_t LASTLEN:7;        /**< bit:  6..12  Last block length */
        uint32_t _reserved1:18;    /**< bit: 13..30  (reserved) */
        uint32_t BUSY:1;           /**< bit:     31  SHA Busy bit */
    } b;                           /**< Structure used for bit access */
    uint32_t w;                    /**< Structure used for word access */
} HCA_SHA_CR_Type;

/* HCA SHA control: Enable */
#define HCA_SHA_CR_EN_Pos               0U
#define HCA_SHA_CR_EN_Msk               (1U << HCA_SHA_CR_EN_Pos)

/* HCA SHA control: Mode: 224, 256, 384, 512 */
#define HCA_SHA_CR_MODE_Pos             1U
#define HCA_SHA_CR_MODE_Msk             (3U << HCA_SHA_CR_MODE_Pos)

/* HCA SHA control: Initialize the SHA engine */
#define HCA_SHA_CR_INIT_Pos             4U
#define HCA_SHA_CR_INIT_Msk             (1U << HCA_SHA_CR_INIT_Pos)

/* HCA SHA control: Last block flag */
#define HCA_SHA_CR_LAST_Pos             5U
#define HCA_SHA_CR_LAST_Msk             (1U << HCA_SHA_CR_LAST_Pos)

/* HCA SHA control: Last block length */
#define HCA_SHA_CR_LASTLEN_Pos          6U
#define HCA_SHA_CR_LASTLEN_Msk          (0x7FU << HCA_SHA_CR_LASTLEN_Pos)

/* HCA SHA control: Busy bit */
#define HCA_SHA_CR_BUSY_Pos             31U
#define HCA_SHA_CR_BUSY_Msk             (1U << HCA_SHA_CR_BUSY_Pos)

/* HCA HASH result from SHA: Result form SHA 0 */
#define HCA_SHA_HASH_HASH_0_Pos         0U
#define HCA_SHA_HASH_HASH_0_Msk         0xFFFFFFFFFFFFFFFFU

/* HCA HASH result from SHA: Result form SHA 1 */
#define HCA_SHA_HASH_HASH_1_Pos         0U
#define HCA_SHA_HASH_HASH_1_Msk         0xFFFFFFFFFFFFFFFFU

/* HCA HASH result from SHA: Result form SHA 2 */
#define HCA_SHA_HASH_HASH_2_Pos         0U
#define HCA_SHA_HASH_HASH_2_Msk         0xFFFFFFFFFFFFFFFFU

/* HCA HASH result from SHA: Result form SHA 3 */
#define HCA_SHA_HASH_HASH_3_Pos         0U
#define HCA_SHA_HASH_HASH_3_Msk         0xFFFFFFFFFFFFFFFFU

/* HCA HASH result from SHA: Result form SHA 4 */
#define HCA_SHA_HASH_HASH_4_Pos         0U
#define HCA_SHA_HASH_HASH_4_Msk         0xFFFFFFFFFFFFFFFFU

/* HCA HASH result from SHA: Result form SHA 5 */
#define HCA_SHA_HASH_HASH_5_Pos         0U
#define HCA_SHA_HASH_HASH_5_Msk         0xFFFFFFFFFFFFFFFFU

/* HCA HASH result from SHA: Result form SHA 6 */
#define HCA_SHA_HASH_HASH_6_Pos         0U
#define HCA_SHA_HASH_HASH_6_Msk         0xFFFFFFFFFFFFFFFFU

/* HCA HASH result from SHA: Result form SHA 7 */
#define HCA_SHA_HASH_HASH_7_Pos         0U
#define HCA_SHA_HASH_HASH_7_Msk         0xFFFFFFFFFFFFFFFFU

typedef struct _HCA_PKA {
    __IM    uint32_t   REV;           /**< Offset: 0x00 (R/ ) PKA Revision */
            uint32_t   _reserved0;
    __IOM   uint32_t   CR;            /**< Offset: 0x08 (R/W) PKA control cegister */
            uint32_t   _reserved1;
    __IM    uint32_t   SR;            /**< Offset: 0x10 (R/ ) PKA control */
            uint32_t   _reserved2;
    __IOM   uint64_t   OPA;           /**< Offset: 0x18 (R/W) PKA Operand A Address */
            uint32_t   _reserved3[2U];
    __IOM   uint64_t   OPB;           /**< Offset: 0x28 (R/W) PKA Operand B Address */
            uint32_t   _reserved4[2U];
    __IOM   uint64_t   RES;           /**< Offset: 0x38 (R/W) PKA Result Address */
            uint32_t   _reserved5[2U];
    __IOM   uint64_t   MOD;           /**< Offset: 0x48 (R/W) PKA Modulo Address */
} HCA_PKA_Type;

/**
 * Structure type to access PKA Revision (PKA_REV)
 */
typedef union _HCA_PKA_REV {
    struct {
        uint32_t PATCHREV:4;       /**< bit:   0..3  PKA Patch Revision */
        uint32_t MINORREV:4;       /**< bit:   4..7  PKA Minor Revision */
        uint32_t MAJORREV:4;       /**< bit:  8..11  PKA Major Revision */
        uint32_t _reserved0:4;     /**< bit: 12..15  (reserved) */
        uint32_t CONFIG:2;         /**< bit: 16..17  PKA Configuration */
        uint32_t _reserved1:14;    /**< bit: 18..31  (reserved) */
    } b;                           /**< Structure used for bit access */
    uint32_t w;                    /**< Structure used for word access */
} HCA_PKA_REV_Type;

/* HCA PKA Revision: Patch Revision */
#define HCA_PKA_REV_PATCHREV_Pos        0U
#define HCA_PKA_REV_PATCHREV_Msk        (0xFU << HCA_PKA_REV_PATCHREV_Pos)

/* HCA PKA Revision: Minor Revision */
#define HCA_PKA_REV_MINORREV_Pos        4U
#define HCA_PKA_REV_MINORREV_Msk        (0xFU << HCA_PKA_REV_MINORREV_Pos)

/* HCA PKA Revision: Major Revision */
#define HCA_PKA_REV_MAJORREV_Pos        8U
#define HCA_PKA_REV_MAJORREV_Msk        (0xFU << HCA_PKA_REV_MAJORREV_Pos)

/* HCA PKA Revision: Configuration */
#define HCA_PKA_REV_CONFIG_Pos          16U
#define HCA_PKA_REV_CONFIG_Msk          (3U << HCA_PKA_REV_CONFIG_Pos)

/**
 * Structure type to access PKA control cegister (PKA_CR)
 */
typedef union _HCA_PKA_CR {
    struct {
        uint32_t EN:1;             /**< bit:      0  PKA Enable */
        uint32_t START:1;          /**< bit:      1  Operation Start */
        uint32_t MODULOLOAD:1;     /**< bit:      2  Load Modulo */
        uint32_t SRTA:2;           /**< bit:   3..4  Store Result To ALU */
        uint32_t FOP:2;            /**< bit:   5..6  Fetch Operands */
        uint32_t NSRTM:1;          /**< bit:      7  Not Store Result To Memory  */
        uint32_t OPCODE:3;         /**< bit:  8..10  Operation Opcode */
        uint32_t _reserved0:4;     /**< bit: 11..14  (reserved) */
        uint32_t PKAIE:1;          /**< bit:     15  PKA Interrupt Enable */
        uint32_t OPW:10;           /**< bit: 16..25  Operation Width */
        uint32_t _reserved1:5;     /**< bit: 26..30  (reserved) */
        uint32_t PKAERMEM:1;       /**< bit:     31  PKA Erase Memory */
    } b;                           /**< Structure used for bit access */
    uint32_t w;                    /**< Structure used for word access */
} HCA_PKA_CR_Type;

/* HCA PKA control cegister: Enable */
#define HCA_PKA_CR_EN_Pos               0U
#define HCA_PKA_CR_EN_Msk               (1U << HCA_PKA_CR_EN_Pos)

/* HCA PKA control cegister: Operation Start */
#define HCA_PKA_CR_START_Pos            1U
#define HCA_PKA_CR_START_Msk            (1U << HCA_PKA_CR_START_Pos)

/* HCA PKA control cegister: Load Modulo */
#define HCA_PKA_CR_MODULOLOAD_Pos       2U
#define HCA_PKA_CR_MODULOLOAD_Msk       (1U << HCA_PKA_CR_MODULOLOAD_Pos)

/* HCA PKA control cegister: Store Result To ALU */
#define HCA_PKA_CR_SRTA_Pos             3U
#define HCA_PKA_CR_SRTA_Msk             (3U << HCA_PKA_CR_SRTA_Pos)

/* HCA PKA control cegister: Fetch Operands */
#define HCA_PKA_CR_FOP_Pos              5U
#define HCA_PKA_CR_FOP_Msk              (3U << HCA_PKA_CR_FOP_Pos)

/* HCA PKA control cegister: Not Store Result To Memory  */
#define HCA_PKA_CR_NSRTM_Pos            7U
#define HCA_PKA_CR_NSRTM_Msk            (1U << HCA_PKA_CR_NSRTM_Pos)

/* HCA PKA control cegister: Operation Opcode */
#define HCA_PKA_CR_OPCODE_Pos           8U
#define HCA_PKA_CR_OPCODE_Msk           (7U << HCA_PKA_CR_OPCODE_Pos)

/* HCA PKA control cegister: Interrupt Enable */
#define HCA_PKA_CR_PKAIE_Pos            15U
#define HCA_PKA_CR_PKAIE_Msk            (1U << HCA_PKA_CR_PKAIE_Pos)

/* HCA PKA control cegister: Operation Width */
#define HCA_PKA_CR_OPW_Pos              16U
#define HCA_PKA_CR_OPW_Msk              (0x3FFU << HCA_PKA_CR_OPW_Pos)

/* HCA PKA control cegister: Erase Memory */
#define HCA_PKA_CR_PKAERMEM_Pos         31U
#define HCA_PKA_CR_PKAERMEM_Msk         (1U << HCA_PKA_CR_PKAERMEM_Pos)

/**
 * Structure type to access PKA control (PKA_SR)
 */
typedef union _HCA_PKA_SR {
    struct {
        uint32_t BUSY:1;           /**< bit:      0  PKA Busy */
        uint32_t ALUNEG:2;         /**< bit:   1..2  PKA ALU1/2 SIGN */
        uint32_t _reserved0:29;    /**< bit:  3..31  (reserved) */
    } b;                           /**< Structure used for bit access */
    uint32_t w;                    /**< Structure used for word access */
} HCA_PKA_SR_Type;

/* HCA PKA control: Busy */
#define HCA_PKA_SR_BUSY_Pos             0U
#define HCA_PKA_SR_BUSY_Msk             (1U << HCA_PKA_SR_BUSY_Pos)

/* HCA PKA control: ALU1/2 SIGN */
#define HCA_PKA_SR_ALUNEG_Pos           1U
#define HCA_PKA_SR_ALUNEG_Msk           (3U << HCA_PKA_SR_ALUNEG_Pos)

/* HCA PKA Operand A Address: Operand A Address */
#define HCA_PKA_OPA_OPA_Pos             0U
#define HCA_PKA_OPA_OPA_Msk             0xFFFFFFFFU

/* HCA PKA Operand B Address: Operand B Address */
#define HCA_PKA_OPB_OPB_Pos             0U
#define HCA_PKA_OPB_OPB_Msk             0xFFFFFFFFU

/* HCA PKA Result Address: Operation Result Address */
#define HCA_PKA_RES_RES_Pos             0U
#define HCA_PKA_RES_RES_Msk             0xFFFFFFFFU

/* HCA PKA Modulo Address: MODULO Address */
#define HCA_PKA_MOD_MOD_Pos             0U
#define HCA_PKA_MOD_MOD_Msk             0xFFFFFFFFU

typedef struct _HCA_TRNG {
    __IM    uint32_t   REV;          /**< Offset: 0x00 (R/ ) TRNG Revision */
            uint32_t   _reserved0;
    __IOM   uint32_t   CR;           /**< Offset: 0x08 (R/W) TRNG control */
            uint32_t   _reserved1;
    __IOM   uint32_t   SR;           /**< Offset: 0x10 (R/W) TRNG status */
            uint32_t   _reserved2;
    __IM    uint32_t   DATA;         /**< Offset: 0x18 (R/ ) TRNG data */
            uint32_t   _reserved3;
    __IOM   uint32_t   TRIM;         /**< Offset: 0x20 (R/W) TRNG trim */
            uint32_t   _reserved4;
    __IM    uint32_t   OSC1_CNT;     /**< Offset: 0x28 (R/ ) TRNG osc1 counter */
            uint32_t   _reserved5;
    __IM    uint32_t   OSC2_CNT;     /**< Offset: 0x30 (R/ ) TRNG osc1 counter */
} HCA_TRNG_Type;

/**
 * Structure type to access TRNG Revision (TRNG_REV)
 */
typedef union _HCA_TRNG_REV {
    struct {
        uint32_t PATCHREV:4;       /**< bit:   0..3  TRNG Patch Revision */
        uint32_t MINORREV:4;       /**< bit:   4..7  TRNG Minor Revision */
        uint32_t MAJORREV:4;       /**< bit:  8..11  TRNG Major Revision */
        uint32_t _reserved0:20;    /**< bit: 12..31  (reserved) */
    } b;                           /**< Structure used for bit access */
    uint32_t w;                    /**< Structure used for word access */
} HCA_TRNG_REV_Type;

/* HCA TRNG Revision: Patch Revision */
#define HCA_TRNG_REV_PATCHREV_Pos       0U
#define HCA_TRNG_REV_PATCHREV_Msk       (0xFU << HCA_TRNG_REV_PATCHREV_Pos)

/* HCA TRNG Revision: Minor Revision */
#define HCA_TRNG_REV_MINORREV_Pos       4U
#define HCA_TRNG_REV_MINORREV_Msk       (0xFU << HCA_TRNG_REV_MINORREV_Pos)

/* HCA TRNG Revision: Major Revision */
#define HCA_TRNG_REV_MAJORREV_Pos       8U
#define HCA_TRNG_REV_MAJORREV_Msk       (0xFU << HCA_TRNG_REV_MAJORREV_Pos)

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
        uint32_t TESTOUTSEL:2;     /**< bit:  9..10  Test Oscillator in free running mode */
        uint32_t _reserved1:5;     /**< bit: 11..15  (reserved) */
        uint32_t OSCTESTCNT:5;     /**< bit: 16..20  Oscillator test counter */
        uint32_t _reserved2:11;    /**< bit: 21..31  (reserved) */
    } b;                           /**< Structure used for bit access */
    uint32_t w;                    /**< Structure used for word access */
} HCA_TRNG_CR_Type;

/* HCA TRNG control: Health tests start */
#define HCA_TRNG_CR_HTSTART_Pos         0U
#define HCA_TRNG_CR_HTSTART_Msk         (1U << HCA_TRNG_CR_HTSTART_Pos)

/* HCA TRNG control: Random IRQ enable */
#define HCA_TRNG_CR_RNDIRQEN_Pos        1U
#define HCA_TRNG_CR_RNDIRQEN_Msk        (1U << HCA_TRNG_CR_RNDIRQEN_Pos)

/* HCA TRNG control: Health Tests IRQ enable */
#define HCA_TRNG_CR_HTIRQEN_Pos         2U
#define HCA_TRNG_CR_HTIRQEN_Msk         (1U << HCA_TRNG_CR_HTIRQEN_Pos)

/* HCA TRNG control: Burst mode enable */
#define HCA_TRNG_CR_BURSTEN_Pos         3U
#define HCA_TRNG_CR_BURSTEN_Msk         (1U << HCA_TRNG_CR_BURSTEN_Pos)

/* HCA TRNG control: Oscillator test start */
#define HCA_TRNG_CR_STARTOSCTEST_Pos    8U
#define HCA_TRNG_CR_STARTOSCTEST_Msk    (1U << HCA_TRNG_CR_STARTOSCTEST_Pos)

/* HCA TRNG control: Test Oscillator in free running mode */
#define HCA_TRNG_CR_TESTOUTSEL_Pos      9U
#define HCA_TRNG_CR_TESTOUTSEL_Msk      (3U << HCA_TRNG_CR_TESTOUTSEL_Pos)

/* HCA TRNG control: Oscillator test counter */
#define HCA_TRNG_CR_OSCTESTCNT_Pos      16U
#define HCA_TRNG_CR_OSCTESTCNT_Msk      (0x1FU << HCA_TRNG_CR_OSCTESTCNT_Pos)

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
#define HCA_TRNG_SR_RNDRDY_Pos          0U
#define HCA_TRNG_SR_RNDRDY_Msk          (1U << HCA_TRNG_SR_RNDRDY_Pos)

/* HCA TRNG status: Entropy source status (0:running, 1:failed) */
#define HCA_TRNG_SR_SRCS_Pos            1U
#define HCA_TRNG_SR_SRCS_Msk            (1U << HCA_TRNG_SR_SRCS_Pos)

/* HCA TRNG status: Health test ready (0:done, 1:on-going) */
#define HCA_TRNG_SR_HTR_Pos             2U
#define HCA_TRNG_SR_HTR_Msk             (1U << HCA_TRNG_SR_HTR_Pos)

/* HCA TRNG status: Health test status (0:pass, 1:fail) */
#define HCA_TRNG_SR_HTS_Pos             3U
#define HCA_TRNG_SR_HTS_Msk             (1U << HCA_TRNG_SR_HTS_Pos)

/* HCA TRNG status: Adaptative test failing bit */
#define HCA_TRNG_SR_ADAPFAIL_Pos        8U
#define HCA_TRNG_SR_ADAPFAIL_Msk        (1U << HCA_TRNG_SR_ADAPFAIL_Pos)

/* HCA TRNG status: Repetition test failing bit */
#define HCA_TRNG_SR_REPFAIL_Pos         9U
#define HCA_TRNG_SR_REPFAIL_Msk         (1U << HCA_TRNG_SR_REPFAIL_Pos)

/* HCA TRNG status: Osc Test Done */
#define HCA_TRNG_SR_OSCTESTDONE_Pos     10U
#define HCA_TRNG_SR_OSCTESTDONE_Msk     (1U << HCA_TRNG_SR_OSCTESTDONE_Pos)

/* HCA TRNG data: Data register */
#define HCA_TRNG_DATA_DATA_Pos          0U
#define HCA_TRNG_DATA_DATA_Msk          0xFFFFFFFFU

/**
 * Structure type to access TRNG trim (TRNG_TRIM)
 */
typedef union _HCA_TRNG_TRIM {
    struct {
        uint32_t TRIM:28;          /**< bit:  0..27  TRNG trim value */
        uint32_t _reserved0:3;     /**< bit: 28..30  (reserved) */
        uint32_t LOCK:1;           /**< bit:     31  TRNG trim lock (0:RW, 1:R) */
    } b;                           /**< Structure used for bit access */
    uint32_t w;                    /**< Structure used for word access */
} HCA_TRNG_TRIM_Type;

/* HCA TRNG trim: Trim value */
#define HCA_TRNG_TRIM_TRIM_Pos          0U
#define HCA_TRNG_TRIM_TRIM_Msk          (0xFFFFFFFU << HCA_TRNG_TRIM_TRIM_Pos)

/* HCA TRNG trim: Trim lock (0:RW, 1:R) */
#define HCA_TRNG_TRIM_LOCK_Pos          31U
#define HCA_TRNG_TRIM_LOCK_Msk          (1U << HCA_TRNG_TRIM_LOCK_Pos)

/* HCA TRNG osc1 counter: OSC1 counter register */
#define HCA_TRNG_OSC1_CNT_OSC1CNT_Pos   0U
#define HCA_TRNG_OSC1_CNT_OSC1CNT_Msk   0xFFFFFFFFU

/* HCA TRNG osc1 counter: OSC2 counter register */
#define HCA_TRNG_OSC2_CNT_OSC2CNT_Pos   0U
#define HCA_TRNG_OSC2_CNT_OSC2CNT_Msk   0xFFFFFFFFU


#endif /* SIFIVE_HCA1_REGS_H_ */
