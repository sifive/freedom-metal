/* Copyright 2018 SiFive, Inc */
/* SPDX-License-Identifier: Apache-2.0 */

#ifndef MEE__BUTTON_H
#define MEE__BUTTON_H

/*!
 * @file button.h
 * API for interfacing with physical buttons
 */

#include <mee/interrupt.h>

struct mee_button;

struct mee_button_vtable {
    int (*button_exist)(struct mee_button *button, char *label);
    struct mee_interrupt* (*interrupt_controller)(struct mee_button *button);
    int (*get_interrupt_id)(struct mee_button *button);
};

/*!
 * @brief A button device handle
 *
 * A `struct mee_button` is an implementation-defined object which represents
 * a button on a development board.
 */
struct mee_button {
    const struct mee_button_vtable *vtable;
};

/*!
 * @brief Get a reference to a button
 *
 * @param label The DeviceTree label for the button
 * @return A handle for the button
 */
struct mee_button* mee_button_get(char *label);


/*!
 * @brief Get the interrupt controller for a button
 *
 * @param button The handle for the button
 * @return A pointer to the interrupt controller responsible for handling
 * button interrupts.
 */
inline struct mee_interrupt*
    mee_button_interrupt_controller(struct mee_button *button) { return button->vtable->interrupt_controller(button); }

/*!
 * @brief Get the interrupt id for a button
 *
 * @param button The handle for the button
 * @return The interrupt id corresponding to a button.
 */
inline int mee_button_get_interrupt_id(struct mee_button *button) { return button->vtable->get_interrupt_id(button); }

#endif
