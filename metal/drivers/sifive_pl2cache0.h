/* Copyright 2021 SiFive, Inc */
/* SPDX-License-Identifier: Apache-2.0 */

#ifndef METAL__DRIVERS__SIFIVE_PL2CACHE0_H
#define METAL__DRIVERS__SIFIVE_PL2CACHE0_H

/*!
 * @file sifive_pl2cache0.h
 *
 * @brief API for configuring the SiFive private L2 cache controller
 */

#include <metal/interrupt.h>
#include <stdbool.h>
#include <stdint.h>

/*! @brief Cache configuration data */
typedef struct {
    uint32_t num_bank;
    uint32_t num_ways;
    uint32_t num_sets;
    uint32_t block_size;
} sifive_pl2cache0_config;

typedef union _sifive_pl2cache0_configbits {
    struct {
        uint32_t _reserved0 : 3;
        uint32_t cleanEvictEnable : 1;
        uint32_t _reserved1 : 5;
        uint32_t l2AvoidL1LineDisable : 1;
        uint32_t _reserved2 : 6;
        uint32_t softwareEccInjectEnable : 1;
        uint32_t errInjectOnWriteEnable : 1;
        uint32_t errInjectOnReadEnable : 1;
        uint32_t dataUceInjectEnable : 1;
        uint32_t dirUceInjectEnable : 1;
        uint32_t _reserved3 : 11;
    } b;        /**< Structure used for bit access */
    uint32_t w; /**< Structure used for 32bits access */
} sifive_pl2cache0_configbits;

/*! @brief Set of values for ECC error target */
typedef enum {
    SIFIVE_PL2CACHE0_ECC_ERROR_DATA = 0,
    SIFIVE_PL2CACHE0_ECC_ERROR_DIR = 1,
    SIFIVE_PL2CACHE0_ECC_ERROR_LRU = 2,
} sifive_pl2cache0_ecc_errtarget_t;

/*! @brief Set of values for ECC error direction */
typedef enum {
    SIFIVE_PL2CACHE0_ECC_ERROR_READ = 0,
    SIFIVE_PL2CACHE0_ECC_ERROR_WRITE = 1,
} sifive_pl2cache0_ecc_errdirection_t;

/*! @brief Set of values for ECC error type */
typedef enum {
    SIFIVE_PL2CACHE0_ECC_ERROR_CORRECTABLE = 0,
    SIFIVE_PL2CACHE0_ECC_ERROR_UNCORRECTABLE = 1,
} sifive_pl2cache0_ecc_errtype_t;

/*! @brief Set theCleanEvicenable bit of the Private L2 cache controller.
 * @param val boolean parameter true(enable) or false(desable).
 * @return None.*/
void sifive_pl2cache0_set_cleanEvictenale_bit(bool val);

/*! @brief Initialize Private L2 cache controller.
 * @param None.
 * @return None.*/
void sifive_pl2cache0_init(void);

/*! @brief Get cache configuration data.
 * @param config User specified data buffer.
 * @return None.*/
void sifive_pl2cache0_get_config(sifive_pl2cache0_config *config);

/*! @brief Get currently active cache ways.
 * @param None.
 * @return Number of cache ways enabled.*/
uint32_t sifive_pl2cache0_get_enabled_ways(void);

/*! @brief Enable specified cache ways.
 * @param ways Number of ways to be enabled.
 * @return 0 If no error.*/
int sifive_pl2cache0_set_enabled_ways(uint32_t ways);

/*! @brief Inject ECC error into data or meta-data.
 * @param bitindex Bit index to be corrupted on next cache operation.
 * @param target ECC error target location.
 * @param dir ECC error direction.
 * @param type ECC error target type.
 * @return None.*/
void sifive_pl2cache0_inject_ecc_error(uint32_t bitindex,
                                       sifive_pl2cache0_ecc_errtarget_t target,
                                       sifive_pl2cache0_ecc_errdirection_t dir,
                                       sifive_pl2cache0_ecc_errtype_t type);

/*! @brief Flush out entire cache block containing given address.
 * @param flush_addr Address for the cache block to be flushed.
 * @return None.*/
void sifive_pl2cache0_flush(uintptr_t flush_addr);

/*! @brief Get the number of outstanding cache flush requests.
 * @return Outstanding flush requests .*/
uint32_t sifive_pl2cache0_get_flush_count(void);

/*! @brief Select cache performance events to be counted.
 * @param counter Cache performance monitor counter index.
 * @param mask Event selection mask.
 * @return None.*/
void sifive_pl2cache0_set_pmevent_selector(uint32_t counter, uint64_t mask);

/*! @brief Get currently set events for the given counter index.
 * @param counter Cache performance monitor counter index.
 * @return Event selection mask.*/
uint64_t sifive_pl2cache0_get_pmevent_selector(uint32_t counter);

/*! @brief Clears specified cache performance counter.
 * @param counter Cache performance monitor counter index.
 * @return None.*/
void sifive_pl2cache0_clr_pmevent_counter(uint32_t counter);

/*! @brief Reads specified cache performance counter.
 * @param counter Cache performance monitor counter index.
 * @return Counter value.*/
uint64_t sifive_pl2cache0_get_pmevent_counter(uint32_t counter);

/*! @brief Select cache clients to be excluded from performance monitoring.
 * @param mask Client disable mask.
 * @return None.*/
void sifive_pl2cache0_set_client_filter(uint64_t mask);

/*! @brief Get currently set cache client disable mask.
 * @param None.
 * @return Client disable mask.*/
uint64_t sifive_pl2cache0_get_client_filter(void);

#endif
