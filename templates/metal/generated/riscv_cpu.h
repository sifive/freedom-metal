/* Copyright 2020 SiFive, Inc */
/* SPDX-License-Identifier: Apache-2.0 */

#ifndef METAL__GENERATED__RISCV_CPU_H
#define METAL__GENERATED__RISCV_CPU_H

{% include 'template_comment.h' %}

#include <metal/generated/cpu.h>
#include <metal/interrupt.h>
#include <metal/drivers/sifive_buserror0.h>
#include <stdint.h>

static struct dt_cpu_data {
    uint64_t timebase;
    struct metal_interrupt interrupt_controller;
    struct metal_buserror buserror;
} dt_cpu_data[__METAL_DT_NUM_HARTS] = {
    {% for hart in harts %}
    {
        .timebase = {{ hart.timebase_frequency }},
        .interrupt_controller = (struct metal_interrupt) { {{ hart.interrupt_controller.id }} },
        .buserror = (struct metal_buserror) { {{ hart.sifive_buserror[0].id }} }
    },
    {% endfor %}
};

#endif
