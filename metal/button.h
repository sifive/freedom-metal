/* Copyright 2018 SiFive, Inc */
/* SPDX-License-Identifier: Apache-2.0 */

#ifndef METAL__BUTTON_H
#define METAL__BUTTON_H

/*!
 * @file button.h
 * API for interfacing with physical buttons
 */

#include <metal/private/metal_private_button.h>
#include <stdbool.h>
#include <stdint.h>

#define METAL_BUTTON_INVALID_INDEX UINT32_MAX

/*!
 * @brief A button device handle
 *
 * A `struct metal_button` is an implementation-defined object which represents
 * a button on a development board.
 */
struct metal_button {
    uint32_t __button_index;
};

#define METAL_BUTTON_NONE UINT32_MAX

#define metal_button_none ((struct metal_button){METAL_BUTTON_NONE})

static inline bool metal_button_is_none(struct metal_button button) {
    return button.__button_index == METAL_BUTTON_NONE;
}

/*!
 * @brief Get a reference to a button
 *
 * @param index The button index
 * @return A handle for the button
 */
static inline struct metal_button metal_button_get(uint32_t index) {
    return ((struct metal_button){index});
}

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

/*!
 * @brief Clear the button interrupt
 *
 * @param button The button handle
 * @return 0 upon success
 */
int metal_button_clear_interrupt(struct metal_button button);

#endif
