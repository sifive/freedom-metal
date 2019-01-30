/* Copyright 2018 SiFive, Inc */
/* SPDX-License-Identifier: Apache-2.0 */

#include <metal/drivers/sifive,fe310-g000,hfrosc.h>

#define CONFIG_DIVIDER 0x0000003FUL
#define CONFIG_TRIM    0x001F0000UL
#define CONFIG_ENABLE  0x40000000UL
#define CONFIG_READY   0x80000000UL

long __metal_driver_sifive_fe310_g000_hfrosc_get_rate_hz(const struct metal_clock *clock)
{
    struct __metal_driver_sifive_fe310_g000_hfrosc *clk = (void *)clock;
    long cfg = clk->config_base->vtable->get_reg(clk->config_base, clk->config_offset);
    if (cfg & CONFIG_ENABLE == 0)
        return -1;
    if (cfg & CONFIG_READY == 0)
        return -1;
    return metal_clock_get_rate_hz(clk->ref) / ((cfg & CONFIG_DIVIDER) + 1);
}

long __metal_driver_sifive_fe310_g000_hfrosc_set_rate_hz(struct metal_clock *clock, long rate)
{
    return __metal_driver_sifive_fe310_g000_hfrosc_get_rate_hz(clock);
}
