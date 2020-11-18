/* Copyright 2020 SiFive, Inc */
/* SPDX-License-Identifier: Apache-2.0 */

#ifndef METAL__DRIVERS__SIFIVE_PWM0_REGS_H
#define METAL__DRIVERS__SIFIVE_PWM0_REGS_H

#include <metal/drivers/sifive_defs.h>
#include <stdint.h>

/*!
 * @file sifive_pwm0_regs.h
 * @brief registers definition of PWM
 */

/**
 * @addtogroup PWM
 * @addtogroup PWM_REGISTERS
 * @ingroup PWM
 *  @{
 */

/* Want to keep this file as it is, even if line exceed 80 chars */
/* clang-format off */

#ifdef __cplusplus
extern "C" {
#endif

typedef struct _PWM {
    __IOM   uint32_t   PWMCFG;          /**< Offset: 0x00 (R/W) pwm Configuration */
            uint32_t   _reserved0;      /**< Offset: 0x04 */
    __IOM   uint32_t   PWMCOUNT;        /**< Offset: 0x08 (R/W) */
            uint32_t   _reserved1;      /**< Offset: 0x0C */
    __IM    uint32_t   PWMS;            /**< Offset: 0x10 (R/ ) */
            uint32_t   _reserved2[3U];  /**< Offset: 0x14 */
    __IOM   uint32_t   PWMCMP[4U];      /**< Offset: 0x20 (R/W) */
} PWM_Type;

/**
 * Structure type to access pwm Configuration (PWMCFG)
 */
typedef union _PWM_PWMCFG {
    struct {
        uint32_t PWMSCALE:4;        /**< bit:   0..3  Counter scale value. */
        uint32_t _reserved0:4;      /**< bit:   4..7  (reserved) */
        uint32_t PWMSTICKY:1;       /**< bit:      8  Sticky. Disallow clearing of ${prefix}cmpXip bits */
        uint32_t PWMZEROCMP:1;      /**< bit:      9  Reset counter to zero after match. */
        uint32_t PWMDEGLITCH:1;     /**< bit:     10  Deglitch - latch ${prefix}cmpXip within same cycle. */
        uint32_t _reserved1:1;      /**< bit:     11  (reserved) */
        uint32_t PWMENALWAYS:1;     /**< bit:     12  Enable Always - run continuously */
        uint32_t PWMONESHOT:1;      /**< bit:     13  Enable One Shot - run one cycle, then this bit is cleared. */
        uint32_t _reserved2:2;      /**< bit: 14..15  (reserved) */
        uint32_t PWMCMPNCENTER:4;   /**< bit: 16..19  Comparator n Center */
        uint32_t PWMINVERTN:4;      /**< bit: 20..23  Invert Comparator 0 Output */
        uint32_t PWMGANGN:4;        /**< bit: 24..27  Comparator N/(N+1) Gang */
        uint32_t PWMIPN:4;          /**< bit: 28..31  Interrupt N Pending */
    } b;                            /**< Structure used for bit access */
    uint32_t w;                     /**< Structure used for word access */
} PWM_PWMCFG_Type;

/* PWM pwm Configuration */
#define PWM_PWMCFG_PWMSCALE_Pos        0U
#define PWM_PWMCFG_PWMSCALE_Msk        0xFU

#define PWM_PWMCFG_PWMSTICKY_Pos       8U
#define PWM_PWMCFG_PWMSTICKY_Msk       (1U << PWM_PWMCFG_PWMSTICKY_Pos)

#define PWM_PWMCFG_PWMZEROCMP_Pos      9U
#define PWM_PWMCFG_PWMZEROCMP_Msk      (1U << PWM_PWMCFG_PWMZEROCMP_Pos)

#define PWM_PWMCFG_PWMDEGLITCH_Pos     10U
#define PWM_PWMCFG_PWMDEGLITCH_Msk     (1U << PWM_PWMCFG_PWMDEGLITCH_Pos)

#define PWM_PWMCFG_PWMENALWAYS_Pos     12U
#define PWM_PWMCFG_PWMENALWAYS_Msk     (1U << PWM_PWMCFG_PWMENALWAYS_Pos)

#define PWM_PWMCFG_PWMONESHOT_Pos      13U
#define PWM_PWMCFG_PWMONESHOT_Msk      (1U << PWM_PWMCFG_PWMONESHOT_Pos)

#define PWM_PWMCFG_PWMCMPNCENTER_Pos   16U
#define PWM_PWMCFG_PWMCMPNCENTER_Msk   (0xFU << PWM_PWMCFG_PWMCMPNCENTER_Pos)

#define PWM_PWMCFG_PWMINVERTN_Pos      20U
#define PWM_PWMCFG_PWMINVERTN_Msk      (0xFU << PWM_PWMCFG_PWMINVERTN_Pos)

#define PWM_PWMCFG_PWMGANGN_Pos        24U
#define PWM_PWMCFG_PWMGANGN_Msk        (0xFU << PWM_PWMCFG_PWMGANGN_Pos)

#define PWM_PWMCFG_PWMIPN_Pos          28U
#define PWM_PWMCFG_PWMIPN_Msk          (0xFU << PWM_PWMCFG_PWMIPN_Pos)

/**
 * Structure type to access  (PWMCOUNT)
 */
typedef union _PWM_PWMCOUNT {
    struct {
        uint32_t PWMCOUNT:23;   /**< bit:  0..22  Counter Register */
        uint32_t _reserved0:9;  /**< bit: 23..31  (reserved) */
    } b;                        /**< Structure used for bit access */
    uint32_t w;                 /**< Structure used for word access */
} PWM_PWMCOUNT_Type;

/* PWM  */
#define PWM_PWMCOUNT_PWMCOUNT_Pos      0U
#define PWM_PWMCOUNT_PWMCOUNT_Msk      0x7FFFFFU

/**
 * Structure type to access  (PWMS)
 */
typedef union _PWM_PWMS {
    struct {
        uint8_t  PWMS;          /**< bit:   0..7  Scaled value of Counter */
        uint32_t _reserved0:24; /**< bit:  8..31  (reserved) */
    } b;                        /**< Structure used for bit access */
    uint32_t w;                 /**< Structure used for word access */
} PWM_PWMS_Type;

/* PWM  */
#define PWM_PWMS_PWMS_Pos              0U
#define PWM_PWMS_PWMS_Msk              0xFFU

/**
 * Structure type to access  (PWMCMP)
 */
typedef union _PWM_PWMCMP {
    struct {
        uint8_t  PWMCMP;       /**< bit:   0..7  Comparator 0 */
        uint32_t _reserved0:24; /**< bit:  8..31  (reserved) */
    } b;                        /**< Structure used for bit access */
    uint32_t w;                 /**< Structure used for word access */
} PWM_PWMCMP_Type;

/* PWM  */
#define PWM_PWMCMP_PWMCMP_Pos        0U
#define PWM_PWMCMP_PWMCMP_Msk        0xFFU

#ifdef __cplusplus
}
#endif

/** @}*/
#endif /* METAL__DRIVERS__SIFIVE_PWM0_REGS_H */
