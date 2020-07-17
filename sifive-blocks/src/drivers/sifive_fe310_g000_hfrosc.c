/* Copyright 2018 SiFive, Inc */
/* SPDX-License-Identifier: Apache-2.0 */

#include <metal/machine/platform.h>

#ifdef METAL_SIFIVE_FE310_G000_HFROSC

#include <metal/drivers/sifive_fe310_g000_hfrosc.h>
#include <metal/generated/sifive_fe310_g000_hfrosc.h>
#include <metal/io.h>

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

    if (!PRCI_REGW(METAL_SIFIVE_FE310_G000_PRCI_HFROSCCFG) &
        PRCI_HFROSCCFG_ENABLE) {
        return 0;
    }
    while (!PRCI_REGW(METAL_SIFIVE_FE310_G000_PRCI_HFROSCCFG) &
           PRCI_HFROSCCFG_READY)
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
