/* Copyright 2018 SiFive, Inc */
/* SPDX-License-Identifier: Apache-2.0 */

#include <string.h>
#include <metal/led.h>
#include <metal/machine.h>

const struct metal_led* metal_led_get_rgb (char *label, char *color)
{
    int i;
    const struct metal_led *led;
    char led_label[100];

    if ((__METAL_DT_MAX_LEDS == 0) ||
	(label == NULL) || (color == NULL)) {
        return NULL;
    }

    strcpy(led_label, label);
    strcat(led_label, color);
    for (i = 0; i < __METAL_DT_MAX_LEDS; i++) {
        led = (const struct metal_led*)__metal_led_table[i];
        if (led->vtable->led_exist(led, led_label)) {
	    return led;
	}
    }
    return NULL;
}

const struct metal_led* metal_led_get (char *label)
{
    return metal_led_get_rgb(label, "");
}

extern inline void metal_led_enable(const struct metal_led *led);
extern inline void metal_led_on(const struct metal_led *led);
extern inline void metal_led_off(const struct metal_led *led);
extern inline void metal_led_toggle(const struct metal_led *led);
