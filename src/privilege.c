/* Copyright 2019 SiFive, Inc */
/* SPDX-License-Identifier: Apache-2.0 */

#include <metal/io.h>
#include <metal/privilege.h>
#include <metal/riscv.h>

void metal_privilege_drop_to_mode(enum metal_privilege_mode mode,
                                  struct metal_register_file regfile,
                                  metal_privilege_entry_point_t entry_point) {
    riscv_xlen_t mstatus;
    __asm__ volatile("csrr %0, mstatus" : "=r"(mstatus));

    /* Set xPIE bits based on current xIE bits */
    mstatus = __METAL_SET_FIELD(mstatus, RISCV_MSTATUS_UPIE,
                                __METAL_GET_FIELD(mstatus, RISCV_MSTATUS_UIE));
    mstatus = __METAL_SET_FIELD(mstatus, RISCV_MSTATUS_SPIE,
                                __METAL_GET_FIELD(mstatus, RISCV_MSTATUS_SIE));
    mstatus = __METAL_SET_FIELD(mstatus, RISCV_MSTATUS_MPIE,
                                __METAL_GET_FIELD(mstatus, RISCV_MSTATUS_MIE));

    /* Set MPP to the requested privilege mode */
    mstatus = __METAL_SET_FIELD(mstatus, RISCV_MSTATUS_MPP_MASK, mode);

    /* Write back mstatus */
    __asm__ volatile("csrw mstatus, %0" ::"r"(mstatus));

    /* Set the entry point in MEPC */
    __asm__ volatile("csrw mepc, %0" ::"r"(entry_point));

    /* Set the register file */
    __asm__ volatile("mv ra, %0" ::"r"(regfile.ra));
    __asm__ volatile("mv sp, %0" ::"r"(regfile.sp));

    __asm__ volatile("mret");
}
