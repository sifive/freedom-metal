/* Copyright 2018 SiFive, Inc */
/* SPDX-License-Identifier: Apache-2.0 */

#ifndef METAL__BUTTON_H
#define METAL__BUTTON_H

#include <stdint.h>

/*!
 * @file button.h
 * API for interfacing with physical buttons
 */

#include <metal/interrupt.h>

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
 * @param label The DeviceTree label for the button
 * @return A handle for the button
 */
struct metal_button metal_button_get(char *label);

/*!
 * @brief Get the interrupt controller for a button
 *
 * @param button The handle for the button
 * @return A pointer to the interrupt controller responsible for handling
 * button interrupts.
 */
struct metal_interrupt *
metal_button_interrupt_controller(struct metal_button button);

/*!
 * @brief Get the interrupt id for a button
 *
 * @param button The handle for the button
 * @return The interrupt id corresponding to a button.
 */
int metal_button_get_interrupt_id(struct metal_button button);

/*!
 * @brief Check if a button has a given label
 *
 * @param button The handle for the button
 * @param label A null-terminated string
 * @return True if the button label matches
 */
int metal_button_has_label(struct metal_button button, char *label);

#endif
