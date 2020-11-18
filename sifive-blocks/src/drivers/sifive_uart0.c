/* Copyright 2018 SiFive, Inc */
/* SPDX-License-Identifier: Apache-2.0 */

#include <metal/platform.h>

#ifdef METAL_SIFIVE_UART0

#include <metal/clock.h>
#include <metal/cpu.h>
#include <metal/drivers/sifive_uart0_regs.h>
#include <metal/init.h>
#include <metal/io.h>
#include <metal/private/metal_private_sifive_uart0.h>
#include <metal/tty.h>
#include <metal/uart.h>

#define get_index(uart) ((uart).__uart_index)

static struct {
    uint64_t baud_rate;
} uart_state[__METAL_DT_NUM_UARTS] = {
    {.baud_rate = 0},
};

static __inline__ int enable_parent_interrupt(struct metal_uart uart) {
    struct metal_interrupt intc =
        dt_uart_data[get_index(uart)].interrupt_parent;
    int id = dt_uart_data[get_index(uart)].interrupt_id;

    return metal_interrupt_enable(intc, id);
}

static __inline__ int disable_parent_interrupt(struct metal_uart uart) {
    struct metal_interrupt intc =
        dt_uart_data[get_index(uart)].interrupt_parent;
    int id = dt_uart_data[get_index(uart)].interrupt_id;

    return metal_interrupt_disable(intc, id);
}

int sifive_uart0_tx_interrupt_enable(struct metal_uart uart) {
    UART_Type *UART_regs = (UART_Type *)dt_uart_data[get_index(uart)].base_addr;

    UART_regs->IE |= UART_IE_TXWM_IE_Msk;

    return enable_parent_interrupt(uart);
}

int sifive_uart0_tx_interrupt_disable(struct metal_uart uart) {
    UART_Type *UART_regs = (UART_Type *)dt_uart_data[get_index(uart)].base_addr;

    UART_regs->IE &= (~UART_IE_TXWM_IE_Msk);

    if ((UART_regs->IE & UART_IE_RXWM_IE_Msk) == 0) {
        /* Disable the UART interrupt line on the interrupt controller
         * when no UART interrupt sources are enabled */
        return disable_parent_interrupt(uart);
    }
    return 0;
}

int sifive_uart0_rx_interrupt_enable(struct metal_uart uart) {
    UART_Type *UART_regs = (UART_Type *)dt_uart_data[get_index(uart)].base_addr;

    UART_regs->IE |= UART_IE_RXWM_IE_Msk;

    return enable_parent_interrupt(uart);
}

int sifive_uart0_rx_interrupt_disable(struct metal_uart uart) {
    UART_Type *UART_regs = (UART_Type *)dt_uart_data[get_index(uart)].base_addr;

    UART_regs->IE &= (~UART_IE_RXWM_IE_Msk);

    if ((UART_regs->IE & UART_IE_TXWM_IE_Msk) == 0) {
        /* Disable the UART interrupt line on the interrupt controller
         * when no UART interrupt sources are enabled */
        return disable_parent_interrupt(uart);
    }

    return 0;
}

int sifive_uart0_txready(struct metal_uart uart) {
    UART_Type *UART_regs = (UART_Type *)dt_uart_data[get_index(uart)].base_addr;

    return !!(UART_regs->TXDATA & UART_TXDATA_FULL_Msk);
}

int sifive_uart0_set_tx_watermark(struct metal_uart uart, size_t level) {
    UART_Type *UART_regs = (UART_Type *)dt_uart_data[get_index(uart)].base_addr;

    UART_regs->TXCTRL |=
        ((level << UART_TXCTRL_TXCNT_Pos) & UART_TXCTRL_TXCNT_Msk);
    return 0;
}

size_t sifive_uart0_get_tx_watermark(struct metal_uart uart) {
    UART_Type *UART_regs = (UART_Type *)dt_uart_data[get_index(uart)].base_addr;

    return ((UART_regs->TXCTRL & UART_TXCTRL_TXCNT_Msk) >>
            UART_TXCTRL_TXCNT_Pos);
}

