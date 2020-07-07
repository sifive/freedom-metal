/* Copyright 2018 SiFive, Inc */
/* SPDX-License-Identifier: Apache-2.0 */

#include <metal/machine/platform.h>

#ifdef METAL_SIFIVE_UART0

#include <metal/clock.h>
#include <metal/cpu.h>
#include <metal/generated/sifive_uart0.h>
#include <metal/init.h>
#include <metal/io.h>
#include <metal/tty.h>
#include <metal/uart.h>

/* TXDATA Fields */
#define UART_TXEN (1 << 0)
#define UART_TXFULL (1 << 31)

/* RXDATA Fields */
#define UART_RXEN (1 << 0)
#define UART_RXEMPTY (1 << 31)

/* TXCTRL Fields */
#define UART_NSTOP (1 << 1)
#define UART_TXCNT(count) ((0x7 & count) << 16)

/* RXCTRL Fields */
#define UART_RXCNT(count) ((0x7 & count) << 16)

/* IP Fields */
#define UART_TXWM (1 << 0)
#define UART_RXWM (1 << 1)

#define UART_REG(offset) (((uintptr_t)base + offset))
#define UART_REGB(offset)                                                      \
    (__METAL_ACCESS_ONCE((__metal_io_u8 *)UART_REG(offset)))
#define UART_REGW(offset)                                                      \
    (__METAL_ACCESS_ONCE((__metal_io_u32 *)UART_REG(offset)))

static struct {
    uint64_t baud_rate;
} uart_state[__METAL_DT_NUM_UARTS] = {
    {
        .baud_rate = 0    
    },
};

static inline uint32_t get_index(struct metal_uart uart) {
    return uart.__uart_index;
}

static __inline__ int enable_parent_interrupt(struct metal_uart uart) {
    struct metal_interrupt intc = dt_uart_data[get_index(uart)].interrupt_parent;
    int id = dt_uart_data[get_index(uart)].interrupt_id;

    return metal_interrupt_enable(intc, id);
}

static __inline__ int disable_parent_interrupt(struct metal_uart uart) {
    struct metal_interrupt intc = dt_uart_data[get_index(uart)].interrupt_parent;
    int id = dt_uart_data[get_index(uart)].interrupt_id;

    return metal_interrupt_disable(intc, id);
}

int sifive_uart0_tx_interrupt_enable(struct metal_uart uart) {
    uintptr_t base = dt_uart_data[get_index(uart)].base_addr;

    UART_REGW(METAL_SIFIVE_UART0_IE) |= UART_TXWM;

    return enable_parent_interrupt(uart);
}

int sifive_uart0_tx_interrupt_disable(struct metal_uart uart) {
    uintptr_t base = dt_uart_data[get_index(uart)].base_addr;

    UART_REGW(METAL_SIFIVE_UART0_IE) &= ~UART_TXWM;

    if ((UART_REGW(METAL_SIFIVE_UART0_IE) & UART_RXWM) == 0) {
        /* Disable the UART interrupt line on the interrupt controller
         * when no UART interrupt sources are enabled */
        return disable_parent_interrupt(uart); 
    }
    return 0;
}

int sifive_uart0_rx_interrupt_enable(struct metal_uart uart) {
    uintptr_t base = dt_uart_data[get_index(uart)].base_addr;

    UART_REGW(METAL_SIFIVE_UART0_IE) |= UART_RXWM;

    return enable_parent_interrupt(uart);
}

int sifive_uart0_rx_interrupt_disable(struct metal_uart uart) {
    uintptr_t base = dt_uart_data[get_index(uart)].base_addr;

    UART_REGW(METAL_SIFIVE_UART0_IE) &= ~UART_RXWM;

    if ((UART_REGW(METAL_SIFIVE_UART0_IE) & UART_TXWM) == 0) {
        /* Disable the UART interrupt line on the interrupt controller
         * when no UART interrupt sources are enabled */
        return disable_parent_interrupt(uart); 
    }
}

int sifive_uart0_txready(struct metal_uart uart) {
    uintptr_t base = dt_uart_data[get_index(uart)].base_addr;

    return !!((UART_REGW(METAL_SIFIVE_UART0_TXDATA) & UART_TXFULL));
}

int sifive_uart0_set_tx_watermark(struct metal_uart uart, size_t level) {
    uintptr_t base = dt_uart_data[get_index(uart)].base_addr;

    UART_REGW(METAL_SIFIVE_UART0_TXCTRL) |= UART_TXCNT(level);
    return 0;
}

size_t sifive_uart0_get_tx_watermark(struct metal_uart uart) {
    uintptr_t base = dt_uart_data[get_index(uart)].base_addr;

    return ((UART_REGW(METAL_SIFIVE_UART0_TXCTRL) >> 16) & 0x7);
}

int sifive_uart0_set_rx_watermark(struct metal_uart uart, size_t level) {
    uintptr_t base = dt_uart_data[get_index(uart)].base_addr;

    UART_REGW(METAL_SIFIVE_UART0_RXCTRL) |= UART_RXCNT(level);
    return 0;
}

size_t sifive_uart0_get_rx_watermark(struct metal_uart uart) {
    uintptr_t base = dt_uart_data[get_index(uart)].base_addr;

    return ((UART_REGW(METAL_SIFIVE_UART0_RXCTRL) >> 16) & 0x7);
}

