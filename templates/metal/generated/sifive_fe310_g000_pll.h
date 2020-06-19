/* Copyright 2020 SiFive, Inc */
/* SPDX-License-Identifier: Apache-2.0 */

#ifndef METAL__DRIVERS__SIFIVE_FE310_G000_PLL_CLOCK_H
#define METAL__DRIVERS__SIFIVE_FE310_G000_PLL_CLOCK_H

{% include 'template_comment.h' %}

#include <metal/clock.h>
#include <metal/drivers/sifive_fe310_g000_hfrosc.h>
#include <metal/drivers/sifive_fe310_g000_hfxosc.h>
#include <stdbool.h>

#define __METAL_DT_NUM_SIFIVE_FE310_G000_PLL_CLOCKS {{ sifive_fe310_g000_plls|length }}

static const struct dt_sifive_fe310_g000_pll_clock_data {
	uint64_t init_rate;
	uintptr_t config;
	uintptr_t divider;
	bool has_hfxosc;
	struct metal_clock hfrosc;
	struct metal_clock hfxosc;
} dt_clock_data[__METAL_DT_NUM_SIFIVE_FE310_G000_PLL_CLOCKS] = {
	{% for clk in sifive_fe310_g000_plls %}
	{
		.init_rate = {{ clk.clock_frequency[0] }},
		.config = {{ clk.regs_by_name["config"] }},
		.divider = {{ clk.regs_by_name["divider"] }}

	{% if "pllref" in clk.clock_names %}
		.has_hfxosc = true,
		.hfxosc = (struct metal_clock) { {{ clk.clocks_by_name["pllref"].id }} },
	{% else %}
		.has_hfxosc = false,
	{% endif %}
		.hfrosc = (struct metal_clock) { {{ clk.clocks_by_name["pllsel0"].id }} },
	},
	{% endfor %}
};

#endif
