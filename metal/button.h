/* Copyright 2018 SiFive, Inc */
/* SPDX-License-Identifier: Apache-2.0 */

#ifndef METAL__BUTTON_H
#define METAL__BUTTON_H

/*!
 * @file button.h
 * API for interfacing with physical buttons
 */

#include <metal/generated/button.h>
#include <stdbool.h>
#include <stdint.h>

/*!
 * @brief A button device handle
 *
 * A `struct metal_button` is an implementation-defined object which represents
 * a button on a development board.
 */
struct metal_button {
    uint32_t __button_index;
};

/*!
 * @brief Get a reference to a button
 *
 * @param index The button index
 * @return A handle for the button
 */
struct metal_button metal_button_get(uint32_t index);

/*!
 * @brief Returns if the button is currently pressed
 *
 * @param button The button handtle
 * @return True if the button is pressed
 */
bool metal_button_is_pressed(struct metal_button button);

/*!
 * @brief Enable the button interrupt
 *
 * @param button The button handle
 * @return 0 upon success
 */
int metal_button_enable_interrupt(struct metal_button button);

/*!
 * @brief Disable the button interrupt
 *
 * @param button The button handle
 * @return 0 upon success
 */
int metal_button_disable_interrupt(struct metal_button button);

#endif
