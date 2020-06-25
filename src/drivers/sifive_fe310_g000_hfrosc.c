/* Copyright 2018 SiFive, Inc */
/* SPDX-License-Identifier: Apache-2.0 */

#include <metal/machine/platform.h>

#ifdef METAL_SIFIVE_FE310_G000_HFROSC

#include <metal/drivers/sifive_fe310_g000_hfrosc.h>
#include <metal/generated/sifive_fe310_g000_hfrosc.h>
#include <metal/io.h>

#define CONFIG_DIVIDER 0x0000003FUL
#define CONFIG_TRIM 0x001F0000UL
#define CONFIG_ENABLE 0x40000000UL
#define CONFIG_READY 0x80000000UL

static inline uint32_t get_index(struct metal_clock clk) {
    return clk.__clock_index;
}

uint64_t __metal_driver_sifive_fe310_g000_hfrosc_get_rate_hz(
    struct metal_clock clock) {

    uintptr_t base = dt_clock_data[get_index(clock)].base;
    uint32_t cfg = __METAL_ACCESS_ONCE((__metal_io_u32 *)base);

    if ((cfg & CONFIG_ENABLE) == 0)
        return 0;
    if ((cfg & CONFIG_READY) == 0)
        return 0;

    struct metal_clock ref = dt_clock_data[get_index(clock)].ref;
    uint64_t ref_rate = metal_clock_get_rate_hz(clock);

    return ref_rate / ((cfg & CONFIG_DIVIDER) + 1);
}

uint64_t __metal_driver_sifive_fe310_g000_hfrosc_set_rate_hz(
    struct metal_clock clock, uint64_t rate) {
    return __metal_driver_sifive_fe310_g000_hfrosc_get_rate_hz(clock);
}

void __metal_driver_sifive_fe310_g000_hfrosc_register_pre_rate_change_callback(struct metal_clock clk,
    metal_clock_callback *cb) {}

void __metal_driver_sifive_fe310_g000_hfrosc_register_post_rate_change_callback(struct metal_clock clk,
    metal_clock_callback *cb) {}

#endif /* METAL_SIFIVE_FE310_G000_HFROSC */

typedef int no_empty_translation_units;
