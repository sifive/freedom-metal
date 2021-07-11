/* Copyright 2018 SiFive, Inc */
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

#ifndef METAL__DRIVERS__SIFIVE_FE310_G000_HFXOSC_H
#define METAL__DRIVERS__SIFIVE_FE310_G000_HFXOSC_H

#include <metal/clock.h>
#include <stdint.h>

uint64_t sifive_fe310_g000_hfxosc_get_rate_hz(struct metal_clock clock);

uint64_t sifive_fe310_g000_hfxosc_set_rate_hz(struct metal_clock clock,
                                              uint64_t rate);

#endif
