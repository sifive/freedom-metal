/* Copyright 2018 SiFive, Inc */
/* SPDX-License-Identifier: Apache-2.0 */

#include <metal/machine/platform.h>

#ifdef METAL_RISCV_CLINT0

#include <metal/cpu.h>
#include <metal/io.h>

#define CLINT_REGW(offset) __METAL_ACCESS_ONCE((__metal_io_u32 *)(METAL_RISCV_CLINT0_0_BASE_ADDR + (offset)))

int metal_cpu_software_clear_ipi(struct metal_cpu cpu, int hartid) {
    CLINT_REGW(METAL_RISCV_CLINT0_MSIP_BASE + (4 * hartid)) = 0;
    return 0;
}

int metal_cpu_software_set_ipi(struct metal_cpu cpu, int hartid) {
    CLINT_REGW(METAL_RISCV_CLINT0_MSIP_BASE + (4 * hartid)) = 1;
    return 0;
}

int metal_cpu_get_msip(struct metal_cpu cpu, int hartid) {
    return CLINT_REGW(4 * hartid) == 1;
}

uint64_t metal_cpu_get_mtime(struct metal_cpu cpu) {
    uint64_t lo, hi;

    /* Guard against rollover when reading */
    do {
        hi = CLINT_REGW(METAL_RISCV_CLINT0_MTIME + 4);
        lo = CLINT_REGW(METAL_RISCV_CLINT0_MTIME);
    } while(hi != CLINT_REGW(METAL_RISCV_CLINT0_MTIME + 4));

    return (hi << 32) | lo;
}

int metal_cpu_set_mtimecmp(struct metal_cpu cpu, uint64_t time) {
    uint32_t hartid = cpu.__hartid;
    /* Per spec, the RISC-V MTIME/MTIMECMP registers are 64 bit,
     * and are NOT internally latched for multiword transfers.
     * Need to be careful about sequencing to avoid triggering
     * spurious interrupts: For that set the high word to a max
     * value first.
     */
    CLINT_REGW(METAL_RISCV_CLINT0_MTIMECMP_BASE + (8 * hartid) + 4) = 0xFFFFFFFFUL;
    CLINT_REGW(METAL_RISCV_CLINT0_MTIMECMP_BASE + (8 * hartid) + 0) = time;
    CLINT_REGW(METAL_RISCV_CLINT0_MTIMECMP_BASE + (8 * hartid) + 4) = time >> 32;
    return 0;
}

#endif /* METAL_RISCV_CLINT0 */

typedef int no_empty_translation_units;
