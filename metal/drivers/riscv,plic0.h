/* Copyright 2018 SiFive, Inc */
/* SPDX-License-Identifier: Apache-2.0 */

#ifndef METAL__DRIVERS__RISCV_PLIC0_H
#define METAL__DRIVERS__RISCV_PLIC0_H

#include <metal/compiler.h>
#include <metal/drivers/riscv,cpu.h>


struct __metal_driver_vtable_riscv_plic0 {
    const struct metal_interrupt_vtable plic_vtable;
};

void __metal_driver_riscv_plic0_init(const struct metal_interrupt *controller);
int __metal_driver_riscv_plic0_register(const struct metal_interrupt *plic,
				      int id, metal_interrupt_handler_t isr,
				      void *priv_data);
int __metal_driver_riscv_plic0_enable(const struct metal_interrupt *controller, int id);
int __metal_driver_riscv_plic0_disable(const struct metal_interrupt *controller, int id);

__METAL_DECLARE_VTABLE(__metal_driver_vtable_riscv_plic0) = {
    .plic_vtable.interrupt_init = __metal_driver_riscv_plic0_init,
    .plic_vtable.interrupt_register = __metal_driver_riscv_plic0_register,
    .plic_vtable.interrupt_enable   = __metal_driver_riscv_plic0_enable,
    .plic_vtable.interrupt_disable  = __metal_driver_riscv_plic0_disable,
};

struct __metal_driver_riscv_plic0_data {
    int init_done;
    __metal_interrupt_data metal_exint_table[];
};

struct __metal_driver_riscv_plic0 {
    const struct metal_interrupt controller;
    const struct __metal_driver_vtable_riscv_plic0 *vtable;
    const unsigned long control_base;
    const unsigned long control_size;
    const struct metal_interrupt *interrupt_parent;
    const int interrupt_line;
    const int max_priority;
    const int num_interrupts;
    const int interrupt_controller;
    struct __metal_driver_riscv_plic0_data *data;
};


#define METAL_PLIC_SOURCE_BYTE_MASK       0x7
#define METAL_PLIC_SOURCE_BYTE_SHIFT      3
#define METAL_PLIC_SOURCE_PRIORITY_SHIFT  2
#define METAL_PLIC_SOURCE_PENDING_SHIFT   0
#define METAL_PLIC_HART_ENABLE_SHIFT      7
#define METAL_PLIC_HART_THRESHOLD_SHIFT   12
#define METAL_PLIC_HART_CLAIM_SHIFT       12

#define METAL_PLIC_PRIORITY_OFFSET        0x0000UL
#define METAL_PLIC_ENABLE_OFFSET          0x2000UL
#define METAL_PLIC_THRESHOLD_OFFSET       0x200000UL
#define METAL_PLIC_CLAIM_OFFSET           0x200004UL

#endif
