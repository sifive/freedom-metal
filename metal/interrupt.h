/* Copyright 2018 SiFive, Inc */
/* SPDX-License-Identifier: Apache-2.0 */

#ifndef METAL__INTERRUPT_H
#define METAL__INTERRUPT_H

/*! @file interrupt.h
 *  @brief API for registering and manipulating interrupts
 */

#include <metal/interrupt_vector_handlers.h>
#include <stdint.h>
#include <stddef.h>

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
 * @brief Possible mode of privilege interrupts to operate
 */
typedef enum metal_intr_priv_mode_ {
    METAL_INTR_PRIV_M_MODE = 0,
    METAL_INTR_PRIV_MU_MODE = 1,
    METAL_INTR_PRIV_MSU_MODE = 2
} metal_intr_priv_mode;

/*!
 * @brief The bitmask of hart context
 */
typedef struct metal_affinity_ {
    unsigned long bitmask;
} metal_affinity;

#define for_each_metal_affinity(bit, metal_affinity)                           \
    for (bit = 0; metal_affinity.bitmask; bit++, metal_affinity.bitmask >>= 1)

#define metal_affinity_set_val(metal_affinity, val)                            \
    metal_affinity.bitmask = val;

#define metal_affinity_set_bit(metal_affinity, bit, val)                       \
    metal_affinity.bitmask |= ((val & 0x1) << bit);

/*!
 * @brief Function signature for interrupt callback handlers
 */
typedef void (*metal_interrupt_handler_t)(int, void *);
typedef void (*metal_interrupt_vector_handler_t)(void);

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
 * @brief Configure vector mode for an interrupt controller
 *
 * Configure vector mode for an interrupt controller.
 * This function must be called after initialization and before
 * configuring individual interrupts, registering ISR.
 *
 * @param controller The handle for the interrupt controller
 * @param mode The vector mode of the interrupt controller.
 * @return 0 upon success
 */
int
metal_interrupt_set_vector_mode(struct metal_interrupt controller,
                                metal_vector_mode mode);

/*!
 * @brief Get vector mode of a given an interrupt controller
 *
 * Configure vector mode for an interrupt controller.
 * This function must be called after initialization and before
 * configuring individual interrupts, registering ISR.
 *
 * @param controller The handle for the interrupt controller
 * @param mode The vector mode of the interrupt controller.
 * @return The interrupt vector mode
 */
metal_vector_mode
metal_interrupt_get_vector_mode(struct metal_interrupt controller);

/*!
 * @brief Configure privilege mode a of given interrupt controller
 *
 * Configure privilege mode for a given interrupt controller.
 * This function must be called after initialization and before
 * configuring individual interrupts, registering ISR.
 *
 * @param controller The handle for the interrupt controller
 * @param privilege The privilege mode of the interrupt controller.
 * @return 0 upon success
 */
int metal_interrupt_set_privilege(struct metal_interrupt controller,
                                  metal_intr_priv_mode privilege);

/*!
 * @brief Get privilege mode a of given interrupt controller
 *
 * Get privilege mode for a given interrupt controller.
 * This function must be called after initialization and before
 * configuring individual interrupts, registering ISR.
 *
 * @param controller The handle for the interrupt controller
 * @return The interrupt privilege mode
 */
metal_intr_priv_mode
metal_interrupt_get_privilege(struct metal_interrupt controller);

/*!
 * @brief clear an interrupt
 * @param controller The handle for the interrupt controller
 * @param id The interrupt ID to trigger
 * @return 0 upon success
 */
int metal_interrupt_clear(struct metal_interrupt controller,
                          int id);

/*!
 * @brief Set an interrupt
 * @param controller The handle for the interrupt controller
 * @param id The interrupt ID to trigger
 * @return 0 upon success
 */
int metal_interrupt_set(struct metal_interrupt controller, int id);

/*!
 * @brief Register an interrupt handler
 * @param controller The handle for the interrupt controller
 * @param id The interrupt ID to register
 * @param handler The interrupt handler callback
 * @param priv_data Private data for the interrupt handler
 * @return 0 upon success
 */
int
metal_interrupt_register_handler(struct metal_interrupt controller, int id,
                                 metal_interrupt_handler_t handler,
                                 void *priv_data);
