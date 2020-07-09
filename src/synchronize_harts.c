/* Copyright 2019 SiFive, Inc */
/* SPDX-License-Identifier: Apache-2.0 */

#include <metal/cpu.h>

/*
 * _synchronize_harts() is called by crt0.S to cause harts > 0 to wait for
 * hart 0 to finish copying the datat section, zeroing the BSS, and running
 * the libc contstructors.
 */
__attribute__((section(".init"))) void __metal_synchronize_harts() {
#if __METAL_DT_NUM_HARTS > 1
    uint32_t hartid = metal_cpu_get_current_hartid();

    /* Disable machine interrupts as a precaution */
    metal_cpu_disable_interrupts(metal_cpu_get(hartid));
    metal_cpu_disable_ipi(metal_cpu_get(hartid));

    if (hartid == 0) {
        /* Hart 0 waits for all harts to set their MSIP bit */
        for (uint32_t i = 1; i < __METAL_DT_NUM_HARTS; i++) {
            struct metal_cpu cpu = metal_cpu_get(i);
            while (metal_cpu_get_ipi(cpu) == 0)
                ;
        }

        /* Hart 0 clears everyone's MSIP bit */
        for (uint32_t i = 1; i < __METAL_DT_NUM_HARTS; i++) {
            struct metal_cpu cpu = metal_cpu_get(i);
            metal_cpu_clear_ipi(cpu);
        }
    } else {
        /* Other harts set their MSIP bit to indicate they're ready */
        struct metal_cpu cpu = metal_cpu_get(hartid);
        metal_cpu_set_ipi(cpu);

        __asm__ volatile("fence w,rw");

        /* Wait for hart 0 to clear the MSIP bit */
        while (metal_cpu_get_ipi(cpu) == 1)
            ;
    }

#endif /* __METAL_DT_NUM_HARTS > 1 */
}
