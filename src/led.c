/* Copyright 2018 SiFive, Inc */
/* SPDX-License-Identifier: Apache-2.0 */

#include <assert.h>
#include <metal/led.h>

void metal_led_enable(struct metal_led led) __attribute__((weak));
void metal_led_enable(struct metal_led led) {}

void metal_led_on(struct metal_led led) __attribute__((weak));
void metal_led_on(struct metal_led led) {}

void metal_led_off(struct metal_led led) __attribute__((weak));
void metal_led_off(struct metal_led led) {}

void metal_led_toggle(struct metal_led led) __attribute__((weak));
void metal_led_toggle(struct metal_led led) {}
