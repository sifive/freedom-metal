/* Copyright 2019 SiFive, Inc. */
/* SPDX-License-Identifier: Apache-2.0 */

#ifndef METAL__RTC_H
#define METAL__RTC_H

#include <metal/generated/rtc.h>
#include <metal/machine/platform.h>
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

#define METAL_RTC_INVALID_INDEX UINT32_MAX

/*!
 * @brief Handle for a Real-Time Clock
 */
struct metal_rtc {
    uint32_t __rtc_index;
};

/*!
 * @brief Get the handle for an RTC by index
 * @return The RTC handle
 */
static inline struct metal_rtc metal_rtc_get_device(uint32_t index) {
#if __METAL_DT_NUM_RTCS > 0
    if (index > __METAL_DT_NUM_RTCS) {
        return (struct metal_rtc){index};
    }
#endif
    return (struct metal_rtc){METAL_RTC_INVALID_INDEX};
}

/*!
 * @brief Get the rate of the RTC
 * @return The rate in Hz
 */
uint64_t metal_rtc_get_rate(struct metal_rtc rtc);

/*!
 * @brief Set (if possible) the rate of the RTC
 * @return The new rate of the RTC (not guaranteed to be the same as requested)
 */
uint64_t metal_rtc_set_rate(struct metal_rtc rtc, const uint64_t rate);

/*!
 * @brief Get the compare value of the RTC
 * @return The compare value
 */
uint64_t metal_rtc_get_compare(struct metal_rtc rtc);

/*!
 * @brief Set the compare value of the RTC
 * @return The set compare value (not guaranteed to be exactly the requested
 * value)
 *
 * The RTC device might impose limits on the maximum compare value or the
 * granularity of the compare value.
 *
 * @return The value of the compare value after the set operation
 */
uint64_t metal_rtc_set_compare(struct metal_rtc rtc, const uint64_t compare);

/*!
 * @brief Get the current count of the RTC
 * @return The count
 */
uint64_t metal_rtc_get_count(struct metal_rtc rtc);

/*!
 * @brief Set the current count of the RTC
 * @return The set value of the count (not guaranteed to be exactly the
 * requested value)
 *
 * The RTC device might impose limits on the maximum value of the count
 *
 * @return The count of the RTC after the set operation
 */
uint64_t metal_rtc_set_count(struct metal_rtc rtc, const uint64_t count);

/*!
 * @brief Enable the RTC compare interrupt
 * @return 0 upon success
 */
int metal_rtc_enable_interrupt(struct metal_rtc rtc);

/*!
 * @brief Disable the RTC compare interrupt
 * @return 0 upon success
 */
int metal_rtc_disable_interrupt(struct metal_rtc rtc);

/*!
 * @brief Start or stop the RTC
 * @return 0 if the RTC was successfully started/stopped
 */
int metal_rtc_run(struct metal_rtc rtc, const enum metal_rtc_run_option option);

#endif
