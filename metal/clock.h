/* Copyright 2018 SiFive, Inc */
/* SPDX-License-Identifier: Apache-2.0 */

#ifndef METAL__CLOCK_H
#define METAL__CLOCK_H

#include <stddef.h>
#include <stdint.h>

/*!
 * @file clock.h
 * @brief API for manipulating clock sources
 *
 * The clock interface allows for controlling the rate of various clocks in the
 * system.
 */

/*!
 * @struct metal_clock
 * @brief The handle for a clock
 *
 * Clocks are defined as a `struct metal_clock`, the contents of
 * which are implementation defined. Users of the clock interface must call
 * functions which accept a `struct metal_clock` as an argument to interract
 * with the clock.
 *
 * Note that no mechanism for obtaining a pointer to a `struct metal_clock` has
 * been defined, making it impossible to call any of these functions without
 * invoking implementation-defined behavior.
 */
struct metal_clock {
    uint32_t __clock_index;
};

/*!
 * @brief Returns the current rate of the given clock
 *
 * @param clk The handle for the clock
 * @return The current rate of the clock in Hz
 */
uint64_t metal_clock_get_rate_hz(struct metal_clock clk);

/*!
 * @brief Set the current rate of a clock
 *
 * @param clk The handle for the clock
 * @param hz The desired rate in Hz
 * @return The new rate of the clock in Hz.
 *
 * Attempts to set the current rate of the given clock to as close as possible
 * to the given rate in Hz. Returns the actual value that's been selected, which
 * could be anything!
 *
 * Prior to and after the rate change of the clock, this will call the
 * registered pre- and post-rate change callbacks.
 */
uint64_t metal_clock_set_rate_hz(struct metal_clock clk, uint64_t hz);

#endif
