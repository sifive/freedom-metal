/* Copyright 2018 SiFive, Inc */
/* SPDX-License-Identifier: Apache-2.0 */

#ifndef MEE__DRIVERS__RISCV_RV64_PMP_H
#define MEE__DRIVERS__RISCV_RV64_PMP_H

#include <mee/compiler.h>
#include <mee/pmp.h>

struct __mee_driver_vtable_riscv_rv64_pmp {
    const struct mee_pmp_vtable pmp;
};

struct __mee_driver_riscv_rv64_pmp;

void __mee_driver_riscv_rv64_pmp_init(struct mee_pmp *pmp);
int __mee_driver_riscv_rv64_pmp_set_region(struct mee_pmp *pmp, unsigned int region, struct mee_pmp_config config, size_t address);
int __mee_driver_riscv_rv64_pmp_get_region(struct mee_pmp *pmp, unsigned int region, struct mee_pmp_config *config, size_t *address);

__MEE_DECLARE_VTABLE(__mee_driver_vtable_riscv_rv64_pmp) = {
    .pmp.init          = __mee_driver_riscv_rv64_pmp_init,
    .pmp.set_region    = __mee_driver_riscv_rv64_pmp_set_region,
    .pmp.get_region    = __mee_driver_riscv_rv64_pmp_get_region,
};

struct __mee_driver_riscv_rv64_pmp {
    struct mee_pmp pmp;
    const struct __mee_driver_vtable_riscv_rv64_pmp *vtable;
    const int num_regions;
};

#endif
