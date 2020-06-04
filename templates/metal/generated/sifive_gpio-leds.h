/* Copyright 2018 SiFive, Inc */
/* SPDX-License-Identifier: Apache-2.0 */

#ifndef METAL__DRIVERS__SIFIVE_GPIO_LEDS_H
#define METAL__DRIVERS__SIFIVE_GPIO_LEDS_H

{% include 'template_comment.h' %}

#include <metal/generated/led.h>
#include <metal/gpio.h>

static const struct dt_led_data {
	char *label;
	struct metal_gpio gpio;
	uint32_t pin;
} dt_led_data[__METAL_DT_NUM_ledS] = {
	{% for led in leds %}
	{
		.label = "{{ led.label }}",

	    .gpio = {{ led.gpio.id }},
	    .pin = {{ led.gpio.pin }},
	},
	{% endfor %}
};

#endif
