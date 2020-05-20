/* Copyright 2018 SiFive, Inc */
/* SPDX-License-Identifier: Apache-2.0 */

#ifndef METAL__LED_H
#define METAL__LED_H

#include <stdint.h>

/*!
 * @file led.h
 * @brief API for manipulating LEDs
 */

/*!
 * @brief A handle for an LED
 */
struct metal_led {
    uint8_t __no_empty_structs;
};

/*!
 * @brief Get a handle for an LED
 * @param label The DeviceTree label for the desired LED
 * @return A handle to the LED, or NULL if none is found for the requested label
 */
struct metal_led *metal_led_get(char *label);

/*!
 * @brief Get a handle for a channel of an RGB LED
 * @param label The DeviceTree label for the desired LED
 * @param color The color for the LED in the DeviceTree
 * @return A handle to the LED, or NULL if none is found for the requested label
 * and color
 */
struct metal_led *metal_led_get_rgb(char *label, char *color);

/*!
 * @brief Enable an LED
 * @param led The handle for the LED
 */
void metal_led_enable(struct metal_led *led);

/*!
 * @brief Turn an LED on
 * @param led The handle for the LED
 */
void metal_led_on(struct metal_led *led);

/*!
 * @brief Turn an LED off
 * @param led The handle for the LED
 */
void metal_led_off(struct metal_led *led);

/*!
 * @brief Toggle the on/off state of an LED
 * @param led The handle for the LED
 */
void metal_led_toggle(struct metal_led *led);

/*!
 * @brief Check if a led has a given label
 *
 * @param led The handle for the led
 * @param label A null-terminated string
 * @return True if the led label matches
 */
int metal_led_has_label(struct metal_led *led, char *label);

#endif
