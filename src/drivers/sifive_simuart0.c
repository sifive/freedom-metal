/* Copyright 2020 SiFive, Inc */
/* SPDX-License-Identifier: Apache-2.0 */

#include <metal/machine/platform.h>

#ifdef METAL_SIFIVE_SIMUART0

#include <metal/generated/sifive_simuart0.h>
#include <metal/init.h>
#include <metal/machine.h>

/* TXDATA Fields */
#define SIMUART_TXEN (1 << 0)

#define SIMUART_REG(offset) (((unsigned long)base + offset))
#define SIMUART_REGB(offset)                                                   \
    (__METAL_ACCESS_ONCE((__metal_io_u8 *)SIMUART_REG(offset)))
#define SIMUART_REGW(offset)                                                   \
    (__METAL_ACCESS_ONCE((__metal_io_u32 *)SIMUART_REG(offset)))

static uint32_t baud_rate[__METAL_DT_NUM_UARTS] = {0};

static inline uint32_t get_index(struct metal_uart uart) {
    return uart.__uart_index;
}

struct metal_interrupt *
metal_uart_interrupt_controller(struct metal_uart uart) {
    return dt_uart_data[get_index(uart)].interrupt_parent;
}

int metal_uart_get_interrupt_id(struct metal_uart uart) {
    return dt_uart_data[get_index(uart)].interrupt_id;
}

int metal_uart_putc(struct metal_uart uart, int c) {
    long base = dt_uart_data[get_index(uart)].base_addr;

    SIMUART_REGW(METAL_SIFIVE_SIMUART0_TXDATA) = c;
    return 0;
}

int metal_uart_get_baud_rate(struct metal_uart uart) {
    return baud_rate[get_index(uart)];
}

int metal_uart_set_baud_rate(struct metal_uart uart, int baud_rate) {
    long base = dt_uart_data[get_index(uart)].base_addr;

    struct metal_clock clock = dt_uart_data[get_index(uart)].clock;
    if (clock != NULL) {
        long clock_rate = dt_clock_get_rate_hz(clock);
        baud_rate[get_index(uart)] = baud_rate;
        SIMUART_REGW(METAL_SIFIVE_SIMUART0_DIV) = clock_rate / baud_rate - 1;
        SIMUART_REGW(METAL_SIFIVE_SIMUART0_TXCTRL) |= SIMUART_TXEN;
    }
    return 0;
}

#ifdef METAL_STDOUT_SIFIVE_SIMUART0
#if defined(__METAL_DT_STDOUT_UART_HANDLE)

int metal_tty_putc(int c) {
    return metal_uart_putc(__METAL_DT_STDOUT_UART_HANDLE, c);
}

#ifndef __METAL_DT_STDOUT_UART_BAUD
#define __METAL_DT_STDOUT_UART_BAUD 115200
#endif

#endif /* __METAL_DT_STDOUT_UART_HANDLE */
#endif /* METAL_STDOUT_SIFIVE_SIMUART0 */

#endif /* METAL_SIFIVE_SIMUART0 */

typedef int no_empty_translation_units;
