/* Copyright 2019 SiFive, Inc. */
/* SPDX-License-Identifier: Apache-2.0 */

#ifndef METAL__RTC_H
#define METAL__RTC_H

#include <stdint.h>

/*!
 * @file rtc.h
 * @brief API for Real-Time Clocks
 */

/*!
 * @brief List of RTC run behaviors
 */
enum metal_rtc_run_option {
    METAL_RTC_STOP = 0,
    METAL_RTC_RUN,
};

/*!
 * @brief Handle for a Real-Time Clock
 */
struct metal_rtc {
    uint8_t __no_empty_structs;
};

/*!
 * @brief Get the rate of the RTC
 * @return The rate in Hz
 */
uint64_t metal_rtc_get_rate(const struct metal_rtc *const rtc);

/*!
 * @brief Set (if possible) the rate of the RTC
 * @return The new rate of the RTC (not guaranteed to be the same as requested)
 */
uint64_t metal_rtc_set_rate(const struct metal_rtc *const rtc,
                            const uint64_t rate);

/*!
 * @brief Get the compare value of the RTC
 * @return The compare value
 */
uint64_t metal_rtc_get_compare(const struct metal_rtc *const rtc);

/*!
 * @brief Set the compare value of the RTC
 * @return The set compare value (not guaranteed to be exactly the requested
 * value)
 *
 * The RTC device might impose limits on the maximum compare value or the
 * granularity of the compare value.
 */
uint64_t metal_rtc_set_compare(const struct metal_rtc *const rtc,
                               const uint64_t compare);

/*!
 * @brief Get the current count of the RTC
 * @return The count
 */
uint64_t metal_rtc_get_count(const struct metal_rtc *const rtc);

/*!
 * @brief Set the current count of the RTC
 * @return The set value of the count (not guaranteed to be exactly the
 * requested value)
 *
 * The RTC device might impose limits on the maximum value of the count
 */
uint64_t metal_rtc_set_count(const struct metal_rtc *const rtc,
                             const uint64_t count);

/*!
 * @brief Start or stop the RTC
 * @return 0 if the RTC was successfully started/stopped
 */
int metal_rtc_run(const struct metal_rtc *const rtc,
                  const enum metal_rtc_run_option option);

/*!
 * @brief Get the interrupt handle for the RTC compare
 * @return The interrupt handle
 */
struct metal_interrupt *
metal_rtc_get_interrupt(const struct metal_rtc *const rtc);

/*!
 * @brief Get the interrupt ID for the RTC compare
 * @return The interrupt ID
 */
int metal_rtc_get_interrupt_id(const struct metal_rtc *const rtc);

/*!
 * @brief Get the handle for an RTC by index
 * @return The RTC handle, or NULL if none is available at that index
 */
struct metal_rtc *metal_rtc_get_device(int index);

#endif
