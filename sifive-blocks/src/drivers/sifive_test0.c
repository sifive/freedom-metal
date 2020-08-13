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

#include <metal/platform.h>

#ifdef METAL_SIFIVE_TEST0

#include <metal/io.h>
#include <stdint.h>

void metal_shutdown(int code) __attribute__((noreturn));
void metal_shutdown(int code) {
    uintptr_t base = METAL_SIFIVE_TEST0_0_BASE_ADDRESS;
    uint32_t out = (code << 16) + (code == 0 ? 0x5555 : 0x3333);
    while (1) {
        __METAL_ACCESS_ONCE((
            __metal_io_u32 *)(base + METAL_SIFIVE_TEST0_FINISHER_OFFSET)) = out;
    }
}

#endif /* METAL_SIFIVE_TEST0 */

typedef int no_empty_translation_units;
