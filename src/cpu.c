/* Copyright 2018 SiFive, Inc */
/* SPDX-License-Identifier: Apache-2.0 */

#include <metal/cpu.h>
#include <metal/machine.h>

struct metal_cpu *metal_cpu_get(unsigned int hartid) {
    if (hartid < __METAL_DT_MAX_HARTS) {
        return (struct metal_cpu *)__metal_cpu_table[hartid];
    }
    return NULL;
}

int metal_cpu_get_current_hartid() {
#ifdef __riscv
    int mhartid;
    __asm__ volatile("csrr %0, mhartid" : "=r"(mhartid));
    return mhartid;
#endif
}

int metal_cpu_get_num_harts() { return __METAL_DT_MAX_HARTS; }
