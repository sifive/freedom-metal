/* Copyright 2020 SiFive, Inc */
/* SPDX-License-Identifier: Apache-2.0 */

#ifndef METAL__PLATFORM__RISCV_CLINT0_H
#define METAL__PLATFORM__RISCV_CLINT0_H

{% for clint in riscv_clint0s %}
#define METAL_RISCV_CLINT0_{{ loop.index0 }}_BASE_ADDR {{ "0x%x" % clint.regs_by_name["control"] }}
{% endfor %}

#define METAL_RISCV_CLINT0
#define METAL_RISCV_CLINT0_MSIP_BASE 0x0
#define METAL_RISCV_CLINT0_MTIMECMP_BASE 0x4000
#define METAL_RISCV_CLINT0_MTIME 0xBFF8

#endif
