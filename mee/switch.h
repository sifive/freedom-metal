/* Copyright 2018 SiFive, Inc */
/* SPDX-License-Identifier: Apache-2.0 */

#ifndef MEE__SWITCH_H
#define MEE__SWITCH_H

/*!
 * @file switch.h
 * @brief API for reading toggle switches
 */

#include <mee/interrupt.h>

struct mee_switch;

struct mee_switch_vtable {
    int (*switch_exist)(struct mee_switch *flip, char *label);
    struct mee_interrupt* (*interrupt_controller)(struct mee_switch *flip);
    int (*get_interrupt_id)(struct mee_switch *flip);
};

/*!
 * @brief A handle for a switch
 */
struct mee_switch {
    const struct mee_switch_vtable *vtable;
};

/*!
 * @brief Get a handle for a switch
 * @param label The DeviceTree label for the desired switch
 * @return A handle to the switch, or NULL if none is found for the requested label
 */
struct mee_switch* mee_switch_get(char *label);

/*!
 * @brief Get the interrupt controller for a switch
 * @param flip The handle for the switch
 * @return The interrupt controller handle
 */
inline struct mee_interrupt*
    mee_switch_interrupt_controller(struct mee_switch *flip) { return flip->vtable->interrupt_controller(flip); }

/*!
 * @brief Get the interrupt id for a switch
 * @param flip The handle for the switch
 * @return The interrupt ID for the switch
 */
inline int mee_switch_get_interrupt_id(struct mee_switch *flip) { return flip->vtable->get_interrupt_id(flip); }

#endif
