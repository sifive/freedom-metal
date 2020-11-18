/* Copyright 2020 SiFive, Inc */
/* SPDX-License-Identifier: Apache-2.0 */

#ifndef METAL__DRIVERS__SIFIVE_SPI0_REGS_H
#define METAL__DRIVERS__SIFIVE_SPI0_REGS_H

#include <metal/drivers/sifive_defs.h>
#include <stdint.h>
/*!
 * @file sifive_spi0_regs.h
 * @brief registers definition of SPI
 */

/**
 * @addtogroup SPI
 * @addtogroup SPI_REGISTERS
 * @ingroup SPI
 *  @{
 */

/* Want to keep this file as it is, even if line exceed 80 chars */
/* clang-format off */

#ifdef __cplusplus
extern "C" {
#endif

typedef struct _SPI {
    __IOM   uint32_t   SCKDIV;          /**< Offset: 0x00 (R/W) */
    __IOM   uint32_t   SCKMODE;         /**< Offset: 0x04 (R/W) Serial clock mode */
            uint32_t   _reserved0[2U];  /**< Offset: 0x08 */
    __IOM   uint32_t   CSID;            /**< Offset: 0x10 (R/W) */
    __IOM   uint32_t   CSDEF;           /**< Offset: 0x14 (R/W) Chip select default */
    __IOM   uint32_t   CSMODE;          /**< Offset: 0x18 (R/W) */
            uint32_t   _reserved1[3U];  /**< Offset: 0x1C */
    __IOM   uint32_t   DELAY0;           /**< Offset: 0x28 (R/W) */
    __IOM   uint32_t   DELAY1;         /**< Offset: 0x2C (R/W) */
            uint32_t   _reserved2[2U];  /**< Offset: 0x30 */
    __IOM   uint32_t   EXTRADEL;        /**< Offset: 0x38 (R/W) delay from the sck edge */
    __IOM   uint32_t   SAMPLEDEL;       /**< Offset: 0x3C (R/W) Number of delay stages from slave to SPI controller */
    __IOM   uint32_t   FMT;             /**< Offset: 0x40 (R/W) Serial frame format */
            uint32_t   _reserved3;      /**< Offset: 0x44 */
    __IOM   uint32_t   TXDATA;          /**< Offset: 0x48 (R/W) Transmit data */
    __IM    uint32_t   RXDATA;          /**< Offset: 0x4C (R/ ) Receive data */
    __IOM   uint32_t   TXMARK;          /**< Offset: 0x50 (R/W) */
    __IOM   uint32_t   RXMARK;          /**< Offset: 0x54 (R/W) */
            uint32_t   _reserved4[2U];  /**< Offset: 0x58 */
    __IOM   uint32_t   FCTRL;        /**< Offset: 0x60 (R/W) */
    __IOM   uint32_t   FFMT;            /**< Offset: 0x64 (R/W) SPIFlash read instruction format */
            uint32_t   _reserved5[2U];  /**< Offset: 0x68 */
    __IOM   uint32_t   IE;              /**< Offset: 0x70 (R/W) SPI interrupt enable */
    __IM    uint32_t   IP;              /**< Offset: 0x74 (R/ ) SPI interrupt pending */
} SPI_Type;

/**
 * Structure type to access  (SCKDIV)
 */
typedef union _SPI_SCKDIV {
    struct {
        uint32_t SCKDIV:12;     /**< bit:  0..11  Serial clock divisor */
        uint32_t _reserved0:20; /**< bit: 12..31  (reserved) */
    } b;                        /**< Structure used for bit access */
    uint32_t w;                 /**< Structure used for word access */
} SPI_SCKDIV_Type;

/* SPI  */
#define SPI_SCKDIV_SCKDIV_Pos              0U
#define SPI_SCKDIV_SCKDIV_Msk              0xFFFU

/**
 * Structure type to access Serial clock mode (SCKMODE)
 */
typedef union _SPI_SCKMODE {
    struct {
        uint32_t SCKMODE_PHA:1; /**< bit:      0  Serial clock phase */
        uint32_t SCKMODE_POL:1; /**< bit:      1  Serial clock polarity */
        uint32_t _reserved0:30; /**< bit:  2..31  (reserved) */
    } b;                        /**< Structure used for bit access */
    uint32_t w;                 /**< Structure used for word access */
} SPI_SCKMODE_Type;

/* SPI Serial clock mode */
#define SPI_SCKMODE_SCKMODE_PHA_Pos        0U
#define SPI_SCKMODE_SCKMODE_PHA_Msk        1U

#define SPI_SCKMODE_SCKMODE_POL_Pos        1U
#define SPI_SCKMODE_SCKMODE_POL_Msk        (1U << SPI_SCKMODE_SCKMODE_POL_Pos)

/**
 * Structure type to access  (CSID)
 */
typedef union _SPI_CSID {
    struct {
        uint32_t CSID:1;        /**< bit:      0  Chip select id */
        uint32_t _reserved0:31; /**< bit:  1..31  (reserved) */
    } b;                        /**< Structure used for bit access */
    uint32_t w;                 /**< Structure used for word access */
} SPI_CSID_Type;

/* SPI  */
#define SPI_CSID_CSID_Pos                  0U
#define SPI_CSID_CSID_Msk                  1U

/**
 * Structure type to access Chip select default (CSDEF)
 */
typedef union _SPI_CSDEF {
    struct {
        uint32_t CSDEF:1;       /**< bit:      0  Chip select default */
        uint32_t _reserved0:31; /**< bit:  1..31  (reserved) */
    } b;                        /**< Structure used for bit access */
    uint32_t w;                 /**< Structure used for word access */
} SPI_CSDEF_Type;

/* SPI Chip select default */
#define SPI_CSDEF_CSDEF_Pos                0U
#define SPI_CSDEF_CSDEF_Msk                1U

/**
 * Structure type to access  (CSMODE)
 */
typedef union _SPI_CSMODE {
    struct {
        uint32_t CSMODE:2;      /**< bit:   0..1  Chip select mode */
        uint32_t _reserved0:30; /**< bit:  2..31  (reserved) */
    } b;                        /**< Structure used for bit access */
    uint32_t w;                 /**< Structure used for word access */
} SPI_CSMODE_Type;

/* SPI  */
#define SPI_CSMODE_CSMODE_Pos              0U
#define SPI_CSMODE_CSMODE_Msk              3U

/**
 * Structure type to access  (DELAY0)
 */
typedef union _SPI_DELAY0 {
    struct {
        uint8_t  CSSCK;         /**< bit:   0..7  CS to SCK delay */
        uint8_t  _reserved0;    /**< bit:  8..15  (reserved) */
        uint8_t  SCKCS;         /**< bit: 16..23  SCK to CS delay */
        uint8_t  _reserved1;    /**< bit: 24..31  (reserved) */
    } b;                        /**< Structure used for bit access */
    uint32_t w;                 /**< Structure used for word access */
} SPI_DELAY0_Type;

/* SPI  */
#define SPI_DELAY0_CSSCK_Pos                0U
#define SPI_DELAY0_CSSCK_Msk                0xFFU

#define SPI_DELAY0_SCKCS_Pos                16U
#define SPI_DELAY0_SCKCS_Msk                (0xFFU << SPI_DELAY0_SCKCS_Pos)

/**
 * Structure type to access  (DELAY1)
 */
typedef union _SPI_DELAY1 {
    struct {
        uint8_t  INTERCS;       /**< bit:   0..7  Minimum CS inactive time */
        uint8_t  _reserved0;    /**< bit:  8..15  (reserved) */
        uint8_t  INTERXFR;      /**< bit: 16..23  Minimum interframe delay */
        uint8_t  _reserved1;    /**< bit: 24..31  (reserved) */
    } b;                        /**< Structure used for bit access */
    uint32_t w;                 /**< Structure used for word access */
} SPI_DELAY1_Type;

/* SPI  */
#define SPI_DELAY1_INTERCS_Pos            0U
#define SPI_DELAY1_INTERCS_Msk            0xFFU

#define SPI_DELAY1_INTERXFR_Pos           16U
#define SPI_DELAY1_INTERXFR_Msk           (0xFFU << SPI_DELAY1_INTERXFR_Pos)

/**
 * Structure type to access delay from the sck edge (EXTRADEL)
 */
typedef union _SPI_EXTRADEL {
    struct {
        uint32_t EXTRADEL_COARSE:12;    /**< bit:  0..11  Coarse grain sample delay */
        uint32_t _reserved0:20;         /**< bit: 12..31  (reserved) */
    } b;                                /**< Structure used for bit access */
    uint32_t w;                         /**< Structure used for word access */
} SPI_EXTRADEL_Type;

/* SPI delay from the sck edge */
#define SPI_EXTRADEL_EXTRADEL_COARSE_Pos   0U
#define SPI_EXTRADEL_EXTRADEL_COARSE_Msk   0xFFFU

/**
 * Structure type to access Number of delay stages from slave to SPI controller (SAMPLEDEL)
 */
typedef union _SPI_SAMPLEDEL {
    struct {
        uint32_t SAMPLEDEL_SD:5;    /**< bit:   0..4  Number of delay stages from slave to the SPI controller */
        uint32_t _reserved0:27;     /**< bit:  5..31  (reserved) */
    } b;                            /**< Structure used for bit access */
    uint32_t w;                     /**< Structure used for word access */
} SPI_SAMPLEDEL_Type;

/* SPI Number of delay stages from slave to SPI controller */
#define SPI_SAMPLEDEL_SAMPLEDEL_SD_Pos     0U
#define SPI_SAMPLEDEL_SAMPLEDEL_SD_Msk     0x1FU

/**
 * Structure type to access Serial frame format (FMT)
 */
typedef union _SPI_FMT {
    struct {
        uint32_t PROTO:2;       /**< bit:   0..1  SPI Protocol */
        uint32_t ENDIAN:1;      /**< bit:      2  SPI Endianness */
        uint32_t IODIR:1;       /**< bit:      3  SPI I/O Direction */
        uint32_t _reserved0:12; /**< bit:  4..15  (reserved) */
        uint32_t LEN:4;         /**< bit: 16..19  Number of bits per frame */
        uint32_t _reserved1:12; /**< bit: 20..31  (reserved) */
    } b;                        /**< Structure used for bit access */
    uint32_t w;                 /**< Structure used for word access */
} SPI_FMT_Type;

/* SPI Serial frame format */
#define SPI_FMT_PROTO_Pos                  0U
#define SPI_FMT_PROTO_Msk                  3U

#define SPI_FMT_ENDIAN_Pos                 2U
#define SPI_FMT_ENDIAN_Msk                 (1U << SPI_FMT_ENDIAN_Pos)

#define SPI_FMT_IODIR_Pos                  3U
#define SPI_FMT_IODIR_Msk                  (1U << SPI_FMT_IODIR_Pos)

#define SPI_FMT_LEN_Pos                    16U
#define SPI_FMT_LEN_Msk                    (0xFU << SPI_FMT_LEN_Pos)

/**
 * Structure type to access Transmit data (TXDATA)
 */
typedef union _SPI_TXDATA {
    struct {
        uint8_t  DATA;          /**< bit:   0..7  Transmit data */
        uint32_t _reserved0:23; /**< bit:  8..30  (reserved) */
        uint32_t FULL:1;        /**< bit:     31  Transmit FIFO full */
    } b;                        /**< Structure used for bit access */
    uint32_t w;                 /**< Structure used for word access */
} SPI_TXDATA_Type;

/* SPI Transmit data */
#define SPI_TXDATA_DATA_Pos                0U
#define SPI_TXDATA_DATA_Msk                0xFFU

#define SPI_TXDATA_FULL_Pos                31U
#define SPI_TXDATA_FULL_Msk                (1U << SPI_TXDATA_FULL_Pos)

/**
 * Structure type to access Receive data (RXDATA)
 */
typedef union _SPI_RXDATA {
    struct {
        uint8_t  DATA;          /**< bit:   0..7  Receive data */
        uint32_t _reserved0:23; /**< bit:  8..30  (reserved) */
        uint32_t EMPTY:1;       /**< bit:     31  Receive FIFO empty */
    } b;                        /**< Structure used for bit access */
    uint32_t w;                 /**< Structure used for word access */
} SPI_RXDATA_Type;

/* SPI Receive data */
#define SPI_RXDATA_DATA_Pos                0U
#define SPI_RXDATA_DATA_Msk                0xFFU

#define SPI_RXDATA_EMPTY_Pos               31U
#define SPI_RXDATA_EMPTY_Msk               (1U << SPI_RXDATA_EMPTY_Pos)

/**
 * Structure type to access  (TXMARK)
 */
typedef union _SPI_TXMARK {
    struct {
        uint32_t TXMARK:4;      /**< bit:   0..3  Transmit watermark */
        uint32_t _reserved0:28; /**< bit:  4..31  (reserved) */
    } b;                        /**< Structure used for bit access */
    uint32_t w;                 /**< Structure used for word access */
} SPI_TXMARK_Type;

/* SPI  */
#define SPI_TXMARK_TXMARK_Pos              0U
#define SPI_TXMARK_TXMARK_Msk              0xFU

/**
 * Structure type to access  (RXMARK)
 */
typedef union _SPI_RXMARK {
    struct {
        uint32_t RXMARK:4;      /**< bit:   0..3  Receive watermark */
        uint32_t _reserved0:28; /**< bit:  4..31  (reserved) */
    } b;                        /**< Structure used for bit access */
    uint32_t w;                 /**< Structure used for word access */
} SPI_RXMARK_Type;

/* SPI  */
#define SPI_RXMARK_RXMARK_Pos              0U
#define SPI_RXMARK_RXMARK_Msk              0xFU

/**
 * Structure type to access  (FCTRL)
 */
typedef union _SPI_FCTRL {
    struct {
        uint32_t FLASH_EN:1;    /**< bit:      0  SPIFlash mode select */
        uint32_t _reserved0:31; /**< bit:  1..31  (reserved) */
    } b;                        /**< Structure used for bit access */
    uint32_t w;                 /**< Structure used for word access */
} SPI_FCTRL_Type;

/* SPI  */
#define SPI_FCTRL_FLASH_EN_Pos          0U
#define SPI_FCTRL_FLASH_EN_Msk          1U

/**
 * Structure type to access SPIFlash read instruction format (FFMT)
 */
typedef union _SPI_FFMT {
    struct {
        uint32_t CMD_EN:1;      /**< bit:      0  Enable sending of command */
        uint32_t ADDR_LEN:3;    /**< bit:   1..3  Number of address bytes */
        uint32_t PAD_CNT:4;     /**< bit:   4..7  Number of dummy cycles */
        uint32_t CMD_PROTO:2;   /**< bit:   8..9  Protocol for transmitting command */
        uint32_t ADDR_PROTO:2;  /**< bit: 10..11  Protocol for transmitting address and padding */
        uint32_t DATA_PROTO:2;  /**< bit: 12..13  Protocol for transmitting receiving data */
        uint32_t _reserved0:2;  /**< bit: 14..15  (reserved) */
        uint8_t  CMD_CODE;      /**< bit: 16..23  Value of command byte */
        uint8_t  PAD_CODE;      /**< bit: 24..31  First 8 bits to transmit during dummy cycles */
    } b;                        /**< Structure used for bit access */
    uint32_t w;                 /**< Structure used for word access */
} SPI_FFMT_Type;

/* SPI SPIFlash read instruction format */
#define SPI_FFMT_CMD_EN_Pos                0U
#define SPI_FFMT_CMD_EN_Msk                1U

#define SPI_FFMT_ADDR_LEN_Pos              1U
#define SPI_FFMT_ADDR_LEN_Msk              (7U << SPI_FFMT_ADDR_LEN_Pos)

#define SPI_FFMT_PAD_CNT_Pos               4U
#define SPI_FFMT_PAD_CNT_Msk               (0xFU << SPI_FFMT_PAD_CNT_Pos)

#define SPI_FFMT_CMD_PROTO_Pos             8U
#define SPI_FFMT_CMD_PROTO_Msk             (3U << SPI_FFMT_CMD_PROTO_Pos)

#define SPI_FFMT_ADDR_PROTO_Pos            10U
#define SPI_FFMT_ADDR_PROTO_Msk            (3U << SPI_FFMT_ADDR_PROTO_Pos)

#define SPI_FFMT_DATA_PROTO_Pos            12U
#define SPI_FFMT_DATA_PROTO_Msk            (3U << SPI_FFMT_DATA_PROTO_Pos)

#define SPI_FFMT_CMD_CODE_Pos              16U
#define SPI_FFMT_CMD_CODE_Msk              (0xFFU << SPI_FFMT_CMD_CODE_Pos)

#define SPI_FFMT_PAD_CODE_Pos              24U
#define SPI_FFMT_PAD_CODE_Msk              (0xFFU << SPI_FFMT_PAD_CODE_Pos)

/**
 * Structure type to access SPI interrupt enable (IE)
 */
typedef union _SPI_IE {
    struct {
        uint32_t TXWM_IE:1;     /**< bit:      0  Transmit watermark interrupt enable */
        uint32_t RXWM_IE:1;     /**< bit:      1  Receive watermark interrupt enable */
        uint32_t _reserved0:30; /**< bit:  2..31  (reserved) */
    } b;                        /**< Structure used for bit access */
    uint32_t w;                 /**< Structure used for word access */
} SPI_IE_Type;

/* SPI SPI interrupt enable */
#define SPI_IE_TXWM_IE_Pos                 0U
#define SPI_IE_TXWM_IE_Msk                 1U

#define SPI_IE_RXWM_IE_Pos                 1U
#define SPI_IE_RXWM_IE_Msk                 (1U << SPI_IE_RXWM_IE_Pos)

/**
 * Structure type to access SPI interrupt pending (IP)
 */
typedef union _SPI_IP {
    struct {
        uint32_t TXWM_IP:1;     /**< bit:      0  Transmit watermark interrupt pending */
        uint32_t RXWM_IP:1;     /**< bit:      1  Receive watermark interrupt pending */
        uint32_t _reserved0:30; /**< bit:  2..31  (reserved) */
    } b;                        /**< Structure used for bit access */
    uint32_t w;                 /**< Structure used for word access */
} SPI_IP_Type;

/* SPI SPI interrupt pending */
#define SPI_IP_TXWM_IP_Pos                 0U
#define SPI_IP_TXWM_IP_Msk                 1U

#define SPI_IP_RXWM_IP_Pos                 1U
#define SPI_IP_RXWM_IP_Msk                 (1U << SPI_IP_RXWM_IP_Pos)

#ifdef __cplusplus
}
#endif

/** @}*/
#endif /* METAL__DRIVERS__SIFIVE_SPI0_REGS_H */
