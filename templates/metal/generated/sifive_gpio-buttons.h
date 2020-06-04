/* Copyright 2020 SiFive, Inc */
/* SPDX-License-Identifier: Apache-2.0 */

#ifndef METAL__DRIVERS__SIFIVE_GPIO_BUTTONS_H
#define METAL__DRIVERS__SIFIVE_GPIO_BUTTONS_H

{% include 'template_comment.h' %}

#include <metal/generated/button.h>
#include <metal/gpio.h>
#include <metal/interrupt.h>

static const struct dt_button_data {
	char *label;
	struct metal_gpio gpio;
	struct metal_interrupt *interrupt_parent;
	uint32_t interrupt_id;
} dt_button_data[__METAL_DT_NUM_BUTTONS] = {
	{% for button in buttons %}
	{
		.label = "{{ button.label }}",

	    .gpio = {{ button.gpio.id }},

		{% if defined(button.interrupt_parent) %}
			.interrupt_parent = (struct metal_interrupt *) = {{ metal_interrupt(button.interrupt_parent.id) }},
			.interrupt_id = {{ button.interrupts[0] }},
		{% else %}
			.interrupt_parent = NULL,
		{% endif %}
	},
	{% endfor %}
};

#endif
