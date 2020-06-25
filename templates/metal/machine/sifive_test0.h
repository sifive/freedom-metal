/* Copyright 2020 SiFive, Inc */
/* SPDX-License-Identifier: Apache-2.0 */

#ifndef METAL__PLATFORM__SIFIVE_TEST0_H
#define METAL__PLATFORM__SIFIVE_TEST0_H

{% for sifive_test0 in sifive_test0s %}
#define METAL_SIFIVE_TEST0_{{ loop.index0 }}_BASE_ADDR {{ '0x%x' % sifive_test0.regs_by_name["control"] }}
{% endfor %}

#define METAL_SIFIVE_TEST0
#define METAL_SIFIVE_TEST0_FINISHER_OFFSET 0UL

#endif
