/* Copyright 2020 SiFive, Inc */
/* SPDX-License-Identifier: Apache-2.0 */

#ifndef METAL__DRIVERS__SIFIVE_FE310_G000_LFROSC_CLOCK_H
#define METAL__DRIVERS__SIFIVE_FE310_G000_LFROSC_CLOCK_H

{% include 'template_comment.h' %}

#include <metal/clock.h>

#define __METAL_DT_NUM_SIFIVE_FE310_G000_LFROSC_CLOCKS {{ sifive_fe310_g000_lfroscs|length }}

static const struct dt_sifive_fe310_g000_lfrosc_clock_data {
	uintptr_t config;
	uintptr_t mux;
	struct metal_clock lfrosc;
	struct metal_clock psdlfaltclk;
} dt_clock_data[__METAL_DT_NUM_SIFIVE_FE310_G000_LFROSC_CLOCKS] = {
	{% for clk in sifive_fe310_g000_lfrosc_clocks %}
	{
		.config = {{ clk.reg["config"] }},
		.mux = {{ clk.reg["mux"] }}
		.lfrosc = (struct metal_clock) { {{ clk.clocks["lfrosc"].id }} },
		.lfrosc = (struct metal_clock) { {{ clk.clocks["psdlfaltclk"].id }} },
	},
	{% endfor %}
};

{% set driver_string = to_snakecase(sifive_fe310_g000_lfroscs[0].clocks[0].compatible[0]) %}
{% include 'clock_dispatch.h' %}

#endif
