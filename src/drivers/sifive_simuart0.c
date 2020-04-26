/* Copyright 2018 SiFive, Inc */
/* SPDX-License-Identifier: Apache-2.0 */

#include <metal/machine/platform.h>

#ifdef METAL_SIFIVE_SIMUART0

#include <metal/drivers/sifive_simuart0.h>
#include <metal/machine.h>
#include <assert.h>

/* TXDATA Fields */
#define SIMUART_TXEN               (1 <<  0)
#define SIMUART_TXFULL             (1 << 31)

/* RXDATA Fields */
#define SIMUART_RXEN               (1 <<  0)
#define SIMUART_RXEMPTY            (1 << 31)

/* TXCTRL Fields */
#define SIMUART_NSTOP              (1 <<  1)
#define SIMUART_TXCNT(count)       ((0x7 & count) << 16)

/* IP Fields */
#define SIMUART_TXWM               (1 <<  0)

#define SIMUART_REG(offset)   (((unsigned long)control_base + offset))
#define SIMUART_REGB(offset)  (__METAL_ACCESS_ONCE((__metal_io_u8  *)SIMUART_REG(offset)))
#define SIMUART_REGW(offset)  (__METAL_ACCESS_ONCE((__metal_io_u32 *)SIMUART_REG(offset)))

struct metal_interrupt *
__metal_driver_sifive_simuart0_interrupt_controller(struct metal_uart *uart)
{
    return __metal_driver_sifive_simuart0_interrupt_parent(uart);
}

int __metal_driver_sifive_simuart0_get_interrupt_id(struct metal_uart *uart)
{
    return (__metal_driver_sifive_simuart0_interrupt_line(uart) + METAL_INTERRUPT_ID_GL0);
}


int __metal_driver_sifive_simuart0_txready(struct metal_uart *uart)
{
  long control_base = __metal_driver_sifive_simuart0_control_base(uart);

  return !((SIMUART_REGW(METAL_SIFIVE_SIMUART0_TXDATA) & SIMUART_TXFULL));
}


int __metal_driver_sifive_simuart0_putc(struct metal_uart *uart, int c)
{
    long control_base = __metal_driver_sifive_simuart0_control_base(uart);

    while (!__metal_driver_sifive_simuart0_txready(uart)) {
		/* wait */
    }
    SIMUART_REGW(METAL_SIFIVE_SIMUART0_TXDATA) = c;
    return 0;
}


int __metal_driver_sifive_simuart0_getc(struct metal_uart *uart, int *c)
{
    uint32_t ch;
    long control_base = __metal_driver_sifive_simuart0_control_base(uart);
    /* No seperate status register, we get status and the byte at same time */
    ch = SIMUART_REGW(METAL_SIFIVE_SIMUART0_RXDATA);;
    if( ch & SIMUART_RXEMPTY ){
      *c = -1; /* aka: EOF in most of the world */
    } else {
      *c = ch & 0x0ff;
    }
    return 0;
}


int __metal_driver_sifive_simuart0_get_baud_rate(struct metal_uart *guart)
{
    struct __metal_driver_sifive_simuart0 *uart = (void *)guart;
    return uart->baud_rate;
}

int __metal_driver_sifive_simuart0_set_baud_rate(struct metal_uart *guart, int baud_rate)
{
    struct __metal_driver_sifive_simuart0 *uart = (void *)guart;
    long control_base = __metal_driver_sifive_simuart0_control_base(guart);
    struct metal_clock *clock = __metal_driver_sifive_simuart0_clock(guart);

    uart->baud_rate = baud_rate;

    if (clock != NULL) {
        long clock_rate = clock->vtable->get_rate_hz(clock);
        SIMUART_REGW(METAL_SIFIVE_SIMUART0_DIV) = clock_rate / baud_rate - 1;
        SIMUART_REGW(METAL_SIFIVE_SIMUART0_TXCTRL) |= SIMUART_TXEN;
        SIMUART_REGW(METAL_SIFIVE_SIMUART0_RXCTRL) |= SIMUART_RXEN;
    }
    return 0;
}

