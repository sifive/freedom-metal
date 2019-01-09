/* Copyright 2018 SiFive, Inc */
/* SPDX-License-Identifier: Apache-2.0 */

#include <string.h>
#include <mee/led.h>
#include <mee/machine.h>

struct mee_led* mee_led_get_rgb (char *label, char *color)
{
    struct mee_led *led;
    char led_label[100];

    if ((__MEE_DT_MAX_LEDS == 0) ||
	(label == NULL) || (color == NULL)) {
        return NULL;
    }

    strcpy(led_label, label);
    strcat(led_label, color);
    for (i = 0; i < __MEE_DT_MAX_LEDS; i++) {
        led = (struct mee_led*)&(__mee_led_table[i]);
        if (led->vtable->led_exist(led, led_label)) {
	    return led;
	}
    }
    return NULL;
}

struct mee_led* mee_led_get (char *label)
{
    return mee_led_get_rgb(label, "");
}

extern inline void mee_led_enable(struct mee_led *led);
extern inline void mee_led_on(struct mee_led *led);
extern inline void mee_led_off(struct mee_led *led);
extern inline void mee_led_toggle(struct mee_led *led);
