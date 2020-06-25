/* Copyright 2020 SiFive, Inc */
/* SPDX-License-Identifier: Apache-2.0 */

#ifndef METAL__GENERATED__RISCV_CPU_H
#define METAL__GENERATED__RISCV_CPU_H

{% include 'template_comment.h' %}

#include <metal/cpu.h>
#include <metal/generated/cpu.h>
#include <metal/interrupt.h>
#include <stdint.h>
#include <stdbool.h>

static const struct dt_cpu_data {
    uint64_t timebase;
    bool has_buserror;
    struct metal_buserror buserror;
} dt_cpu_data[__METAL_DT_NUM_HARTS] = {
    {% for hart in harts %}
    {
        .timebase = {{ hart.timebase_frequency[0] }},
    {% if hart.sifive_buserror is defined %}
        .has_buserror = true,
        .buserror = (struct metal_buserror) { {{ hart.sifive_buserror[0].id }} }
    {% else %}
        .has_buserror = false,
    {% endif %}
    },
    {% endfor %}
};

#endif
