/* Copyright 2020 SiFive, Inc */
/* SPDX-License-Identifier: Apache-2.0 */

#include <metal/machine/platform.h>

#ifdef METAL_SIFIVE_NB2UART0

#include <metal/drivers/sifive_nb2uart0.h>
#include <metal/machine.h>
#include <stdio.h>
#include <stdlib.h>

#define MAX_COUNT	10000

struct metal_interrupt *
__metal_driver_sifive_nb2uart0_interrupt_controller(struct metal_uart *uart)
{
	return __metal_driver_sifive_nb2uart0_interrupt_parent(uart);
}

int __metal_driver_sifive_nb2uart0_get_interrupt_id(struct metal_uart *uart)
{
	long control_base = __metal_driver_sifive_nb2uart0_control_base(uart);

 	UART_REGW(METAL_SIFIVE_NB2UART0_IER) =  1;//temp fix
	return __metal_driver_sifive_nb2uart0_interrupt_line(uart);
}

int __metal_driver_sifive_nb2uart0_txready(struct metal_uart *uart)
{
	long control_base = __metal_driver_sifive_nb2uart0_control_base(uart);

	return (UART_REGW(METAL_SIFIVE_NB2UART0_USR) & UART_TFNF);
}

int __metal_driver_sifive_nb2uart0_putc(struct metal_uart *uart, int c)
{
	long control_base = __metal_driver_sifive_nb2uart0_control_base(uart);
	uint32_t timeout=0;

	while (!(UART_REGW(METAL_SIFIVE_NB2UART0_LSR) & UART_LSR_THRE))
	{
		if(timeout > MAX_COUNT) {
			printf("Exit from function \"__metal_driver_sifive_nb2uart0_putc()\" due to timeout");
			exit(1);
		}
		else
			timeout++;
	}

	UART_REGW(METAL_SIFIVE_NB2UART0_THR) = c;

	return 0;
}

int __metal_driver_sifive_nb2uart0_getc(struct metal_uart *uart, int *c)
{
	uint32_t ch = 0;
	long control_base = __metal_driver_sifive_nb2uart0_control_base(uart);


	if (!(UART_REGW(METAL_SIFIVE_NB2UART0_LSR) & UART_LSR_DR))
		return -1;
	else
	ch = UART_REGW(METAL_SIFIVE_NB2UART0_RBR);

	*c = ch & 0x0FF;
	
	/*
	if(UART_REGW(METAL_SIFIVE_NB2UART0_USR) & UART_RFNE) {
		*c = -1;
	} 
	else {
		*c = ch & 0x0FF;
	}*/

	return 0;
}

int __metal_driver_sifive_nb2uart0_get_baud_rate(struct metal_uart *guart)
{
	struct __metal_driver_sifive_nb2uart0 *uart = (void *)guart;

	return uart->baud_rate;
}

int __metal_driver_sifive_nb2uart0_set_baud_rate(struct metal_uart *guart, int baud_rate)
{
	struct __metal_driver_sifive_nb2uart0 *uart = (void *)guart;
	long control_base = __metal_driver_sifive_nb2uart0_control_base(guart);
	struct metal_clock *clock = __metal_driver_sifive_nb2uart0_clock(guart);

	uart->baud_rate = baud_rate;

	if (clock != NULL) {
		long clock_rate = clock->vtable->get_rate_hz(clock);

		uart->divisor=DIV_ROUND_CLOSEST(clock_rate, 16 * baud_rate);

		/* set DLAB to access DLL and DLH registers */
		UART_REGW(METAL_SIFIVE_NB2UART0_LCR) &= ~(UART_DLAB);
                UART_REGW(METAL_SIFIVE_NB2UART0_LCR) |=  (UART_DLAB);

		/* set clock divisor */
		UART_REGW(METAL_SIFIVE_NB2UART0_DLL) = (uart->divisor & 0x00FF);
                UART_REGW(METAL_SIFIVE_NB2UART0_DLH) = ((uart->divisor & 0xFF00) >> 8);

		/* clear DLAB */
                UART_REGW(METAL_SIFIVE_NB2UART0_LCR) &= ~(UART_DLAB);
	}

	return 0;
}

static void pre_rate_change_callback_func(void *priv)
{
}

static void post_rate_change_callback_func(void *priv)
{
	struct __metal_driver_sifive_nb2uart0 *uart = priv;
	metal_uart_set_baud_rate(&uart->uart, uart->baud_rate);
}

