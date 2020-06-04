/* Copyright 2018 SiFive, Inc */
/* SPDX-License-Identifier: Apache-2.0 */

#include <metal/led.h>

/* Weak stubs when no driver is present */

struct metal_led metal_led_get(char *label) __attribute__((weak));
struct metal_led metal_led_get(char *label) {
    assert(0);
    return (struct metal_led) { 0 };
}

struct metal_led metal_led_get_rgb(char *label, char *color) __attribute__((weak));
struct metal_led metal_led_get_rgb(char *label, char *color) {
    assert(0);
    return (struct metal_led) { 0 };
}

int metal_led_has_label(struct metal_led led, char *label)
    __attribute__((weak));
int metal_led_has_label(struct metal_led led, char *label) { return 0; }

void metal_led_enable(struct metal_led led) __attribute__((weak));
void metal_led_enable(struct metal_led led) {}

void metal_led_on(struct metal_led led) __attribute__((weak));
void metal_led_on(struct metal_led led) {}

void metal_led_off(struct metal_led led) __attribute__((weak));
void metal_led_off(struct metal_led led) {}

void metal_led_toggle(struct metal_led led) __attribute__((weak));
void metal_led_toggle(struct metal_led led) {}
