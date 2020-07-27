/* Copyright 2020 SiFive, Inc */
/* SPDX-License-Identifier: Apache-2.0 */

#ifndef METAL__DRIVERS__SIFIVE_BUSERROR0_H
#define METAL__DRIVERS__SIFIVE_BUSERROR0_H

/*!
 * @file sifive_buserror0.h
 *
 * @brief API for configuring the SiFive Bus Error Unit
 */

#include <metal/cpu.h>
#include <stdbool.h>
#include <stdint.h>

/*!
 * @brief The set of possible events handled by a SiFive Bus Error Unit
 */
typedef enum {
    /*! @brief No event or error has been detected */
    METAL_BUSERROR_EVENT_NONE = 0,

    /*! @brief A correctable ECC error has occurred in the I$ or ITIM */
    METAL_BUSERROR_EVENT_INST_CORRECTABLE_ECC_ERROR = (1 << 2),
    /*! @brief An uncorrectable ECC error has occurred in the I$ or ITIM */
    METAL_BUSERROR_EVENT_INST_UNCORRECTABLE_ECC_ERROR = (1 << 3),
    /*! @brief A TileLink load or store bus error has occurred */
    METAL_BUSERROR_EVENT_LOAD_STORE_ERROR = (1 << 5),
    /*! @brief A correctable ECC error has occurred in the D$ or DTIM */
    METAL_BUSERROR_EVENT_DATA_CORRECTABLE_ECC_ERROR = (1 << 6),
    /*! @brief An uncorrectable ECC error has occurred in the D$ or DTIM */
    METAL_BUSERROR_EVENT_DATA_UNCORRECTABLE_ECC_ERROR = (1 << 7),

    /*! @brief Used to set/clear all interrupts or query/clear all accrued
       events */
    METAL_BUSERROR_EVENT_ALL =
        METAL_BUSERROR_EVENT_INST_CORRECTABLE_ECC_ERROR |
        METAL_BUSERROR_EVENT_INST_UNCORRECTABLE_ECC_ERROR |
        METAL_BUSERROR_EVENT_LOAD_STORE_ERROR |
        METAL_BUSERROR_EVENT_DATA_CORRECTABLE_ECC_ERROR |
        METAL_BUSERROR_EVENT_DATA_UNCORRECTABLE_ECC_ERROR,
    /*! @brief A synonym of METAL_BUSERROR_EVENT_ALL */
    METAL_BUSERROR_EVENT_ANY = METAL_BUSERROR_EVENT_ALL,

    /*! @brief A value which is impossible for the bus error unit to report.
     * Indicates an error has occurred if provided as a return value. */
    METAL_BUSERROR_EVENT_INVALID = (1 << 8),
} sifive_buserror_event_t;

/*!
 * @brief Enable bus error events
 *
 * Enabling bus error events causes them to be registered as accrued and,
 * if the corresponding interrupt is inabled, trigger interrupts.
 *
 * @param cpu The CPU handle corresponding to the hart to configure
 * @param events A mask of error events to enable
 * @return 0 upon success
 */
int sifive_buserror_enable_events(struct metal_cpu cpu,
                                  sifive_buserror_event_t events);

/*!
 * @brief Disable bus error events
 *
 * @param cpu The CPU handle corresponding to the hart to configure
 * @param events A mask of error events to enable
 * @return 0 upon success
 */
int sifive_buserror_disable_events(struct metal_cpu cpu,
                                   sifive_buserror_event_t events);

/*!
 * @brief Get enabled bus error events
 * @param cpu The CPU handle corresponding to the hart to query
 * @return A mask of all enabled events
 */
sifive_buserror_event_t sifive_buserror_get_event_enabled(struct metal_cpu cpu);

/*!
 * @brief Get the error event which caused the most recent interrupt
 *
 * This method should be called from within the interrupt handler for the bus
 * error unit interrupt
 *
 * @param beu The bus error unit handle
 * @return The event which caused the interrupt
 */
sifive_buserror_event_t sifive_buserror_get_cause(struct metal_cpu cpu);

/*!
 * @brief Clear the cause register for the bus error unit
 *
 * This method should be called from within the interrupt handler for the bus
 * error unit to un-latch the cause register for the next event
 *
 * @param cpu The CPU handle corresponding to the hart to configure
 * @return 0 upon success
 */
int sifive_buserror_clear_cause(struct metal_cpu cpu);

/*!
 * @brief Get the physical address of the error event
 *
 * This method should be called from within the interrupt handler for the bus
 * error unit.
 *
 * @param beu The bus error unit handle
 * @return The address of the error event
 */
uintptr_t sifive_buserror_get_event_address(struct metal_cpu cpu);

/*!
 * @brief Returns true if the event is set in the accrued register
 *
 * @param cpu The CPU handle corresponding to the hart to configure
 * @param event The event to query
 * @return True if the event is set in the accrued register
 */
bool sifive_buserror_is_event_accrued(struct metal_cpu cpu,
                                      sifive_buserror_event_t events);

/*!
 * @brief Clear the given event from the accrued register
 *
 * @param cpu The CPU handle corresponding to the hart to configure
 * @param event The event to clear
 * @return 0 upon success
 */
int sifive_buserror_clear_event_accrued(struct metal_cpu cpu,
                                        sifive_buserror_event_t events);

/*!
 * @brief Enable the platform-level buserror unit interrupt
 *
 * @param cpu The CPU handle corresponding to the hart to configure
 * @return 0 upon success
 */
int sifive_buserror_enable_platform_interrupt(struct metal_cpu cpu,
                                              sifive_buserror_event_t events);

/*!
 * @brief Disable the platform-level buserror unit interrupt
 *
 * @param cpu The CPU handle corresponding to the hart to configure
 * @return 0 upon success
 */
int sifive_buserror_disable_platform_interrupt(struct metal_cpu cpu,
                                               sifive_buserror_event_t events);

/*!
 * @brief Enable the hart-local buserror unit interrupt
 *
 * @param cpu The CPU handle corresponding to the hart to configure
 * @return 0 upon success
 */
int sifive_buserror_enable_local_interrupt(struct metal_cpu cpu,
                                           sifive_buserror_event_t events);

/*!
 * @brief Disable the hart-local buserror unit interrupt
 *
 * @param cpu The CPU handle corresponding to the hart to configure
 * @return 0 upon success
 */
int sifive_buserror_disable_local_interrupt(struct metal_cpu cpu,
                                            sifive_buserror_event_t events);

#endif
