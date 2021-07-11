/* Copyright 2018 SiFive, Inc. */
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

#include <metal/platform.h>

#ifdef METAL_FIXED_CLOCK

#include <metal/drivers/fixed_clock.h>
#include <metal/private/metal_private_fixed_clock.h>

#define get_index(clk) ((clk).__clock_index)

uint64_t fixed_clock_get_rate_hz(struct metal_clock clk) {
    return dt_clock_rate[get_index(clk)];
}

uint64_t fixed_clock_set_rate_hz(struct metal_clock clk, uint64_t target_hz) {
    return dt_clock_rate[get_index(clk)];
}

#endif /* METAL_FIXED_CLOCK */

typedef int no_empty_translation_units;
