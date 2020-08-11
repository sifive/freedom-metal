/* Copyright 2019 SiFive, Inc */
/* SPDX-License-Identifier: Apache-2.0 */

#include <metal/platform.h>

#ifdef METAL_SIFIVE_FE310_G000_LFROSC

#include <metal/drivers/sifive_fe310_g000_lfrosc.h>
#include <metal/generated/sifive_fe310_g000_lfrosc.h>
#include <metal/io.h>

#ifndef METAL_SIFIVE_AON0
#error No AON block detected.
#endif

/* LFROSCCFG */
#define METAL_LFROSCCFG_EN (1 << 30)
#define METAL_LFROSCCFG_RDY (1 << 31)

/* LFCLKMUX */
#define METAL_LFCLKMUX_SEL 1
#define METAL_LFCLKMUX_EXT_MUX_STATUS (1 << 31)

#define LFROSC_REGW(offset)                                                    \
    __METAL_ACCESS_ONCE(                                                       \
        (__metal_io_u32 *)(METAL_SIFIVE_AON0_0_BASE_ADDRESS + (offset)))

uint64_t sifive_fe310_g000_lfrosc_get_rate_hz(struct metal_clock clock) {

    if (LFROSC_REGW(METAL_SIFIVE_AON0_LFCLKMUX) &
        METAL_LFCLKMUX_EXT_MUX_STATUS) {
        if (!LFROSC_REGW(METAL_SIFIVE_AON0_LFROSCCFG) & METAL_LFROSCCFG_EN) {
            return 0;
        }
        while (!LFROSC_REGW(METAL_SIFIVE_AON0_LFROSCCFG) & METAL_LFROSCCFG_RDY)
            ;

        return metal_clock_get_rate_hz(REF_LFROSC(clock));
    }

    return metal_clock_get_rate_hz(REF_PSDLFALTCLK(clock));
}

uint64_t sifive_fe310_g000_lfrosc_set_rate_hz(struct metal_clock clock,
                                              uint64_t rate) {
    return sifive_fe310_g000_lfrosc_get_rate_hz(clock);
}

#endif /* METAL_SIFIVE_FE310_G000_LFROSC */

typedef int no_empty_translation_units;
