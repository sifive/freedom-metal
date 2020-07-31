/* Copyright 2018 SiFive, Inc */
/* SPDX-License-Identifier: Apache-2.0 */

#ifndef METAL__INTERRUPT_H
#define METAL__INTERRUPT_H

/*! @file interrupt.h
 *  @brief API for registering and manipulating interrupts
 */

#include <metal/cpu.h>
#include <metal/interrupt_handlers.h>
#include <stddef.h>
#include <stdint.h>

/*!
 * @brief Possible mode of interrupts to operate
 */
typedef enum metal_vector_mode_ {
    METAL_DIRECT_MODE = 0,
    METAL_VECTORED_MODE = 1,
    METAL_CLIC_DIRECT_MODE = 2,
    METAL_CLIC_VECTORED_MODE = 3,
} metal_vector_mode;

/*!
 * @brief The bitmask of hart context
 */
typedef struct metal_affinity_ {
    unsigned long bitmask;
} metal_affinity;

/*!
 * @brief A handle for an interrupt controller
 */
struct metal_interrupt {
    uint32_t __interrupt_index;
};

/*!
 * @brief Initialize a given interrupt controller
 *
 * Initialize a given interrupt controller. This function must be called
 * before any interrupts are registered or enabled with the handler. It
 * is invalid to initialize an interrupt controller more than once.
 *
 * @param controller The handle for the interrupt controller
 */
void metal_interrupt_init(struct metal_interrupt controller);

/*!
 * @brief Enable an interrupt
 * @param controller The handle for the interrupt controller
 * @param id The interrupt ID to enable
 * @return 0 upon success
 */
int metal_interrupt_enable(struct metal_interrupt controller, int id);

/*!
 * @brief Disable an interrupt
 * @param controller The handle for the interrupt controller
 * @param id The interrupt ID to disable
 * @return 0 upon success
 */
int metal_interrupt_disable(struct metal_interrupt controller, int id);

#endif
