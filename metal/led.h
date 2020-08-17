/* Copyright 2018 SiFive, Inc */
/* SPDX-License-Identifier: Apache-2.0 */

#ifndef METAL__LED_H
#define METAL__LED_H

/*!
 * @file led.h
 * @brief API for manipulating LEDs
 */

#include <metal/private/metal_private_led.h>
#include <stdbool.h>
#include <stdint.h>

/*!
 * @brief A handle for an LED
 */
struct metal_led {
    uint32_t __led_index;
};

#define METAL_LED_NONE UINT32_MAX

#define metal_led_none ((struct metal_led){METAL_LED_NONE})

static inline bool metal_led_is_none(struct metal_led led) {
    return led.__led_index == METAL_LED_NONE;
}

void metal_led_enable(struct metal_led led);

/*!
 * @brief Turn an LED on
 * @param led The handle for the LED
 */
void metal_led_on(struct metal_led led);

/*!
 * @brief Turn an LED off
 * @param led The handle for the LED
 */
void metal_led_off(struct metal_led led);

/*!
 * @brief Toggle the on/off state of an LED
 * @param led The handle for the LED
 */
void metal_led_toggle(struct metal_led led);

#endif
