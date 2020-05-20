/* Copyright 2018 SiFive, Inc */
/* SPDX-License-Identifier: Apache-2.0 */

#include <metal/led.h>
#include <metal/machine.h>
#include <string.h>

struct metal_led *metal_led_get_rgb(char *label, char *color) {
#if __METAL_DT_MAX_LEDS > 0
    int i;
    struct metal_led *led;
    char led_label[100];

    if ((__METAL_DT_MAX_LEDS == 0) || (label == NULL) || (color == NULL)) {
        return NULL;
    }

    strcpy(led_label, label);
    strcat(led_label, color);
    for (i = 0; i < __METAL_DT_MAX_LEDS; i++) {
        led = (struct metal_led *)__metal_led_table[i];
        if (metal_led_has_label(led, led_label)) {
            return led;
        }
    }
    return NULL;
#else
    return NULL;
#endif
}

struct metal_led *metal_led_get(char *label) {
    return metal_led_get_rgb(label, "");
}
