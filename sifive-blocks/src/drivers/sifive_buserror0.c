/* Copyright 2020 SiFive, Inc */
/* Copyright © 2020 Keith Packard */
/*
 * Licensed under the Apache License, Version 2.0 (the “License”); you
 * may not use this file except in compliance with the License. A copy
 * of this license can be found in the LICENSE.Apache2 file included
 * with this software. You may also obtain a copy of the License at
 *
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an “AS IS” BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or
 * implied. See the License for the specific language governing
 * permissions and limitations under the License.
 */
/* SPDX-License-Identifier: Apache-2.0 */

#include <metal/drivers/sifive_buserror0.h>
#include <metal/platform.h>
#include <stddef.h>

#ifdef METAL_SIFIVE_BUSERROR0

#include <metal/cpu.h>
#include <metal/init.h>
#include <metal/io.h>
#include <metal/private/metal_private_sifive_buserror0.h>

#define HARTID(cpu) ((cpu).__hartid)
#define CPU_HAS_BEU(cpu) HART_HAS_BEU(HARTID(cpu))
#define BEU_REGB(cpu, offset)                                                  \
    __METAL_ACCESS_ONCE(                                                       \
        (__metal_io_u8 *)(BEU_BASE_ADDR(HARTID(cpu)) + ((uintptr_t)offset)))

/* Enable all events on all hart bus error units */
METAL_CONSTRUCTOR(metal_driver_sifive_buserror_init) {
    for (int hart = 0; hart < metal_cpu_get_num_harts(); hart++) {
        struct metal_cpu cpu = metal_cpu_get(hart);
        sifive_buserror_enable_events(cpu, METAL_BUSERROR_EVENT_ALL);
    }
}

int sifive_buserror_enable_events(struct metal_cpu cpu,
                                  sifive_buserror_event_t events) {
    if (!CPU_HAS_BEU(cpu)) {
        return -1;
    }

    BEU_REGB(cpu, METAL_SIFIVE_BUSERROR0_ENABLE) |= events;

    return 0;
}

int sifive_buserror_disable_events(struct metal_cpu cpu,
                                   sifive_buserror_event_t events) {
    if (!CPU_HAS_BEU(cpu)) {
        return -1;
    }

    BEU_REGB(cpu, METAL_SIFIVE_BUSERROR0_ENABLE) &= ~events;

    return 0;
}

sifive_buserror_event_t
sifive_buserror_get_event_enabled(struct metal_cpu cpu) {
    if (!CPU_HAS_BEU(cpu)) {
        return METAL_BUSERROR_EVENT_INVALID;
    }

    return BEU_REGB(cpu, METAL_SIFIVE_BUSERROR0_ENABLE);
}

sifive_buserror_event_t sifive_buserror_get_cause(struct metal_cpu cpu) {
    if (!CPU_HAS_BEU(cpu)) {
        return METAL_BUSERROR_EVENT_INVALID;
    }

    return (1 << BEU_REGB(cpu, METAL_SIFIVE_BUSERROR0_CAUSE));
}

int sifive_buserror_clear_cause(struct metal_cpu cpu) {
    if (!CPU_HAS_BEU(cpu)) {
        return -1;
    }

    BEU_REGB(cpu, METAL_SIFIVE_BUSERROR0_CAUSE) = METAL_BUSERROR_EVENT_NONE;

    return 0;
}

uintptr_t sifive_buserror_get_event_address(struct metal_cpu cpu) {
    if (!CPU_HAS_BEU(cpu)) {
        return -1;
    }

    return BEU_REGB(cpu, METAL_SIFIVE_BUSERROR0_VALUE);
}

bool sifive_buserror_is_event_accrued(struct metal_cpu cpu,
                                      sifive_buserror_event_t events) {
    if (!CPU_HAS_BEU(cpu)) {
        return -1;
    }
    if (!(events & METAL_BUSERROR_EVENT_ANY)) {
        return false;
    }

    return !!(events & BEU_REGB(cpu, METAL_SIFIVE_BUSERROR0_ACCRUED));
}

int sifive_buserror_clear_event_accrued(struct metal_cpu cpu,
                                        sifive_buserror_event_t events) {
    if (!CPU_HAS_BEU(cpu)) {
        return -1;
    }
    if (!(events & METAL_BUSERROR_EVENT_ANY)) {
        return -2;
    }

    BEU_REGB(cpu, METAL_SIFIVE_BUSERROR0_ACCRUED) &= ~events;

    return 0;
}

int sifive_buserror_enable_platform_interrupt(struct metal_cpu cpu,
                                              sifive_buserror_event_t events) {
    if (!CPU_HAS_BEU(cpu)) {
        return -1;
    }
    if (!(events & METAL_BUSERROR_EVENT_ANY)) {
        return -2;
    }

    BEU_REGB(cpu, METAL_SIFIVE_BUSERROR0_PLATFORM_INTERRUPT) |= events;

    return 0;
}

int sifive_buserror_disable_platform_interrupt(struct metal_cpu cpu,
                                               sifive_buserror_event_t events) {
    if (!CPU_HAS_BEU(cpu)) {
        return -1;
    }
    if (!(events & METAL_BUSERROR_EVENT_ANY)) {
        return -2;
    }

    BEU_REGB(cpu, METAL_SIFIVE_BUSERROR0_PLATFORM_INTERRUPT) &= ~events;

    return 0;
}

int sifive_buserror_enable_local_interrupt(struct metal_cpu cpu,
                                           sifive_buserror_event_t events) {
    if (!CPU_HAS_BEU(cpu)) {
        return -1;
    }
    if (!(events & METAL_BUSERROR_EVENT_ANY)) {
        return -2;
    }

    BEU_REGB(cpu, METAL_SIFIVE_BUSERROR0_LOCAL_INTERRUPT) |= events;

    return 0;
}

int sifive_buserror_disable_local_interrupt(struct metal_cpu cpu,
                                            sifive_buserror_event_t events) {
    if (!CPU_HAS_BEU(cpu)) {
        return -1;
    }
    if (!(events & METAL_BUSERROR_EVENT_ANY)) {
        return -2;
    }

    BEU_REGB(cpu, METAL_SIFIVE_BUSERROR0_LOCAL_INTERRUPT) &= ~events;

    return 0;
}

#endif

typedef int no_empty_translation_units;
