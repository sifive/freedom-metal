/* Copyright 2018 SiFive, Inc */
/* SPDX-License-Identifier: Apache-2.0 */

#ifndef MEE__LED_H
#define MEE__LED_H

/*!
 * @file led.h
 * @brief API for manipulating LEDs
 */

struct mee_led;

struct mee_led_vtable {
    int (*led_exist)(struct mee_led *led, char *label);
    void (*led_enable)(struct mee_led *led);
    void (*led_on)(struct mee_led *led);
    void (*led_off)(struct mee_led *led);
    void (*led_toggle)(struct mee_led *led);
};

/*!
 * @brief A handle for an LED
 */
struct mee_led {
    const struct mee_led_vtable *vtable;
};

/*!
 * @brief Get a handle for an LED
 * @param label The DeviceTree label for the desired LED
 * @return A handle to the LED, or NULL if none is found for the requested label
 */
struct mee_led* mee_led_get(char *label);

/*!
 * @brief Get a handle for a channel of an RGB LED
 * @param label The DeviceTree label for the desired LED
 * @param color The color for the LED in the DeviceTree
 * @return A handle to the LED, or NULL if none is found for the requested label and color
 */
struct mee_led* mee_led_get_rgb(char *label, char *color);

/*!
 * @brief Enable an LED
 * @param led The handle for the LED
 */
inline void mee_led_enable(struct mee_led *led) { led->vtable->led_enable(led); }

/*!
 * @brief Turn an LED on
 * @param led The handle for the LED
 */
inline void mee_led_on(struct mee_led *led) { led->vtable->led_on(led); }

/*!
 * @brief Turn an LED off
 * @param led The handle for the LED
 */
inline void mee_led_off(struct mee_led *led) { led->vtable->led_off(led); }

/*!
 * @brief Toggle the on/off state of an LED
 * @param led The handle for the LED
 */
inline void mee_led_toggle(struct mee_led *led) { led->vtable->led_toggle(led); }

#endif