static void pre_rate_change_callback_func(void *priv)
{
    struct __metal_driver_sifive_simuart0 *uart = priv;
    long control_base = __metal_driver_sifive_simuart0_control_base((struct metal_uart *)priv);
    struct metal_clock *clock = __metal_driver_sifive_simuart0_clock((struct metal_uart *)priv);

    /* Detect when the TXDATA is empty by setting the transmit watermark count
     * to one and waiting until an interrupt is pending */

    SIMUART_REGW(METAL_SIFIVE_SIMUART0_TXCTRL) &= ~(SIMUART_TXCNT(0x7));
    SIMUART_REGW(METAL_SIFIVE_SIMUART0_TXCTRL) |= SIMUART_TXCNT(1);

    while((SIMUART_REGW(METAL_SIFIVE_SIMUART0_IP) & SIMUART_TXWM) == 0) ;

    /* When the TXDATA clears, the SIMUART is still shifting out the last byte.
     * Calculate the time we must drain to finish transmitting and then wait
     * that long. */

    long bits_per_symbol = (SIMUART_REGW(METAL_SIFIVE_SIMUART0_TXCTRL) & (1 << 1)) ? 9 : 10;
    long clk_freq = clock->vtable->get_rate_hz(clock);
    long cycles_to_wait = bits_per_symbol * clk_freq / uart->baud_rate;

    for(volatile long x = 0; x < cycles_to_wait; x++)
        __asm__("nop");
}

static void post_rate_change_callback_func(void *priv)
{
    struct __metal_driver_sifive_simuart0 *uart = priv;
    metal_uart_set_baud_rate(&uart->uart, uart->baud_rate);
}

void __metal_driver_sifive_simuart0_init(struct metal_uart *guart, int baud_rate)
{
    #if 0
    struct __metal_driver_sifive_simuart0 *uart = (void *)(guart);
    struct metal_clock *clock = __metal_driver_sifive_simuart0_clock(guart);
    struct __metal_driver_sifive_gpio0 *pinmux = __metal_driver_sifive_simuart0_pinmux(guart);

    assert(1)

    if(clock != NULL) {
        uart->pre_rate_change_callback.callback = &pre_rate_change_callback_func;
        uart->pre_rate_change_callback.priv = guart;
        metal_clock_register_pre_rate_change_callback(clock, &(uart->pre_rate_change_callback));

        uart->post_rate_change_callback.callback = &post_rate_change_callback_func;
        uart->post_rate_change_callback.priv = guart;
        metal_clock_register_post_rate_change_callback(clock, &(uart->post_rate_change_callback));
    }

    metal_uart_set_baud_rate(&(uart->uart), baud_rate);

    if (pinmux != NULL) {
        long pinmux_output_selector = __metal_driver_sifive_simuart0_pinmux_output_selector(guart);
        long pinmux_source_selector = __metal_driver_sifive_simuart0_pinmux_source_selector(guart);
        pinmux->gpio.vtable->enable_io(
            (struct metal_gpio *) pinmux,
            pinmux_output_selector,
            pinmux_source_selector
        );
    }
    #endif
}

__METAL_DEFINE_VTABLE(__metal_driver_vtable_sifive_simuart0) = {
    .uart.init          = __metal_driver_sifive_simuart0_init,
    .uart.putc          = __metal_driver_sifive_simuart0_putc,
    .uart.getc          = __metal_driver_sifive_simuart0_getc,
    .uart.get_baud_rate = __metal_driver_sifive_simuart0_get_baud_rate,
    .uart.set_baud_rate = __metal_driver_sifive_simuart0_set_baud_rate,
    .uart.controller_interrupt = __metal_driver_sifive_simuart0_interrupt_controller,
    .uart.get_interrupt_id     = __metal_driver_sifive_simuart0_get_interrupt_id,
};

#endif /* METAL_SIFIVE_SIMUART0 */

typedef int no_empty_translation_units;
