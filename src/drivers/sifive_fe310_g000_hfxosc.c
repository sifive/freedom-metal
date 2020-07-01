/* Copyright 2018 SiFive, Inc */
/* SPDX-License-Identifier: Apache-2.0 */

#include <metal/machine/platform.h>

#ifdef METAL_SIFIVE_FE310_G000_HFXOSC

#include <metal/drivers/sifive_fe310_g000_hfxosc.h>
#include <metal/generated/sifive_fe310_g000_hfxosc.h>
#include <metal/io.h>

#define CONFIG_ENABLE 0x40000000UL
#define CONFIG_READY 0x80000000UL

static inline uint32_t get_index(struct metal_clock clk) {
    return clk.__clock_index;
}

uint64_t __metal_driver_sifive_fe310_g000_hfxosc_get_rate_hz(
    struct metal_clock clock) {

    uintptr_t base = dt_clock_data[get_index(clock)].base;
    uint32_t cfg = __METAL_ACCESS_ONCE((__metal_io_u32 *)base);

    if ((cfg & CONFIG_ENABLE) == 0)
        return 0;
    if ((cfg & CONFIG_READY) == 0)
        return 0;

    struct metal_clock ref = dt_clock_data[get_index(clock)].ref;
    return metal_clock_get_rate_hz(ref);
}

uint64_t __metal_driver_sifive_fe310_g000_hfxosc_set_rate_hz(
    struct metal_clock clock, uint64_t rate) {
    return __metal_driver_sifive_fe310_g000_hfxosc_get_rate_hz(clock);
}

#endif /* METAL_SIFIVE_FE310_G000_HFXOSC */

typedef int no_empty_translation_units;
