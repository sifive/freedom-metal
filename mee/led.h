/* Copyright 2018 SiFive, Inc */
/* SPDX-License-Identifier: Apache-2.0 */

#ifndef MEE__LED_H
#define MEE__LED_H

//#include <mee/led.h>

struct mee_led;

struct mee_led_vtable {
    int (*led_exist)(struct mee_led *led, char *label);
    void (*led_enable)(struct mee_led *led);
    void (*led_on)(struct mee_led *led);
    void (*led_off)(struct mee_led *led);
    void (*led_toggle)(struct mee_led *led);
};

struct mee_led {
    const struct mee_led_vtable *vtable;
};

struct mee_led* mee_led_get(char *label);
struct mee_led* mee_led_get_rgb(char *label, char *color);
inline void mee_led_enable(struct mee_led *led) { led->vtable->led_enable(led); }
inline void mee_led_on(struct mee_led *led) { led->vtable->led_on(led); }
inline void mee_led_off(struct mee_led *led) { led->vtable->led_off(led); }
inline void mee_led_toggle(struct mee_led *led) { led->vtable->led_toggle(led); }

#endif
