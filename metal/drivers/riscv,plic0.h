/* Copyright 2018 SiFive, Inc */
/* SPDX-License-Identifier: Apache-2.0 */

#ifndef METAL__DRIVERS__RISCV_PLIC0_H
#define METAL__DRIVERS__RISCV_PLIC0_H

#include <metal/compiler.h>
#include <metal/drivers/riscv,cpu.h>


struct __metal_driver_vtable_riscv_plic0 {
    struct metal_interrupt_vtable plic_vtable;
};

void __metal_driver_riscv_plic0_init(struct metal_interrupt *controller);
int __metal_driver_riscv_plic0_register(struct metal_interrupt *plic,
				      int id, metal_interrupt_handler_t isr,
				      void *priv_data);
int __metal_driver_riscv_plic0_enable(struct metal_interrupt *controller, int id);
int __metal_driver_riscv_plic0_disable(struct metal_interrupt *controller, int id);

__METAL_DECLARE_VTABLE(__metal_driver_vtable_riscv_plic0) = {
    .plic_vtable.interrupt_init = __metal_driver_riscv_plic0_init,
    .plic_vtable.interrupt_register = __metal_driver_riscv_plic0_register,
    .plic_vtable.interrupt_enable   = __metal_driver_riscv_plic0_enable,
    .plic_vtable.interrupt_disable  = __metal_driver_riscv_plic0_disable,
};

#define __METAL_MACHINE_MACROS
#include <metal/machine.h>
struct __metal_driver_riscv_plic0 {
    struct metal_interrupt controller;
    const struct __metal_driver_vtable_riscv_plic0 *vtable;
    const unsigned long control_base;
    const unsigned long control_size;
    struct metal_interrupt *interrupt_parent;
    const int interrupt_line;
    int max_priority;
    int num_interrupts;
    int init_done;
    int interrupt_controller;
    metal_interrupt_handler_t metal_exint_table[__METAL_PLIC_SUBINTERRUPTS];
    __metal_interrupt_data metal_exdata_table[__METAL_PLIC_SUBINTERRUPTS];
};
#undef __METAL_MACHINE_MACROS


#define METAL_PLIC_SOURCE_MASK            0x1F
#define METAL_PLIC_SOURCE_SHIFT           5
#define METAL_PLIC_SOURCE_PRIORITY_SHIFT  2
#define METAL_PLIC_SOURCE_PENDING_SHIFT   0

#define METAL_PLIC_PRIORITY_OFFSET        0x0000UL
#define METAL_PLIC_ENABLE_OFFSET          0x2000UL
#define METAL_PLIC_THRESHOLD_OFFSET       0x200000UL
#define METAL_PLIC_CLAIM_OFFSET           0x200004UL

#endif
