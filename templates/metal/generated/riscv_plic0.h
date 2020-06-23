/* Copyright 2020 SiFive, Inc */
/* SPDX-License-Identifier: Apache-2.0 */

#ifndef METAL__DRIVERS__SIFIVE_UART0_H
#define METAL__DRIVERS__SIFIVE_UART0_H

{% include 'template_comment.h' %}

#include <metal/interrupt.h>
#include <metal/machine/platform.h>
#include <stdint.h>

{% if riscv_plic0s is defined %}

#define __METAL_DT_NUM_RISCV_PLIC0S {{ riscv_plic0s|length }}

static const struct dt_intc_data {
    uintptr_t base;
    uint32_t num_interrupts;
    uint32_t context_id;
    uint32_t max_priority;
    struct metal_interrupt interrupt_parent;
    uint32_t interrupt_id;
} dt_intc_data[__METAL_DT_NUM_RISCV_PLIC0S] = {
    {% for intc in riscv_plic0s %}
    {
        .base = METAL_RISCV_PLIC0_{{ intc.id }}_BASE_ADDR,

        .num_interrupts = {{ intc.riscv_ndev[0] }},
        .context_id = 0,
        .max_priority = {{ intc.riscv_max_priority[0] }},
        .interrupt_parent = (struct metal_interrupt) { {{ intc.interrupts_extended[0].id }} },
        .interrupt_id = {{ intc.interrupts_extended[1] }},
    },
    {% endfor %}
};

{% set driver_string = to_snakecase(riscv_plic0s[0].interrupts_extended[0].compatible[0]) %}
{% include 'interrupt_dispatch.h' %}

{% endif %}

#endif
