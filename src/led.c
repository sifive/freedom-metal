/* Copyright 2018 SiFive, Inc */
/* SPDX-License-Identifier: Apache-2.0 */

#include <assert.h>
#include <metal/led.h>
#include <stdbool.h>

struct metal_led metal_led_get_rgb(char *label, char *color) __attribute__((weak));
struct metal_led metal_led_get_rgb(char *label, char *color) {
    assert(false);
    return (struct metal_led) { 0 };
}

struct metal_led *metal_led_get(char *label) __attribute__((weak));
struct metal_led *metal_led_get(char *label) {
    assert(false);
    return (struct metal_led) { 0 };
}

void metal_led_enable(struct metal_led led) __attribute__((weak));
void metal_led_enable(struct metal_led led) {}

void metal_led_on(struct metal_led led) __attribute__((weak));
void metal_led_on(struct metal_led led) {}

void metal_led_off(struct metal_led led) __attribute__((weak));
void metal_led_off(struct metal_led led) {}

void metal_led_toggle(struct metal_led led) __attribute__((weak));
void metal_led_toggle(struct metal_led led) {}
