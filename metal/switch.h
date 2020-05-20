/* Copyright 2018 SiFive, Inc */
/* SPDX-License-Identifier: Apache-2.0 */

#ifndef METAL__SWITCH_H
#define METAL__SWITCH_H

/*!
 * @file switch.h
 * @brief API for reading toggle switches
 */

#include <metal/interrupt.h>
#include <stdint.h>

/*!
 * @brief A handle for a switch
 */
struct metal_switch {
    uint8_t __no_empty_structs;
};

/*!
 * @brief Get a handle for a switch
 * @param label The DeviceTree label for the desired switch
 * @return A handle to the switch, or NULL if none is found for the requested
 * label
 */
struct metal_switch *metal_switch_get(char *label);

/*!
 * @brief Get the interrupt controller for a switch
 * @param sw The handle for the switch
 * @return The interrupt controller handle
 */
struct metal_interrupt *
metal_switch_interrupt_controller(struct metal_switch *sw);

/*!
 * @brief Get the interrupt id for a switch
 * @param sw The handle for the switch
 * @return The interrupt ID for the switch
 */
int metal_switch_get_interrupt_id(struct metal_switch *sw);

/*!
 * @brief Check if a switch has a given label
 *
 * @param switch The handle for the switch
 * @param label A null-terminated string
 * @return True if the switch label matches
 */
int metal_switch_has_label(struct metal_switch *sw, char *label);

#endif