int sifive_uart0_putc(struct metal_uart uart, int c) {
    uintptr_t base = dt_uart_data[get_index(uart)].base_addr;

    while (sifive_uart0_txready(uart) != 0) {
        /* wait */
    }
    UART_REGW(METAL_SIFIVE_UART0_TXDATA) = c;
    return 0;
}

int sifive_uart0_getc(struct metal_uart uart, int *c) {
    uintptr_t base = dt_uart_data[get_index(uart)].base_addr;

    /* No seperate status register, we get status and the byte at same time */
    uint32_t ch = UART_REGW(METAL_SIFIVE_UART0_RXDATA);
    ;
    if (ch & UART_RXEMPTY) {
        *c = -1; /* aka: EOF in most of the world */
    } else {
        *c = ch & 0x0ff;
    }
    return 0;
}

int sifive_uart0_get_baud_rate(struct metal_uart uart) {
    return uart_state[get_index(uart)].baud_rate;
}

int sifive_uart0_set_baud_rate(struct metal_uart uart, int baud_rate) {
    uint32_t index = get_index(uart);
    uintptr_t base = dt_uart_data[get_index(uart)].base_addr;
    struct metal_clock clock = dt_uart_data[get_index(uart)].clock;

    uart_state[index].baud_rate = baud_rate;

    long clock_rate =  metal_clock_get_rate_hz(clock);
    UART_REGW(METAL_SIFIVE_UART0_DIV) = clock_rate / baud_rate - 1;
    UART_REGW(METAL_SIFIVE_UART0_TXCTRL) |= UART_TXEN;
    UART_REGW(METAL_SIFIVE_UART0_RXCTRL) |= UART_RXEN;

    return 0;
}

void _sifive_uart0_pre_rate_change_callback(struct metal_uart uart) {
    uintptr_t base = dt_uart_data[get_index(uart)].base_addr;
    struct metal_clock clock = dt_uart_data[get_index(uart)].clock;

    /* Detect when the TXDATA is empty by setting the transmit watermark count
     * to one and waiting until an interrupt is pending */

    UART_REGW(METAL_SIFIVE_UART0_TXCTRL) &= ~(UART_TXCNT(0x7));
    UART_REGW(METAL_SIFIVE_UART0_TXCTRL) |= UART_TXCNT(1);

    while ((UART_REGW(METAL_SIFIVE_UART0_IP) & UART_TXWM) == 0)
        ;

    /* When the TXDATA clears, the UART is still shifting out the last byte.
     * Calculate the time we must drain to finish transmitting and then wait
     * that long. */

    long bits_per_symbol =
        (UART_REGW(METAL_SIFIVE_UART0_TXCTRL) & (1 << 1)) ? 9 : 10;
    uint32_t baud_rate = uart_state[get_index(uart)].baud_rate;

    if (baud_rate != 0) {
        uint64_t ticks_to_wait = bits_per_symbol * MTIME_RATE_HZ_DEF / baud_rate;

        struct metal_cpu cpu = metal_cpu_get(metal_cpu_get_current_hartid());
        uint64_t mtime = metal_cpu_get_mtime(cpu);
        uint64_t mtime_end = mtime + ticks_to_wait;
        while (mtime <= mtime_end) {
            mtime = metal_cpu_get_mtime(cpu);
        }
    }
}

void _sifive_uart0_post_rate_change_callback(struct metal_uart uart) {
    uint32_t baud_rate = uart_state[get_index(uart)].baud_rate;
    sifive_uart0_set_baud_rate(uart, baud_rate);
}

void sifive_uart0_init(struct metal_uart uart, uint32_t baud_rate) {
    uint32_t index = get_index(uart);

    sifive_uart0_set_baud_rate(uart, baud_rate);

    int has_pinmux = dt_uart_data[index].has_pinmux;
    if (has_pinmux) {
        struct metal_gpio pinmux = dt_uart_data[index].pinmux;
        uint32_t output_sel = dt_uart_data[index].pinmux_output_selector;
        uint32_t source_sel = dt_uart_data[index].pinmux_source_selector;
        metal_gpio_enable_pinmux(pinmux, output_sel, source_sel);
    }
}

#ifdef METAL_STDOUT_SIFIVE_UART0
#ifdef __METAL_DT_STDOUT_UART_HANDLE

#ifndef __METAL_DT_STDOUT_UART_BAUD
#define __METAL_DT_STDOUT_UART_BAUD 115200
#endif

METAL_CONSTRUCTOR(metal_tty_init) {
    sifive_uart0_init(__METAL_DT_STDOUT_UART_HANDLE,
                    __METAL_DT_STDOUT_UART_BAUD);
}

int metal_tty_putc(int c) {
    return sifive_uart0_putc(__METAL_DT_STDOUT_UART_HANDLE, c);
}

int metal_tty_getc(int *c) {
    do {
        sifive_uart0_getc(__METAL_DT_STDOUT_UART_HANDLE, c);
        /* -1 means no key pressed, getc waits */
    } while (-1 == *c);
    return 0;
}

#endif /* __METAL_DT_STDOUT_UART_HANDLE */
#endif /* METAL_STDOUT_SIFIVE_UART0 */

#endif /* METAL_SIFIVE_UART0 */

typedef int no_empty_translation_units;
