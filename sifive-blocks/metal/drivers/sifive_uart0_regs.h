/* Copyright 2020 SiFive, Inc */
/* SPDX-License-Identifier: Apache-2.0 */

#ifndef METAL__DRIVERS__SIFIVE_UART0_REGS_H
#define METAL__DRIVERS__SIFIVE_UART0_REGS_H

#include <metal/drivers/sifive_defs.h>
#include <stdint.h>

/*!
 * @file sifive_uart0_regs.h
 * @brief registers definition of UART
 */

/**
 * @addtogroup UART
 * @addtogroup UART_REGISTERS
 * @ingroup UART
 *  @{
 */

/* Want to keep this file as it is, even if line exceed 80 chars */
/* clang-format off */

#ifdef __cplusplus
extern "C" {
#endif


typedef struct _UART {
    __IOM   uint32_t   TXDATA;  /**< Offset: 0x00 (R/W) Transmit data */
    __IM    uint32_t   RXDATA;  /**< Offset: 0x04 (R/ ) Receive data */
    __IOM   uint32_t   TXCTRL;  /**< Offset: 0x08 (R/W) Serial transmit control */
    __IOM   uint32_t   RXCTRL;  /**< Offset: 0x0C (R/W) Serial receive control */
    __IOM   uint32_t   IE;      /**< Offset: 0x10 (R/W) Serial interrupt enable */
    __IM    uint32_t   IP;      /**< Offset: 0x14 (R/ ) Serial interrupt pending */
    __IOM   uint32_t   DIV;     /**< Offset: 0x18 (R/W) */
} UART_Type;

/**
 * Structure type to access Transmit data (TXDATA)
 */
typedef union _UART_TXDATA {
    struct {
        uint8_t  DATA;          /**< bit:   0..7  Transmit data */
        uint32_t _reserved0:23; /**< bit:  8..30  (reserved) */
        uint32_t FULL:1;        /**< bit:     31  Transmit FIFO full */
    } b;                        /**< Structure used for bit access */
    uint32_t w;                 /**< Structure used for word access */
} UART_TXDATA_Type;

/* UART Transmit data */
#define UART_TXDATA_DATA_Pos    0U
#define UART_TXDATA_DATA_Msk    0xFFU

#define UART_TXDATA_FULL_Pos    31U
#define UART_TXDATA_FULL_Msk    (1U << UART_TXDATA_FULL_Pos)

/**
 * Structure type to access Receive data (RXDATA)
 */
typedef union _UART_RXDATA {
    struct {
        uint8_t  DATA;          /**< bit:   0..7  Receive data */
        uint32_t _reserved0:23; /**< bit:  8..30  (reserved) */
        uint32_t EMPTY:1;       /**< bit:     31  Receive FIFO empty */
    } b;                        /**< Structure used for bit access */
    uint32_t w;                 /**< Structure used for word access */
} UART_RXDATA_Type;

/* UART Receive data */
#define UART_RXDATA_DATA_Pos    0U
#define UART_RXDATA_DATA_Msk    (0xFFU << UART_RXDATA_DATA_Pos)

#define UART_RXDATA_EMPTY_Pos   31U
#define UART_RXDATA_EMPTY_Msk   (1U << UART_RXDATA_EMPTY_Pos)

/**
 * Structure type to access Serial transmit control (TXCTRL)
 */
typedef union _UART_TXCTRL {
    struct {
        uint32_t TXEN:1;        /**< bit:      0  Transmit enable */
        uint32_t NSTOP:1;       /**< bit:      1  Number of stop bits */
        uint32_t _reserved0:14; /**< bit:  2..15  (reserved) */
        uint32_t TXCNT:4;       /**< bit: 16..19  Transmit watermark level */
        uint32_t _reserved1:12; /**< bit: 20..31  (reserved) */
    } b;                        /**< Structure used for bit access */
    uint32_t w;                 /**< Structure used for word access */
} UART_TXCTRL_Type;

/* UART Serial transmit control */
#define UART_TXCTRL_TXEN_Pos    0U
#define UART_TXCTRL_TXEN_Msk    1U

#define UART_TXCTRL_NSTOP_Pos   1U
#define UART_TXCTRL_NSTOP_Msk   (1U << UART_TXCTRL_NSTOP_Pos)

#define UART_TXCTRL_TXCNT_Pos   16U
#define UART_TXCTRL_TXCNT_Msk   (0xFU << UART_TXCTRL_TXCNT_Pos)

/**
 * Structure type to access Serial receive control (RXCTRL)
 */
typedef union _UART_RXCTRL {
    struct {
        uint32_t RXEN:1;        /**< bit:      0  Receive enable */
        uint32_t _reserved0:15; /**< bit:  1..15  (reserved) */
        uint32_t RXCNT:4;       /**< bit: 16..19  Receive watermark level */
        uint32_t _reserved1:12; /**< bit: 20..31  (reserved) */
    } b;                        /**< Structure used for bit access */
    uint32_t w;                 /**< Structure used for word access */
} UART_RXCTRL_Type;

/* UART  */
#define UART_RXCTRL_RXEN_Pos      0U
#define UART_RXCTRL_RXEN_Msk      1U

#define UART_RXCTRL_RXCNT_Pos     16U
#define UART_RXCTRL_RXCNT_Msk     (0xFU << UART_RXCTRL_RXCNT_Pos)

/**
 * Structure type to access Serial interrupt enable (IE)
 */
typedef union _UART_IE {
    struct {
        uint32_t TXWM_IE:1;     /**< bit:      0  Transmit watermark interrupt enable */
        uint32_t RXWM_IE:1;     /**< bit:      1  Receive watermark interrupt enable */
        uint32_t _reserved0:30; /**< bit:  2..31  (reserved) */
    } b;                        /**< Structure used for bit access */
    uint32_t w;                 /**< Structure used for word access */
} UART_IE_Type;

/* UART Serial interrupt enable */
#define UART_IE_TXWM_IE_Pos     0U
#define UART_IE_TXWM_IE_Msk     1U

#define UART_IE_RXWM_IE_Pos     1U
#define UART_IE_RXWM_IE_Msk     (1U << UART_IE_RXWM_IE_Pos)

/**
 * Structure type to access Serial interrupt pending (IP)
 */
typedef union _UART_IP {
    struct {
        uint32_t TXWM_IP:1;     /**< bit:      0  Transmit watermark interrupt pending */
        uint32_t RXWM_IP:1;     /**< bit:      1  Receive watermark interrupt pending */
        uint32_t _reserved0:30; /**< bit:  2..31  (reserved) */
    } b;                        /**< Structure used for bit access */
    uint32_t w;                 /**< Structure used for word access */
} UART_IP_Type;

/* UART Serial interrupt pending */
#define UART_IP_TXWM_IP_Pos     0U
#define UART_IP_TXWM_IP_Msk     1U

#define UART_IP_RXWM_IP_Pos     1U
#define UART_IP_RXWM_IP_Msk     (1U << UART_IP_RXWM_IP_Pos)

/**
 * Structure type to access  (DIV)
 */
typedef union _UART_DIV {
    struct {
        uint16_t DIV;           /**< bit:  0..15  Baud rate divisor */
        uint16_t _reserved0;    /**< bit: 16..31  (reserved) */
    } b;                        /**< Structure used for bit access */
    uint32_t w;                 /**< Structure used for word access */
} UART_DIV_Type;

/* UART  */
#define UART_DIV_DIV_Pos        0U
#define UART_DIV_DIV_Msk        0xFFFFU


#ifdef __cplusplus
}
#endif

/** @}*/
#endif /* METAL__DRIVERS__SIFIVE_UART0_REGS_H */
