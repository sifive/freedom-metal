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

#ifdef METAL_SIFIVE_FE310_G000_HFROSC

#include <metal/drivers/sifive_fe310_g000_hfrosc.h>
#include <metal/io.h>
#include <metal/private/metal_private_sifive_fe310_g000_hfrosc.h>

#ifndef METAL_SIFIVE_FE310_G000_PRCI
#error No SiFive FE310-G000 PRCI available.
#endif

#define PRCI_HFROSCCFG_ENABLE (1 << 30)
#define PRCI_HFROSCCFG_READY (1 << 31)

#define PRCI_REGW(offset)                                                      \
    __METAL_ACCESS_ONCE(                                                       \
        (__metal_io_u32 *)(METAL_SIFIVE_FE310_G000_PRCI_0_BASE_ADDRESS +       \
                           (offset)))

uint64_t sifive_fe310_g000_hfrosc_get_rate_hz(struct metal_clock clock) {

    if (!(PRCI_REGW(METAL_SIFIVE_FE310_G000_PRCI_HFROSCCFG) &
          PRCI_HFROSCCFG_ENABLE)) {
        return 0;
    }
    while (!(PRCI_REGW(METAL_SIFIVE_FE310_G000_PRCI_HFROSCCFG) &
             PRCI_HFROSCCFG_READY))
        ;

    struct metal_clock ref = REF_CLOCK(clock);
    return metal_clock_get_rate_hz(ref);
}

uint64_t sifive_fe310_g000_hfrosc_set_rate_hz(struct metal_clock clock,
                                              uint64_t rate) {
    return sifive_fe310_g000_hfrosc_get_rate_hz(clock);
}

#endif /* METAL_SIFIVE_FE310_G000_HFROSC */

typedef int no_empty_translation_units;
