/* Copyright 2018 SiFive, Inc */
/* SPDX-License-Identifier: Apache-2.0 */

#include <metal/uart.h>

/* Stubs for when no driver is present */

void metal_uart_init(struct metal_uart uart, uint32_t baud_rate)
    __attribute__((weak));
void metal_uart_init(struct metal_uart uart, uint32_t baud_rate) { return; }

int metal_uart_putc(struct metal_uart uart, int c) __attribute__((weak));
int metal_uart_putc(struct metal_uart uart, int c) { return -1; }

int metal_uart_txready(struct metal_uart uart) __attribute__((weak));
int metal_uart_txready(struct metal_uart uart) { return -1; }

int metal_uart_getc(struct metal_uart uart, int *c) __attribute__((weak));
int metal_uart_getc(struct metal_uart uart, int *c) { return -1; }

int metal_uart_get_baud_rate(struct metal_uart uart) __attribute__((weak));
int metal_uart_get_baud_rate(struct metal_uart uart) { return -1; }

int metal_uart_set_baud_rate(struct metal_uart uart, int baud_rate)
    __attribute__((weak));
int metal_uart_set_baud_rate(struct metal_uart uart, int baud_rate) {
    return -1;
}

struct metal_interrupt metal_uart_interrupt_controller(struct metal_uart uart)
    __attribute__((weak));
struct metal_interrupt metal_uart_interrupt_controller(struct metal_uart uart) {
    assert(0);
    return (struct metal_interrupt) { 0 };
}

int metal_uart_get_interrupt_id(struct metal_uart uart) __attribute__((weak));
int metal_uart_get_interrupt_id(struct metal_uart uart) { return -1; }

int metal_uart_transmit_interrupt_enable(struct metal_uart uart)
    __attribute__((weak));
int metal_uart_transmit_interrupt_enable(struct metal_uart uart) { return -1; }

int metal_uart_transmit_interrupt_disable(struct metal_uart uart)
    __attribute__((weak));
int metal_uart_transmit_interrupt_disable(struct metal_uart uart) {
    return -1;
}

int metal_uart_receive_interrupt_enable(struct metal_uart uart)
    __attribute__((weak));
int metal_uart_receive_interrupt_enable(struct metal_uart uart) { return -1; }

int metal_uart_receive_interrupt_disable(struct metal_uart uart)
    __attribute__((weak));
int metal_uart_receive_interrupt_disable(struct metal_uart uart) { return -1; }

int metal_uart_set_transmit_watermark(struct metal_uart uart, size_t level)
    __attribute__((weak));
int metal_uart_set_transmit_watermark(struct metal_uart uart, size_t level) {
    return -1;
}

size_t metal_uart_get_transmit_watermark(struct metal_uart uart)
    __attribute__((weak));
size_t metal_uart_get_transmit_watermark(struct metal_uart uart) { return 0; }

int metal_uart_set_receive_watermark(struct metal_uart uart, size_t level)
    __attribute__((weak));
int metal_uart_set_receive_watermark(struct metal_uart uart, size_t level) {
    return -1;
}

size_t metal_uart_get_receive_watermark(struct metal_uart uart)
    __attribute__((weak));
size_t metal_uart_get_receive_watermark(struct metal_uart uart) { return 0; }
