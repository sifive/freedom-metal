/* Copyright 2018 SiFive, Inc */
/* SPDX-License-Identifier: Apache-2.0 */

#include <metal/clock.h>

/* These inline internal helpers are defined in metal/clock.h. */

extern __inline__ void
_metal_clock_call_all_callbacks(const metal_clock_callback *const list);
extern __inline__ metal_clock_callback *
_metal_clock_append_to_callbacks(metal_clock_callback *list,
                                 metal_clock_callback *const cb);

/* These stubbed-out implementations are for when no clock has been provided
 * to the device.
 */

uint64_t metal_clock_get_rate_hz(struct metal_clock clk) { return 0; }

uint64_t metal_clock_set_rate_hz(struct metal_clock clk, uint64_t hz) { return 0; }

void
metal_clock_register_post_rate_change_callback(struct metal_clock clk,
                                               metal_clock_callback *cb) {}
void
metal_clock_register_pre_rate_change_callback(struct metal_clock clk,
                                              metal_clock_callback *cb) {}
