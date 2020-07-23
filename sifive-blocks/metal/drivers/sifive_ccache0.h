/* Copyright 2019 SiFive, Inc */
/* SPDX-License-Identifier: Apache-2.0 */

#ifndef METAL__DRIVERS__SIFIVE_CCACHE0_H
#define METAL__DRIVERS__SIFIVE_CCACHE0_H

#include <stdint.h>

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

#endif