void __metal_driver_sifive_nb2uart0_init(struct metal_uart *guart, int baud_rate)
{
	uint32_t val = 0;
	uint32_t timeout=0;
	long control_base = __metal_driver_sifive_nb2uart0_control_base(guart);
	struct __metal_driver_sifive_nb2uart0 *uart = (void *)(guart);
	struct metal_clock *clock = __metal_driver_sifive_nb2uart0_clock(guart);
//	struct __metal_driver_sifive_nb2gpio0 *pinmux = __metal_driver_sifive_nb2uart0_pinmux(guart);

	if(clock != NULL) {
		uart->pre_rate_change_callback.callback = &pre_rate_change_callback_func;
		uart->pre_rate_change_callback.priv = guart;
		metal_clock_register_pre_rate_change_callback(clock, &(uart->pre_rate_change_callback));

		uart->post_rate_change_callback.callback = &post_rate_change_callback_func;
		uart->post_rate_change_callback.priv = guart;
		metal_clock_register_post_rate_change_callback(clock, &(uart->post_rate_change_callback));
	}


/*	if (pinmux != NULL) {
		long pinmux_output_selector = __metal_driver_sifive_nb2uart0_pinmux_output_selector(guart);
		long pinmux_source_selector = __metal_driver_sifive_nb2uart0_pinmux_source_selector(guart);
		pinmux->gpio.vtable->enable_io(
			(struct metal_gpio *) pinmux,
			pinmux_output_selector,
			pinmux_source_selector
		);
	}*/
	
	if(uart != NULL) {

		while (!(UART_REGW(METAL_SIFIVE_NB2UART0_LSR) & UART_LSR_TEMT))
		{
			metal_uart_set_baud_rate(&(uart->uart), baud_rate);
			if(timeout > MAX_COUNT) {
				printf("Exit from function \"__metal_driver_sifive_nb2uart0_init()\" due to timeout");
				exit(1);
			}
			else
				timeout++;
		}

		uart->setting=(unsigned int)uart_line_8n1;

		/* set line control register value */
		UART_REGW(METAL_SIFIVE_NB2UART0_LCR) = uart->setting;

		/*Interrupt Enable*/
		//UART_REGW(METAL_SIFIVE_NB2UART0_IER) = uart->int_enable;


		//UART_REGW(METAL_SIFIVE_NB2UART0_MCR) = uart->int_enable;
		//UART_REGW(METAL_SIFIVE_NB2UART0_FCR) = uart->int_enable;


		/* attempt to determine hardware parameters */
		val = UART_REGW(METAL_SIFIVE_NB2UART0_CPR);
		if(val != 0x0) {
			/* Enable transmit and receive FIFO registers */
			UART_REGW(METAL_SIFIVE_NB2UART0_FCR) &= !(UART_FIFOE);
			UART_REGW(METAL_SIFIVE_NB2UART0_FCR) |= (UART_FIFOE);

			/* Reset control portion of transmit and receive FIFOs */
			UART_REGW(METAL_SIFIVE_NB2UART0_FCR) &= !(UART_XFIFOR);
			UART_REGW(METAL_SIFIVE_NB2UART0_FCR) |= (UART_XFIFOR);
			UART_REGW(METAL_SIFIVE_NB2UART0_FCR) &= !(UART_RFIFOR);
			UART_REGW(METAL_SIFIVE_NB2UART0_FCR) |= (UART_RFIFOR);
		}
	}	
}

