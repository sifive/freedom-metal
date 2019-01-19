/* Copyright 2018 SiFive, Inc */
/* SPDX-License-Identifier: Apache-2.0 */

#ifndef MEE__CLOCK_H
#define MEE__CLOCK_H

/*! 
 * @file clock.h
 * @brief API for manipulating clock sources
 *
 * The clock interface allows for controlling the rate of various clocks in the system.
 */

struct mee_clock;

#include <stddef.h>

/* The generic interface to all clocks. */
struct __mee_clock_vtable {
    long (*get_rate_hz)(const struct mee_clock *clk);
    long (*set_rate_hz)(struct mee_clock *clk, long hz);
};

/*!
 * @brief Function signature of clock pre-rate change callbacks
 */
typedef void (*mee_clock_pre_rate_change_callback)(void *priv);

/*!
 * @brief Function signature of clock post-rate change callbacks
 */
typedef void (*mee_clock_post_rate_change_callback)(void *priv);

/*!
 * @struct mee_clock
 * @brief The handle for a clock
 *
 * Clocks are defined as a pointer to a `struct mee_clock`, the contents of which
 * are implementation defined. Users of the clock interface must call functions
 * which accept a `struct mee_clock *` as an argument to interract with the clock.
 *
 * Note that no mechanism for obtaining a pointer to a `struct mee_clock` has been
 * defined, making it impossible to call any of these functions without invoking
 * implementation-defined behavior.
 */
struct mee_clock {
    const struct __mee_clock_vtable *vtable;

    /* Pre-rate change callback */
    mee_clock_pre_rate_change_callback pre_rate_change_callback;
    void *pre_rate_change_callback_priv;

    /* Post-rate change callback */
    mee_clock_post_rate_change_callback post_rate_change_callback;
    void *post_rate_change_callback_priv;
};

/*!
 * @brief Returns the current rate of the given clock
 *
 * @param clk The handle for the clock
 * @return The current rate of the clock in Hz
 */
inline long mee_clock_get_rate_hz(const struct mee_clock *clk) { return clk->vtable->get_rate_hz(clk); }

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
 * Prior to and after the rate change of the clock, this will call the registered
 * pre- and post-rate change callbacks.
 */
inline long mee_clock_set_rate_hz(struct mee_clock *clk, long hz)
{
    if(clk->pre_rate_change_callback != NULL)
        clk->pre_rate_change_callback(clk->pre_rate_change_callback_priv);

    long out = clk->vtable->set_rate_hz(clk, hz);

    if (clk->post_rate_change_callback != NULL)
        clk->post_rate_change_callback(clk->post_rate_change_callback_priv);

    return out;
}

/*!
 * @brief Register a callback that must be called before a rate change
 *
 * @param clk The handle for the clock
 * @param cb The callback to be registered
 * @param priv Private data for the callback handler
 */
inline void mee_clock_register_pre_rate_change_callback(struct mee_clock *clk, mee_clock_pre_rate_change_callback cb, void *priv)
{
    clk->pre_rate_change_callback = cb;
    clk->pre_rate_change_callback_priv = priv;
}

/*!
 * @brief Registers a callback that must be called after a rate change
 *
 * @param clk The handle for the clock
 * @param cb The callback to be registered
 * @param priv Private data for the callback handler
 */
inline void mee_clock_register_post_rate_change_callback(struct mee_clock *clk, mee_clock_post_rate_change_callback cb, void *priv)
{
    clk->post_rate_change_callback = cb;
    clk->post_rate_change_callback_priv = priv;
}

#endif
