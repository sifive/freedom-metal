/* Copyright 2018 SiFive, Inc */
/* SPDX-License-Identifier: Apache-2.0 */

#ifndef METAL__SWITCH_H
#define METAL__SWITCH_H

/*!
 * @file switch.h
 * @brief API for reading toggle switches
 */

#include <stdint.h>

/*!
 * @brief A handle for a switch
 */
struct metal_switch {
    uint32_t __switch_index;
};

/*!
 * @brief Get a handle for a switch
 * @param label The DeviceTree label for the desired switch
 * @return A handle to the switch, or NULL if none is found for the requested
 * label
 */
struct metal_switch metal_switch_get(char *label);

#endif