int sifive_uart0_set_rx_watermark(struct metal_uart uart, size_t level) {
    UART_Type *UART_regs = (UART_Type *)dt_uart_data[get_index(uart)].base_addr;

    UART_regs->RXCTRL |=
        ((level << UART_RXCTRL_RXCNT_Pos) & UART_RXCTRL_RXCNT_Msk);
    return 0;
}

size_t sifive_uart0_get_rx_watermark(struct metal_uart uart) {
    UART_Type *UART_regs = (UART_Type *)dt_uart_data[get_index(uart)].base_addr;

    return ((UART_regs->RXCTRL & UART_RXCTRL_RXCNT_Msk) >>
            UART_RXCTRL_RXCNT_Pos);
}

int sifive_uart0_putc(struct metal_uart uart, int c) {
    UART_Type *UART_regs = (UART_Type *)dt_uart_data[get_index(uart)].base_addr;

    while (sifive_uart0_txready(uart) != 0) {
        /* wait */
    }
    UART_regs->TXDATA = c;
    return 0;
}

int sifive_uart0_getc(struct metal_uart uart, int *c) {
    UART_Type *UART_regs = (UART_Type *)dt_uart_data[get_index(uart)].base_addr;

    /* No seperate status register, we get status and the byte at same time */
    uint32_t ch = UART_regs->RXDATA;
    ;
    if (ch & UART_RXDATA_EMPTY_Msk) {
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
    UART_Type *UART_regs = (UART_Type *)dt_uart_data[index].base_addr;
    struct metal_clock clock = dt_uart_data[index].clock;

    uart_state[index].baud_rate = baud_rate;

    long clock_rate = metal_clock_get_rate_hz(clock);
    UART_regs->DIV = clock_rate / baud_rate - 1;
    UART_regs->TXCTRL |= UART_TXCTRL_TXEN_Msk;
    UART_regs->RXCTRL |= UART_RXCTRL_RXEN_Msk;

    return 0;
}

void _sifive_uart0_pre_rate_change_callback(uint32_t id) {
    struct metal_uart uart = (struct metal_uart){id};
    uint32_t baud_rate = uart_state[get_index(uart)].baud_rate;

    if (baud_rate == 0)
        return;

    UART_Type *UART_regs = (UART_Type *)dt_uart_data[get_index(uart)].base_addr;

    /* Detect when the TXDATA is empty by setting the transmit watermark count
     * to one and waiting until an interrupt is pending */

    UART_regs->TXCTRL &= ~(UART_RXCTRL_RXCNT_Msk);
    UART_regs->TXCTRL |= ((1 << UART_RXCTRL_RXCNT_Pos) & UART_RXCTRL_RXCNT_Msk);

    while ((UART_regs->IP & UART_IP_TXWM_IP_Msk) == 0) {
        /* wait */
    }

    /* When the TXDATA clears, the UART is still shifting out the last byte.
     * Calculate the time we must drain to finish transmitting and then wait
     * that long. */

    long bits_per_symbol = (UART_regs->TXCTRL & UART_TXCTRL_NSTOP_Msk) ? 9 : 10;
    uint64_t ticks_to_wait = bits_per_symbol * MTIME_RATE_HZ_DEF / baud_rate;

    struct metal_cpu cpu = metal_cpu_get(metal_cpu_get_current_hartid());
    uint64_t mtime = metal_cpu_get_mtime(cpu);
    uint64_t mtime_end = mtime + ticks_to_wait;
    while (mtime <= mtime_end) {
        mtime = metal_cpu_get_mtime(cpu);
    }
}

void _sifive_uart0_post_rate_change_callback(uint32_t id) {
    struct metal_uart uart = (struct metal_uart){id};
    uint32_t baud_rate = uart_state[get_index(uart)].baud_rate;

    if (baud_rate == 0)
        return;

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

METAL_CONSTRUCTOR(init_metal_tty) {
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
