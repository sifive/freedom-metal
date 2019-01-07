/* Copyright 2018 SiFive, Inc */
/* SPDX-License-Identifier: Apache-2.0 */

#ifndef MEE__DRIVERS__RISCV_PLIC0_H
#define MEE__DRIVERS__RISCV_PLIC0_H

#include <mee/compiler.h>
#include <mee/drivers/riscv,cpu.h>


struct __mee_driver_vtable_riscv_plic0 {
    struct mee_interrupt_vtable controller_vtable;
};

void __mee_driver_riscv_plic0_init(struct mee_interrupt *controller);
int __mee_driver_riscv_plic0_register(struct mee_interrupt *plic,
				      int id, mee_interrupt_handler_t isr,
				      void *priv_data);
int __mee_driver_riscv_plic0_enable(struct mee_interrupt *controller, int id);
int __mee_driver_riscv_plic0_disable(struct mee_interrupt *controller, int id);

__MEE_DECLARE_VTABLE(__mee_driver_vtable_riscv_plic0) = {
    .controller_vtable.interrupt_init = __mee_driver_riscv_plic0_init,
    .controller_vtable.interrupt_register = __mee_driver_riscv_plic0_register,
    .controller_vtable.interrupt_enable   = __mee_driver_riscv_plic0_enable,
    .controller_vtable.interrupt_disable  = __mee_driver_riscv_plic0_disable,
};

struct __mee_driver_riscv_plic0 {
    struct mee_interrupt plic0;
    const struct __mee_driver_vtable_riscv_plic0 *vtable;
    const unsigned long control_base;
    const unsigned long control_size;
    struct mee_interrupt *interrupt_parent;
    const int interrupt_line;
    int max_priority;
    int num_interrupts;
    int init_done;
    int interrupt_controller;
    __mee_interrupt_data mee_exint_table[];
};


#define MEE_PLIC_SOURCE_BYTE_MASK       0x7
#define MEE_PLIC_SOURCE_BYTE_SHIFT      3
#define MEE_PLIC_SOURCE_PRIORITY_SHIFT  2
#define MEE_PLIC_SOURCE_PENDING_SHIFT   0
#define MEE_PLIC_HART_ENABLE_SHIFT      7
#define MEE_PLIC_HART_THRESHOLD_SHIFT   12
#define MEE_PLIC_HART_CLAIM_SHIFT       12

#define MEE_PLIC_PRIORITY_OFFSET        0x0000UL
#define MEE_PLIC_ENABLE_OFFSET          0x2000UL
#define MEE_PLIC_THRESHOLD_OFFSET       0x200000UL
#define MEE_PLIC_CLAIM_OFFSET           0x200004UL

#endif
