/* Copyright 2018 SiFive, Inc */
/* SPDX-License-Identifier: Apache-2.0 */

#ifndef METAL__SWITCH_H
#define METAL__SWITCH_H

/*!
 * @file switch.h
 * @brief API for reading toggle switches
 */

#include <metal/generated/switch.h>
#include <stdbool.h>
#include <stdint.h>

/*!
 * @brief A handle for a switch
 */
struct metal_switch {
    uint32_t __switch_index;
};

#define METAL_SWITCH_NONE UINT32_MAX

#define metal_switch_none ((struct metal_switch){METAL_SWITCH_NONE})

static inline bool metal_switch_is_none(struct metal_switch sw) {
    return sw.__switch_index == METAL_SWITCH_NONE;
}

/*!
 * @brief Get a handle for a switch
 * @param index The index of the switch
 * @return A handle to the switch
 * label
 */
struct metal_switch metal_switch_get(uint32_t index);

/*!
 * @brief Check if a switch is currently on
 * @param sw The switch handle
 * @return True if the switch is on
 */
bool metal_switch_is_on(struct metal_switch sw);

/*!
 * @brief Enable the interrupt for a switch
 * @param sw The switch handle
 * @return 0 upon success
 */
int metal_switch_enable_interrupt(struct metal_switch sw);

/*!
 * @brief Disable the interrupt for a switch
 * @param sw The switch handle
 * @return 0 upon success
 */
int metal_switch_disable_interrupt(struct metal_switch sw);

/*!
 * @brief Clear the interrupt for a switch
 * @param sw The switch handle
 * @return 0 upon success
 */
int metal_switch_clear_interrupt(struct metal_switch sw);

#endif
