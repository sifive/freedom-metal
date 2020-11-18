/* Copyright 2020 SiFive, Inc */
/* SPDX-License-Identifier: Apache-2.0 */

#ifndef METAL__DRIVERS__SIFIVE_CLINT_REGS_H
#define METAL__DRIVERS__SIFIVE_CLINT_REGS_H

#include <metal/drivers/sifive_defs.h>
#include <stdint.h>

/*!
 * @file sifive_clint_regs.h
 * @brief registers definition of CLINT
 */

/**
 * @addtogroup CLINT
 * @addtogroup CLINT_REGISTERS
 * @ingroup CLINT
 *  @{
 */

/* Want to keep this file as it is, even if line exceed 80 chars */
/* clang-format off */

#ifdef __cplusplus
extern "C" {
#endif

/* Max number of cores */
#define CLINT_MAX_CORES     (4095)

typedef struct _CLINT {
    __IOM   uint32_t   MSIP[CLINT_MAX_CORES];       /**< Offset: 0x0000 (R/W) MSIP Bits */
            uint32_t   _reserved0;                  /**< Offset: 0x3FFC */
    __IOM   uint64_t   MTIMECMP[CLINT_MAX_CORES];   /**< Offset: 0x4000 (R/W) MTIMECMP */
    __IOM   uint64_t   MTIME;                       /**< Offset: 0xBFF8 (R/W) Timer */
} CLINT_Type;

/**
 * Structure type to access MSIP Bits (MSIP)
 */
typedef union _CLINT_MSIP {
    struct {
        uint32_t MSIP:1;        /**< bit:      0  MSIP bit for Hart n */
        uint32_t _reserved0:31; /**< bit:  1..31  (reserved) */
    } b;                        /**< Structure used for bit access */
    uint32_t w;                 /**< Structure used for word access */
} CLINT_MSIP_Type;

/* CLINT MSIP Bits */
#define CLINT_MSIP_MSIP_Pos           0U
#define CLINT_MSIP_MSIP_Msk           1U

/* CLINT MTIMECMP for hart n */
#define CLINT_MTIMECMP_MTIMECMP_Pos   0U
#define CLINT_MTIMECMP_MTIMECMP_Msk   0xFFFFFFFFFFFFFFFFU

/* CLINT Timer */
#define CLINT_MTIME_MTIME_Pos         0U
#define CLINT_MTIME_MTIME_Msk         0xFFFFFFFFFFFFFFFFU

#ifdef __cplusplus
}
#endif

/** @}*/
#endif /* METAL__DRIVERS__SIFIVE_CLINT_REGS_H */
