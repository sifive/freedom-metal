/* Copyright 2018 SiFive, Inc */
/* SPDX-License-Identifier: Apache-2.0 */

#ifndef METAL__DRIVERS__SIFIVE_UART0_H
#define METAL__DRIVERS__SIFIVE_UART0_H

#include <metal/drivers/sifive,gpio0.h>
#include <metal/drivers/riscv,plic0.h>
#include <metal/clock.h>
#include <metal/io.h>
#include <metal/uart.h>
#include <metal/compiler.h>

struct __metal_driver_vtable_sifive_uart0 {
    const struct metal_uart_vtable uart;
};

struct __metal_driver_sifive_uart0;

void __metal_driver_sifive_uart0_init(struct metal_uart *uart, int baud_rate);
int __metal_driver_sifive_uart0_putc(struct metal_uart *uart, unsigned char c);
int __metal_driver_sifive_uart0_getc(struct metal_uart *uart, unsigned char *c);
int __metal_driver_sifive_uart0_get_baud_rate(struct metal_uart *guart);
int __metal_driver_sifive_uart0_set_baud_rate(struct metal_uart *guart, int baud_rate);
struct metal_interrupt* __metal_driver_sifive_uart0_interrupt_controller(struct metal_uart *uart);
int __metal_driver_sifive_uart0_get_interrupt_id(struct metal_uart *uart);

__METAL_DECLARE_VTABLE(__metal_driver_vtable_sifive_uart0) = {
    .uart.init          = __metal_driver_sifive_uart0_init,
    .uart.putc          = __metal_driver_sifive_uart0_putc,
    .uart.getc          = __metal_driver_sifive_uart0_getc,
    .uart.get_baud_rate = __metal_driver_sifive_uart0_get_baud_rate,
    .uart.set_baud_rate = __metal_driver_sifive_uart0_set_baud_rate,
    .uart.controller_interrupt = __metal_driver_sifive_uart0_interrupt_controller,
    .uart.get_interrupt_id     = __metal_driver_sifive_uart0_get_interrupt_id,
};

struct __metal_driver_sifive_uart0 {
    struct metal_uart uart;
    const struct __metal_driver_vtable_sifive_uart0 *vtable;
    struct metal_clock *clock;
    const unsigned long control_base;
    const unsigned long control_size;
    unsigned long baud_rate;
    struct __metal_driver_sifive_gpio0 *pinmux;
    const unsigned long pinmux_output_selector;
    const unsigned long pinmux_source_selector;
    struct metal_interrupt *interrupt_parent;
    const int num_interrupts;
    const int interrupt_line;
};


#endif
