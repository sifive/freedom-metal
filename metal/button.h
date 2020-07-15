/* Copyright 2018 SiFive, Inc */
/* SPDX-License-Identifier: Apache-2.0 */

#ifndef METAL__BUTTON_H
#define METAL__BUTTON_H

/*!
 * @file button.h
 * API for interfacing with physical buttons
 */

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
 * @param label The DeviceTree label for the button
 * @return A handle for the button
 */
struct metal_button metal_button_get(char *label);

#endif
