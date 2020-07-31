/* Copyright 2019 SiFive, Inc */
/* SPDX-License-Identifier: Apache-2.0 */

#ifndef METAL__WATCHDOG_H
#define METAL__WATCHDOG_H

/*!
 * @file watchdog.h
 *
 * @brief API for configuring watchdog timers
 */

#include <metal/generated/watchdog.h>
#include <metal/machine/platform.h>
#include <stdint.h>

/*!
 * @brief The type of the watchdog timeout counter
 */
typedef uint32_t metal_watchdog_count_t;

/*!
 * @brief List of watchdog timer count behaviors
 */
enum metal_watchdog_run_option {
    METAL_WATCHDOG_STOP = 0,   /*!< Stop the watchdog */
    METAL_WATCHDOG_RUN_ALWAYS, /*!< Run the watchdog continuously, even during
                                  sleep */
    METAL_WATCHDOG_RUN_AWAKE,  /*!< Run the watchdog only while the CPU is awake
                                */
};

/*!
 * @brief List of behaviors when a watchdog triggers
 */
enum metal_watchdog_result {
    METAL_WATCHDOG_NO_RESULT = 0, /*!< When the watchdog triggers, do nothing */
    METAL_WATCHDOG_INTERRUPT, /*!< When the watchdog triggers, fire an interrupt
                               */
    METAL_WATCHDOG_FULL_RESET, /*!< When the watchdog triggers, cause a full
                                  system reset */
};

#define METAL_WATCHDOG_INVALID_INDEX UINT32_MAX

/*!
 * @brief Handle for a Watchdog Timer
 */
struct metal_watchdog {
    uint32_t __wdog_index;
};

static inline struct metal_watchdog metal_watchdog_get_device(uint32_t index) {
#if __METAL_DT_NUM_WDOGS > 0
    if (index < __METAL_DT_NUM_WDOGS) {
        return (struct metal_watchdog){index};
    }
#endif
    return (struct metal_watchdog){METAL_WATCHDOG_INVALID_INDEX};
}

/*!
 * @brief Feed the watchdog timer
 */
int metal_watchdog_feed(struct metal_watchdog wdog);

/*!
 * @brief Get the rate of the watchdog timer in Hz
 *
 * @return the rate of the watchdog timer
 */
uint64_t metal_watchdog_get_rate(struct metal_watchdog wdog);

/*!
 * @brief Set the rate of the watchdog timer in Hz
 *
 * There is no guarantee that the new rate will match the requested rate.
 *
 * @return the new rate of the watchdog timer
 */
uint64_t metal_watchdog_set_rate(struct metal_watchdog wdog,
                                 const uint64_t rate);

/*!
 * @brief Get the timeout of the watchdog timer
 *
 * @return the watchdog timeout value
 */
metal_watchdog_count_t metal_watchdog_get_timeout(struct metal_watchdog wdog);

/*!
 * @brief Set the timeout of the watchdog timer
 *
 * The set rate will be the minimimum of the requested and maximum supported
 * rates.
 *
 * @return the new watchdog timeout value
 */
metal_watchdog_count_t
metal_watchdog_set_timeout(struct metal_watchdog wdog,
                           const metal_watchdog_count_t timeout);

/*!
 * @brief Sets the result behavior of a watchdog timer timeout
 *
 * @return 0 if the requested result behavior is supported
 */
int metal_watchdog_set_result(struct metal_watchdog wdog,
                              const enum metal_watchdog_result result);

/*!
 * @brief Set the run behavior of the watchdog
 *
 * Used to enable/disable the watchdog timer
 *
 * @return 0 if the watchdog was successfully started/stopped
 */
int metal_watchdog_run(struct metal_watchdog wdog,
                       const enum metal_watchdog_run_option option);

/*!
 * @brief Enable the watchdog interrupt
 * @return 0 upon success
 */
int metal_watchdog_enable_interrupt(struct metal_watchdog wdog);

/*!
 * @brief Disable the watchdog interrupt
 * @return 0 upon success
 */
int metal_watchdog_disable_interrupt(struct metal_watchdog wdog);

/*!
 * @brief Clear the watchdog interrupt
 * @return 0 upon success
 */
int metal_watchdog_clear_interrupt(struct metal_watchdog wdog);

#endif /* METAL__WATCHDOG_H */
