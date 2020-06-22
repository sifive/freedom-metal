/* Copyright 2018 SiFive, Inc */
/* SPDX-License-Identifier: Apache-2.0 */

#ifndef METAL__DRIVERS__SIFIVE_GPIO0_H
#define METAL__DRIVERS__SIFIVE_GPIO0_H

{% include 'template_comment.h' %}

#include <metal/generated/gpio.h>
#include <metal/interrupt.h>
#include <metal/machine/platform.h>
#include <stddef.h>
#include <stdint.h>

static const struct dt_gpio_data {
	uintptr_t base_addr;
	struct metal_interrupt interrupt_parent;
	uint32_t interrupt_id_base;
} dt_gpio_data[__METAL_DT_NUM_GPIOS] = {
	{% for gpio in gpios %}
	{
		.base_addr = METAL_SIFIVE_GPIO0_{{ gpio.id }}_BASE_ADDR,

	{% if gpio.interrupt_parent is defined %}
		.interrupt_parent = (struct metal_interrupt) { {{ gpio.interrupt_parent[0].id }} },
		.interrupt_id_base = {{ gpio.interrupts[0] }},
	{% else %}
		.interrupt_parent = NULL,
	{% endif %}
	},
	{% endfor %}
};

{% set driver_string = to_snakecase(gpios[0].interrupt_parent[0].compatible[0]) %}
{% include 'interrupt_dispatch.h' %}

#endif