/*!
 * @brief Register an interrupt vector handler
 * @param controller The handle for the interrupt controller
 * @param id The interrupt ID to register
 * @param handler The interrupt vector handler callback
 * @param priv_data Private data for the interrupt handler
 * @return 0 upon success
 */
int metal_interrupt_register_vector_handler(
    struct metal_interrupt controller, int id,
    metal_interrupt_vector_handler_t handler, void *priv_data);

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

/*!
 * @brief Set interrupt threshold level
 * @param controller The handle for the interrupt controller
 * @param threshold The interrupt threshold level
 * @return 0 upon success
 */
int metal_interrupt_set_threshold(struct metal_interrupt controller,
                                  unsigned int level);

/*!
 * @brief Get an interrupt threshold level
 * @param controller The handle for the interrupt controller
 * @return The interrupt threshold level
 */
unsigned int
metal_interrupt_get_threshold(struct metal_interrupt controller);

/*!
 * @brief Set an interrupt priority level
 * @param controller The handle for the interrupt controller
 * @param id The interrupt ID to enable
 * @param priority The interrupt priority level
 * @return 0 upon success
 */
int metal_interrupt_set_priority(struct metal_interrupt controller,
                                 int id, unsigned int priority);

/*!
 * @brief Get an interrupt priority level
 * @param controller The handle for the interrupt controller
 * @param id The interrupt ID to enable
 * @return The interrupt priority level
 */
unsigned int
metal_interrupt_get_priority(struct metal_interrupt controller, int id);

/*!
 * @brief Set preemptive level and priority for a given interrupt ID
 *
 * Set the preemptive level and priority for a given interrupt ID.
 *
 * @param controller The handle for the interrupt controller
 * @param id The interrupt ID to enable
 * @param level The interrupt level and priority are encoded together
 * @return 0 upon success
 */
int
metal_interrupt_set_preemptive_level(struct metal_interrupt controller, int id,
                                     unsigned int level);

/*!
 * @brief Get an interrupt preemptive level
 * @param controller The handle for the interrupt controller
 * @param id The interrupt ID to enable
 * @return The interrupt level
 */
unsigned int
metal_interrupt_get_preemptive_level(struct metal_interrupt controller, int id);

/*!
 * @brief Enable an interrupt vector
 * @param controller The handle for the interrupt controller
 * @param id The interrupt ID to enable
 * @return 0 upon success
 */
int metal_interrupt_vector_enable(struct metal_interrupt controller, int id);

/*!
 * @brief Disable an interrupt vector
 * @param controller The handle for the interrupt controller
 * @param id The interrupt ID to disable
 * @return 0 upon success
 */
int
metal_interrupt_vector_disable(struct metal_interrupt controller, int id);

/*!
 * @brief Enable an interrupt for the hart contexts
 * @param controller The handle for the interrupt controller
 * @param bitmask The bit mask of hart contexts to enable
 * @param id The interrupt ID to enable
 * @return The result of each hart context. 0 upon success at relevant bit.
 */
metal_affinity
metal_interrupt_affinity_enable(struct metal_interrupt controller,
                                metal_affinity bitmask, int id);

/*!
 * @brief Disable an interrupt for the hart contexts
 * @param controller The handle for the interrupt controller
 * @param bitmask The bit mask of hart contexts to disable
 * @param id The interrupt ID to disable
 * @return The result of each hart context. 0 upon success at relevant bit.
 */
metal_affinity
metal_interrupt_affinity_disable(struct metal_interrupt controller,
                                 metal_affinity bitmask, int id);

/*!
 * @brief Set interrupt threshold level for the hart contexts
 * @param controller The handle for the interrupt controller
 * @param bitmask The bit mask of hart contexts to set threshold
 * @param threshold The interrupt threshold level
 * @return The result of each hart context. 0 upon success at relevant bit.
 */
metal_affinity
metal_interrupt_affinity_set_threshold(struct metal_interrupt controller,
                                       metal_affinity bitmask,
                                       unsigned int level);

/*!
 * @brief Get an interrupt threshold level from the hart context
 * @param controller The handle for the interrupt controller
 * @param context_id The hart context ID to get threshold
 * @return The interrupt threshold level
 */
unsigned int
metal_interrupt_affinity_get_threshold(struct metal_interrupt controller,
                                       int context_id);
#endif
