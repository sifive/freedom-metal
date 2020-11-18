/* Copyright 2020 SiFive, Inc */
/* SPDX-License-Identifier: Apache-2.0 */

#ifndef METAL__DRIVERS__SIFIVE_GPIO0_REGS_H
#define METAL__DRIVERS__SIFIVE_GPIO0_REGS_H

#include <metal/drivers/sifive_defs.h>
#include <stdint.h>

/*!
 * @file sifive_gpio0_regs.h
 * @brief registers definition of GPIO
 */

/**
 * @addtogroup GPIO
 * @addtogroup GPIO_REGISTERS
 * @ingroup GPIO
 *  @{
 */

/* Want to keep this file as it is, even if line exceed 80 chars */
/* clang-format off */

#ifdef __cplusplus
extern "C" {
#endif

typedef struct _GPIO {
    __IM    uint32_t   INPUT_VALUE;         /**< Offset: 0x00 (R/ ) */
    __IOM   uint32_t   INPUT_EN;            /**< Offset: 0x04 (R/W) */
    __IOM   uint32_t   OUTPUT_EN;           /**< Offset: 0x08 (R/W) */
    __IOM   uint32_t   OUTPUT_VALUE;        /**< Offset: 0x0C (R/W) */
    __IOM   uint32_t   PUE;                 /**< Offset: 0x10 (R/W) */
    __IOM   uint32_t   DS;                  /**< Offset: 0x14 (R/W) */
    __IOM   uint32_t   RISE_IE;             /**< Offset: 0x18 (R/W) */
    __IOM   uint32_t   RISE_IP;             /**< Offset: 0x1C (R/W) */
    __IOM   uint32_t   FALL_IE;             /**< Offset: 0x20 (R/W) */
    __IOM   uint32_t   FALL_IP;             /**< Offset: 0x24 (R/W) */
    __IOM   uint32_t   HIGH_IE;             /**< Offset: 0x28 (R/W) */
    __IOM   uint32_t   HIGH_IP;             /**< Offset: 0x2C (R/W) */
    __IOM   uint32_t   LOW_IE;              /**< Offset: 0x30 (R/W) */
    __IOM   uint32_t   LOW_IP;              /**< Offset: 0x34 (R/W) */
    __IOM   uint32_t   IOF_EN;              /**< Offset: 0x38 (R/W) */
    __IOM   uint32_t   IOF_SEL;             /**< Offset: 0x3C (R/W) */
    __IOM   uint32_t   OUT_XOR;             /**< Offset: 0x40 (R/W) */
    __IOM   uint32_t   PASSTHRU_HIGH_IE;    /**< Offset: 0x44 (R/W) */
    __IOM   uint32_t   PASSTHRU_LOW_IE;     /**< Offset: 0x48 (R/W) */
} GPIO_Type;

/**
 * Structure type to access  (INPUT_VALUE)
 */
typedef union _GPIO_INPUT_VALUE {
    struct {
        uint16_t INPUT_VALUE;   /**< bit:  0..15  Pin value */
        uint16_t _reserved0;    /**< bit: 16..31  (reserved) */
    } b;                        /**< Structure used for bit access */
    uint32_t w;                 /**< Structure used for word access */
} GPIO_INPUT_VALUE_Type;

/* GPIO  */
#define GPIO_INPUT_VALUE_INPUT_VALUE_Pos             0U
#define GPIO_INPUT_VALUE_INPUT_VALUE_Msk             0xFFFFU

/**
 * Structure type to access  (INPUT_EN)
 */
typedef union _GPIO_INPUT_EN {
    struct {
        uint16_t INPUT_EN;      /**< bit:  0..15  Pin input enable */
        uint16_t _reserved0;    /**< bit: 16..31  (reserved) */
    } b;                        /**< Structure used for bit access */
    uint32_t w;                 /**< Structure used for word access */
} GPIO_INPUT_EN_Type;

/* GPIO  */
#define GPIO_INPUT_EN_INPUT_EN_Pos                   0U
#define GPIO_INPUT_EN_INPUT_EN_Msk                   0xFFFFU

/**
 * Structure type to access  (OUTPUT_EN)
 */
typedef union _GPIO_OUTPUT_EN {
    struct {
        uint16_t OUTPUT_EN;     /**< bit:  0..15  Pin output enable */
        uint16_t _reserved0;    /**< bit: 16..31  (reserved) */
    } b;                        /**< Structure used for bit access */
    uint32_t w;                 /**< Structure used for word access */
} GPIO_OUTPUT_EN_Type;

/* GPIO  */
#define GPIO_OUTPUT_EN_OUTPUT_EN_Pos                 0U
#define GPIO_OUTPUT_EN_OUTPUT_EN_Msk                 0xFFFFU

/**
 * Structure type to access  (OUTPUT_VALUE)
 */
typedef union _GPIO_OUTPUT_VALUE {
    struct {
        uint16_t OUTPUT_VALUE;  /**< bit:  0..15  Output value */
        uint16_t _reserved0;    /**< bit: 16..31  (reserved) */
    } b;                        /**< Structure used for bit access */
    uint32_t w;                 /**< Structure used for word access */
} GPIO_OUTPUT_VALUE_Type;

/* GPIO  */
#define GPIO_OUTPUT_VALUE_OUTPUT_VALUE_Pos           0U
#define GPIO_OUTPUT_VALUE_OUTPUT_VALUE_Msk           0xFFFFU

/**
 * Structure type to access  (PUE)
 */
typedef union _GPIO_PUE {
    struct {
        uint16_t PUE;           /**< bit:  0..15  Internal pull-up enable */
        uint16_t _reserved0;    /**< bit: 16..31  (reserved) */
    } b;                        /**< Structure used for bit access */
    uint32_t w;                 /**< Structure used for word access */
} GPIO_PUE_Type;

/* GPIO  */
#define GPIO_PUE_PUE_Pos                             0U
#define GPIO_PUE_PUE_Msk                             0xFFFFU

/**
 * Structure type to access  (DS)
 */
typedef union _GPIO_DS {
    struct {
        uint16_t DS;            /**< bit:  0..15  Pin drive strength selection */
        uint16_t _reserved0;    /**< bit: 16..31  (reserved) */
    } b;                        /**< Structure used for bit access */
    uint32_t w;                 /**< Structure used for word access */
} GPIO_DS_Type;

/* GPIO  */
#define GPIO_DS_DS_Pos                               0U
#define GPIO_DS_DS_Msk                               0xFFFFU

/**
 * Structure type to access  (RISE_IE)
 */
typedef union _GPIO_RISE_IE {
    struct {
        uint16_t RISE_IE;       /**< bit:  0..15  Rise interrupt enable */
        uint16_t _reserved0;    /**< bit: 16..31  (reserved) */
    } b;                        /**< Structure used for bit access */
    uint32_t w;                 /**< Structure used for word access */
} GPIO_RISE_IE_Type;

/* GPIO  */
#define GPIO_RISE_IE_RISE_IE_Pos                     0U
#define GPIO_RISE_IE_RISE_IE_Msk                     0xFFFFU

/**
 * Structure type to access  (RISE_IP)
 */
typedef union _GPIO_RISE_IP {
    struct {
        uint16_t RISE_IP;       /**< bit:  0..15  Rise interrupt pending */
        uint16_t _reserved0;    /**< bit: 16..31  (reserved) */
    } b;                        /**< Structure used for bit access */
    uint32_t w;                 /**< Structure used for word access */
} GPIO_RISE_IP_Type;

/* GPIO  */
#define GPIO_RISE_IP_RISE_IP_Pos                     0U
#define GPIO_RISE_IP_RISE_IP_Msk                     0xFFFFU

/**
 * Structure type to access  (FALL_IE)
 */
typedef union _GPIO_FALL_IE {
    struct {
        uint16_t FALL_IE;       /**< bit:  0..15  Fall interrupt enable */
        uint16_t _reserved0;    /**< bit: 16..31  (reserved) */
    } b;                        /**< Structure used for bit access */
    uint32_t w;                 /**< Structure used for word access */
} GPIO_FALL_IE_Type;

/* GPIO  */
#define GPIO_FALL_IE_FALL_IE_Pos                     0U
#define GPIO_FALL_IE_FALL_IE_Msk                     0xFFFFU

/**
 * Structure type to access  (FALL_IP)
 */
typedef union _GPIO_FALL_IP {
    struct {
        uint16_t FALL_IP;       /**< bit:  0..15  Fall interrupt pending */
        uint16_t _reserved0;    /**< bit: 16..31  (reserved) */
    } b;                        /**< Structure used for bit access */
    uint32_t w;                 /**< Structure used for word access */
} GPIO_FALL_IP_Type;

/* GPIO  */
#define GPIO_FALL_IP_FALL_IP_Pos                     0U
#define GPIO_FALL_IP_FALL_IP_Msk                     0xFFFFU

/**
 * Structure type to access  (HIGH_IE)
 */
typedef union _GPIO_HIGH_IE {
    struct {
        uint16_t HIGH_IE;       /**< bit:  0..15  High interrupt enable */
        uint16_t _reserved0;    /**< bit: 16..31  (reserved) */
    } b;                        /**< Structure used for bit access */
    uint32_t w;                 /**< Structure used for word access */
} GPIO_HIGH_IE_Type;

/* GPIO  */
#define GPIO_HIGH_IE_HIGH_IE_Pos                     0U
#define GPIO_HIGH_IE_HIGH_IE_Msk                     0xFFFFU

/**
 * Structure type to access  (HIGH_IP)
 */
typedef union _GPIO_HIGH_IP {
    struct {
        uint16_t HIGH_IP;       /**< bit:  0..15  High interrupt pending */
        uint16_t _reserved0;    /**< bit: 16..31  (reserved) */
    } b;                        /**< Structure used for bit access */
    uint32_t w;                 /**< Structure used for word access */
} GPIO_HIGH_IP_Type;

/* GPIO  */
#define GPIO_HIGH_IP_HIGH_IP_Pos                     0U
#define GPIO_HIGH_IP_HIGH_IP_Msk                     0xFFFFU

/**
 * Structure type to access  (LOW_IE)
 */
typedef union _GPIO_LOW_IE {
    struct {
        uint16_t LOW_IE;        /**< bit:  0..15  Low interrupt enable */
        uint16_t _reserved0;    /**< bit: 16..31  (reserved) */
    } b;                        /**< Structure used for bit access */
    uint32_t w;                 /**< Structure used for word access */
} GPIO_LOW_IE_Type;

/* GPIO  */
#define GPIO_LOW_IE_LOW_IE_Pos                       0U
#define GPIO_LOW_IE_LOW_IE_Msk                       0xFFFFU

/**
 * Structure type to access  (LOW_IP)
 */
typedef union _GPIO_LOW_IP {
    struct {
        uint16_t LOW_IP;        /**< bit:  0..15  Low interrupt pending */
        uint16_t _reserved0;    /**< bit: 16..31  (reserved) */
    } b;                        /**< Structure used for bit access */
    uint32_t w;                 /**< Structure used for word access */
} GPIO_LOW_IP_Type;

/* GPIO  */
#define GPIO_LOW_IP_LOW_IP_Pos                       0U
#define GPIO_LOW_IP_LOW_IP_Msk                       0xFFFFU

/**
 * Structure type to access  (OUT_XOR)
 */
typedef union _GPIO_OUT_XOR {
    struct {
        uint16_t OUT_XOR;       /**< bit:  0..15  Output XOR (invert) enable */
        uint16_t _reserved0;    /**< bit: 16..31  (reserved) */
    } b;                        /**< Structure used for bit access */
    uint32_t w;                 /**< Structure used for word access */
} GPIO_OUT_XOR_Type;

/* GPIO  */
#define GPIO_OUT_XOR_OUT_XOR_Pos                     0U
#define GPIO_OUT_XOR_OUT_XOR_Msk                     0xFFFFU

/**
 * Structure type to access  (PASSTHRU_HIGH_IE)
 */
typedef union _GPIO_PASSTHRU_HIGH_IE {
    struct {
        uint16_t PASSTHRU_HIGH_IE;  /**< bit:  0..15  Pass-through active-high interrupt enable */
        uint16_t _reserved0;        /**< bit: 16..31  (reserved) */
    } b;                            /**< Structure used for bit access */
    uint32_t w;                     /**< Structure used for word access */
} GPIO_PASSTHRU_HIGH_IE_Type;

/* GPIO  */
#define GPIO_PASSTHRU_HIGH_IE_PASSTHRU_HIGH_IE_Pos   0U
#define GPIO_PASSTHRU_HIGH_IE_PASSTHRU_HIGH_IE_Msk   0xFFFFU

/**
 * Structure type to access  (PASSTHRU_LOW_IE)
 */
typedef union _GPIO_PASSTHRU_LOW_IE {
    struct {
        uint16_t PASSTHRU_LOW_IE;   /**< bit:  0..15  Pass-through active-low interrupt enable */
        uint16_t _reserved0;        /**< bit: 16..31  (reserved) */
    } b;                            /**< Structure used for bit access */
    uint32_t w;                     /**< Structure used for word access */
} GPIO_PASSTHRU_LOW_IE_Type;

/* GPIO  */
#define GPIO_PASSTHRU_LOW_IE_PASSTHRU_LOW_IE_Pos     0U
#define GPIO_PASSTHRU_LOW_IE_PASSTHRU_LOW_IE_Msk     0xFFFFU

#ifdef __cplusplus
}
#endif

/** @}*/
#endif /* METAL__DRIVERS__SIFIVE_GPIO0_REGS_H */
