/* Copyright 2020 SiFive, Inc */
/* SPDX-License-Identifier: Apache-2.0 */

#ifndef METAL__PLATFORM__SIFIVE_TEST0_H
#define METAL__PLATFORM__SIFIVE_TEST0_H

{% if shutdowns is defined %}
#define METAL_SIFIVE_TEST0_0_BASE_ADDR {{ '0x%x' % shutdowns[0].regs_by_name["control"] }}
{% endif %}

#define __METAL_HAS_SHUTDOWN
#define METAL_SIFIVE_TEST0
#define METAL_SIFIVE_TEST0_FINISHER_OFFSET 0UL

#endif
