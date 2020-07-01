/* Copyright 2018 SiFive, Inc */
/* SPDX-License-Identifier: Apache-2.0 */

#include <metal/clock.h>

/* These stubbed-out implementations are for when no clock has been provided
 * to the device.
 */

uint64_t metal_clock_get_rate_hz(struct metal_clock clk) { return 0; }

uint64_t metal_clock_set_rate_hz(struct metal_clock clk, uint64_t hz) { return 0; }