void __metal_driver_sifive_nb2uart0_reinit(struct metal_uart *guart, int baud_rate, uart_dls data, uart_stop_bits stop, uart_parity parity)
{
	uint32_t val = 0;
	uint32_t timeout=0;
	long control_base = __metal_driver_sifive_nb2uart0_control_base(guart);
	struct __metal_driver_sifive_nb2uart0 *uart = (void *)(guart);
	struct metal_clock *clock = __metal_driver_sifive_nb2uart0_clock(guart);
//	struct __metal_driver_sifive_nb2gpio0 *pinmux = __metal_driver_sifive_nb2uart0_pinmux(guart);

	if(clock != NULL) {
		uart->pre_rate_change_callback.callback = &pre_rate_change_callback_func;
		uart->pre_rate_change_callback.priv = guart;
		metal_clock_register_pre_rate_change_callback(clock, &(uart->pre_rate_change_callback));

		uart->post_rate_change_callback.callback = &post_rate_change_callback_func;
		uart->post_rate_change_callback.priv = guart;
		metal_clock_register_post_rate_change_callback(clock, &(uart->post_rate_change_callback));
	}

	metal_uart_set_baud_rate(&(uart->uart), baud_rate);

/*	if (pinmux != NULL) {
		long pinmux_output_selector = __metal_driver_sifive_nb2uart0_pinmux_output_selector(guart);
		long pinmux_source_selector = __metal_driver_sifive_nb2uart0_pinmux_source_selector(guart);
		pinmux->gpio.vtable->enable_io(
			(struct metal_gpio *) pinmux,
			pinmux_output_selector,
			pinmux_source_selector
		);
	}*/

	if(uart != NULL) {

		while (!(UART_REGW(METAL_SIFIVE_NB2UART0_LSR) & UART_LSR_TEMT))
		{		
			metal_uart_set_baud_rate(&(uart->uart), baud_rate);
			if(timeout > MAX_COUNT) {
				printf("Exit from function \"__metal_driver_sifive_nb2uart0_reinit()\" due to timeout");
				exit(1);
			}
			else
				timeout++;
		}
		uart->setting=(unsigned int)uart_line_8n1;

		/* set line control register value */
		UART_REGW(METAL_SIFIVE_NB2UART0_LCR) = uart->setting;

		/*Interrupt Enable*/
		//UART_REGW(METAL_SIFIVE_NB2UART0_IER) = uart->int_enable;


		//UART_REGW(METAL_SIFIVE_NB2UART0_MCR) = uart->int_enable;
		//UART_REGW(METAL_SIFIVE_NB2UART0_FCR) = uart->int_enable;


		/* attempt to determine hardware parameters */
		val = UART_REGW(METAL_SIFIVE_NB2UART0_CPR);
		if(val != 0x0) {
			/* Enable transmit and receive FIFO registers */
			UART_REGW(METAL_SIFIVE_NB2UART0_FCR) &= !(UART_FIFOE);
			UART_REGW(METAL_SIFIVE_NB2UART0_FCR) |= (UART_FIFOE);

			/* Reset control portion of transmit and receive FIFOs */
			UART_REGW(METAL_SIFIVE_NB2UART0_FCR) &= !(UART_XFIFOR);
			UART_REGW(METAL_SIFIVE_NB2UART0_FCR) |= (UART_XFIFOR);
			UART_REGW(METAL_SIFIVE_NB2UART0_FCR) &= !(UART_RFIFOR);
			UART_REGW(METAL_SIFIVE_NB2UART0_FCR) |= (UART_RFIFOR);
		}
	}

	switch(data)
	{
		case METAL_UART_5_BITS:
			UART_REGW(METAL_SIFIVE_NB2UART0_LCR) &= !(UART_LCR_WLS_8);
			break;

		case METAL_UART_6_BITS:
			UART_REGW(METAL_SIFIVE_NB2UART0_LCR) &= !(UART_LCR_WLS_8);
			UART_REGW(METAL_SIFIVE_NB2UART0_LCR) |= (UART_LCR_WLS_6);
			break;

		case METAL_UART_7_BITS:
			UART_REGW(METAL_SIFIVE_NB2UART0_LCR) &= !(UART_LCR_WLS_8);
			UART_REGW(METAL_SIFIVE_NB2UART0_LCR) |= (UART_LCR_WLS_7);
			break;

		case METAL_UART_8_BITS:
			UART_REGW(METAL_SIFIVE_NB2UART0_LCR) |= (UART_LCR_WLS_8);
			break;

		default:
			break;
	}

	switch(stop)
	{
		case METAL_UART_1_STOP:
			UART_REGW(METAL_SIFIVE_NB2UART0_LCR) &= !(UART_LCR_STB);
			break;

		case METAL_UART_2_STOP:
			UART_REGW(METAL_SIFIVE_NB2UART0_LCR) |= (UART_LCR_STB);
			break;

		default:
			break;
	}

	switch(parity)
	{
		case METAL_UART_NO_PARITY:
			UART_REGW(METAL_SIFIVE_NB2UART0_LCR) &= !(UART_LCR_PEN);
			break;

		case METAL_UART_ODD_PARITY:
			UART_REGW(METAL_SIFIVE_NB2UART0_LCR) |= (UART_LCR_PEN);
			UART_REGW(METAL_SIFIVE_NB2UART0_LCR) &= !(UART_LCR_EPS);
			break;

		case METAL_UART_EVEN_PARITY:
			UART_REGW(METAL_SIFIVE_NB2UART0_LCR) |= (UART_LCR_PEN);
			UART_REGW(METAL_SIFIVE_NB2UART0_LCR) |= (UART_LCR_EPS);
			break;

		default:
			break;
	}
}

struct metal_interrupt *__metal_driver_sifive_nb2uart0_get_interrupt(struct metal_uart *guart)
{
	return __metal_driver_sifive_nb2uart0_interrupt_parent(guart);
}

__METAL_DEFINE_VTABLE(__metal_driver_vtable_sifive_nb2uart0) = {
    .uart.init          = __metal_driver_sifive_nb2uart0_init,
    .uart.reinit	= __metal_driver_sifive_nb2uart0_reinit,
    .uart.putc          = __metal_driver_sifive_nb2uart0_putc,
    .uart.getc          = __metal_driver_sifive_nb2uart0_getc,
    .uart.get_baud_rate = __metal_driver_sifive_nb2uart0_get_baud_rate,
    .uart.set_baud_rate = __metal_driver_sifive_nb2uart0_set_baud_rate,
    .uart.controller_interrupt = __metal_driver_sifive_nb2uart0_interrupt_controller,
    .uart.get_interrupt_id     = __metal_driver_sifive_nb2uart0_get_interrupt_id,
};

#endif /* METAL_SIFIVE_NB2UART0 */

typedef int no_empty_translation_units;
