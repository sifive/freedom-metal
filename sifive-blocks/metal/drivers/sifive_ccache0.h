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
