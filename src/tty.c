/* Copyright 2018 SiFive, Inc */
/* SPDX-License-Identifier: Apache-2.0 */

#include <metal/init.h>
#include <metal/machine.h>
#include <metal/tty.h>
#include <metal/uart.h>

#if defined(__METAL_DT_STDOUT_UART_HANDLE)
/* This implementation serves as a small shim that interfaces with the first
 * UART on a system. */
int metal_tty_putc(int c) {
    if (c == '\n') {
        metal_tty_putc_raw('\r');
    }
    return metal_tty_putc_raw(c);
}

int metal_tty_putc_raw(int c) {
    return metal_uart_putc(__METAL_DT_STDOUT_UART_HANDLE, c);
}

int metal_tty_getc(int *c) {
    do {
        metal_uart_getc(__METAL_DT_STDOUT_UART_HANDLE, c);
        /* -1 means no key pressed, getc waits */
    } while (-1 == *c);
    return 0;
}

#ifndef __METAL_DT_STDOUT_UART_BAUD
#define __METAL_DT_STDOUT_UART_BAUD 115200
#endif

METAL_CONSTRUCTOR(metal_tty_init) {
    metal_uart_init(__METAL_DT_STDOUT_UART_HANDLE, __METAL_DT_STDOUT_UART_BAUD);
}
#else
/* This implementation of putc performs a very particular nop, which can be
 * found in RTL simulation commit logs (using the default rocket-chip pipeline
 * output).  Its RS1 register source contains the (hex) value of character c.
 */
int rtl_putc(int c) __attribute__((section(".text.metal.nop.putc")));
int rtl_putc(int c) {
    register unsigned long y asm ("a0") = (unsigned long) c;
    asm volatile ("andi x0, %0, 0x99" : : "r" (y));
    return 0;
}
int metal_tty_putc(int c) __attribute__((weak, alias("rtl_putc")));
#pragma message(                                                               \
    "There is no default output device, metal_tty_putc() will throw away all input.")
#endif
