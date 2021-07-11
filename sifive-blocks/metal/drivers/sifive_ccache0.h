/* Copyright 2019 SiFive, Inc */
/* Copyright © 2020 Keith Packard */
/*
 * Licensed under the Apache License, Version 2.0 (the “License”); you
 * may not use this file except in compliance with the License. A copy
 * of this license can be found in the LICENSE.Apache2 file included
 * with this software. You may also obtain a copy of the License at
 *
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an “AS IS” BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or
 * implied. See the License for the specific language governing
 * permissions and limitations under the License.
 */
/* SPDX-License-Identifier: Apache-2.0 */

#ifndef METAL__DRIVERS__SIFIVE_CCACHE0_H
#define METAL__DRIVERS__SIFIVE_CCACHE0_H

#include <stdint.h>

/*! @brief Set of values for ECC error type */
typedef enum {
    SIFIVE_CCACHE0_DATA = 0,
    SIFIVE_CCACHE0_DIR = 1,
} sifive_ccache0_ecc_errtype_t;

/*!
 * @brief Initialize the SiFive L2 Cache, enabling the requested number of ways
 * @param ways The number of ways to enable
 */
void sifive_ccache0_init(uint32_t ways);

/*!
 * @brief Get the number of enabled ways of the SiFive L2 Cache
 * @return The number of enabled ways
 */
uint32_t sifive_ccache0_get_enabled_ways(void);

/*!
 * @brief Enable the requested number of ways of the Sifive L2 Cache
 * @param ways The number of ways to enable
 */
uint32_t sifive_ccache0_set_enabled_ways(uint32_t ways);

/*! @brief Inject ECC error into data or meta-data.
 * @param bitindex Bit index to be corrupted on next cache operation.
 * @param type ECC error target location.
 * @return None.*/
void sifive_ccache0_inject_ecc_error(uint32_t bitindex,
                                     sifive_ccache0_ecc_errtype_t type);

/*! @brief Flush out entire cache block containing given address.
 * @param flush_addr Address for the cache block to be flushed.
 * @return None.*/
void sifive_ccache0_flush(uintptr_t flush_addr);

/*! @brief Get most recently ECC corrected address.
 * @param type ECC error target location.
 * @return Last corrected ECC address.*/
uintptr_t sifive_ccache0_get_ecc_fix_addr(sifive_ccache0_ecc_errtype_t type);

/*! @brief Get number of times ECC errors were corrected.
 *         Clears related ECC interrupt signals.
 * @param type ECC error target location.
 * @return Corrected ECC error count.*/
uint32_t sifive_ccache0_get_ecc_fix_count(sifive_ccache0_ecc_errtype_t type);

/*! @brief Get address location of most recent uncorrected ECC error.
 * @param type ECC error target location.
 * @return Last uncorrected ECC address.*/
uintptr_t sifive_ccache0_get_ecc_fail_addr(sifive_ccache0_ecc_errtype_t type);

/*! @brief Get number of times ECC errors were not corrected.
 *         Clears related ECC interrupt signals.
 * @param type ECC error target location.
 * @return Uncorrected ECC error count.*/
uint32_t sifive_ccache0_get_ecc_fail_count(sifive_ccache0_ecc_errtype_t type);

/*! @brief Get currently active way enable mask value for the given master ID.
 * @param master_id Cache controller master ID.
 * @return Way enable mask. */
uint64_t sifive_ccache0_get_way_mask(uint32_t master_id);

/*! @brief Set way enable mask for the given master ID.
 * @param master_id Cache controller master ID.
 * @param waymask Specify ways to be enabled.
 * @return 0 If no error.*/
int sifive_ccache0_set_way_mask(uint32_t master_id, uint64_t waymask);

/*! @brief Select cache performance events to be counted.
 * @param counter Cache performance monitor counter index.
 * @param mask Event selection mask.
 * @return None.*/
void sifive_ccache0_set_pmevent_selector(uint32_t counter, uint64_t mask);

/*! @brief Get currently set events for the given counter index.
 * @param counter Cache performance monitor counter index.
 * @return Event selection mask.*/
uint64_t sifive_ccache0_get_pmevent_selector(uint32_t counter);

/*! @brief Clears specified cache performance counter.
 * @param counter Cache performance monitor counter index.
 * @return None.*/
void sifive_ccache0_clr_pmevent_counter(uint32_t counter);

/*! @brief Reads specified cache performance counter.
 * @param counter Cache performance monitor counter index.
 * @return Counter value.*/
uint64_t sifive_ccache0_get_pmevent_counter(uint32_t counter);

/*! @brief Select cache clients to be excluded from performance monitoring.
 * @param mask Client disable mask.
 * @return None.*/
void sifive_ccache0_set_client_filter(uint64_t mask);

/*! @brief Get currently set cache client disable mask.
 * @param None.
 * @return Client disable mask.*/
uint64_t sifive_ccache0_get_client_filter(void);

#endif
