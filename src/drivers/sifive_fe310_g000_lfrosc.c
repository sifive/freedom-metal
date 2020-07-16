/* Copyright 2019 SiFive, Inc */
/* SPDX-License-Identifier: Apache-2.0 */

#include <metal/machine/platform.h>

#ifdef METAL_SIFIVE_FE310_G000_LFROSC

#include <metal/drivers/sifive_fe310_g000_lfrosc.h>
#include <metal/generated/sifive_fe310_g000_lfrosc.h>
#include <metal/io.h>

/* LFROSCCFG */
#define METAL_LFROSCCFG_DIV_MASK 0x3F
#define METAL_LFROSCCFG_TRIM_SHIFT 16
#define METAL_LFROSCCFG_TRIM_MASK (0x1F << METAL_LFROSCCFG_TRIM_SHIFT)
#define METAL_LFROSCCFG_EN (1 << 30)
#define METAL_LFROSCCFG_RDY (1 << 31)

/* LFCLKMUX */
#define METAL_LFCLKMUX_SEL 1
#define METAL_LFCLKMUX_EXT_MUX_STATUS (1 << 31)

#define LFROSC_REGW(addr) (__METAL_ACCESS_ONCE((__metal_io_u32 *)addr))

static inline uint32_t get_index(struct metal_clock clock) {
    return clock.__clock_index;
}

uint64_t __metal_driver_sifive_fe310_g000_lfrosc_get_rate_hz(
    struct metal_clock clock) {

    uintptr_t cfg_reg = dt_clock_data[get_index(clock)].config;
    uintptr_t mux_reg = dt_clock_data[get_index(clock)].mux;

    if (LFROSC_REGW(mux_reg) & METAL_LFCLKMUX_EXT_MUX_STATUS) {
        struct metal_clock lfrosc = dt_clock_data[get_index(clock)].lfrosc;
        return dt_clock_get_rate_hz(lfrosc);
    }

    uint64_t div =
        (LFROSC_REGW(cfg_reg) & METAL_LFROSCCFG_DIV_MASK) + 1;

    struct metal_clock psdlfaltclk = dt_clock_data[get_index(clock)].psdlfaltclk;
    return dt_clock_get_rate_hz(psdlfaltclk) / div;
}

uint64_t __metal_driver_sifive_fe310_g000_lfrosc_set_rate_hz(
    struct metal_clock clock, uint64_t rate) {
    return __metal_driver_sifive_fe310_g000_lfrosc_get_rate_hz(clock);
}

uint64_t __metal_driver_sifive_fe310_g000_lfrosc_register_pre_rate_change_callback(
    struct metal_clock clock, uint64_t rate) {}

uint64_t __metal_driver_sifive_fe310_g000_lfrosc_register_post_rate_change_callback(
    struct metal_clock clock, uint64_t rate) {}

#endif /* METAL_SIFIVE_FE310_G000_LFROSC */

typedef int no_empty_translation_units;
