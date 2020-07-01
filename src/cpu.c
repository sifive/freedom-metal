/* Copyright 2018 SiFive, Inc */
/* SPDX-License-Identifier: Apache-2.0 */

#include <metal/cpu.h>
#include <metal/generated/cpu.h>

extern inline struct metal_cpu metal_cpu_get(unsigned int hartid);

int metal_cpu_get_current_hartid() {
    int mhartid;
    __asm__ volatile("csrr %0, mhartid" : "=r"(mhartid));
    return mhartid;
}

int metal_cpu_get_num_harts() { return __METAL_DT_NUM_HARTS; }
