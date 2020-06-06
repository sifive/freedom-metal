#ifndef METAL__DRIVERS__SIFIVE_NB2UART_H
#define METAL__DRIVERS__SIFIVE_NB2UART_H

#include <metal/drivers/sifive_gpio0.h>
#include <metal/drivers/riscv_plic0.h>
#include <metal/clock.h>
#include <metal/io.h>
#include <metal/uart.h>
#include <metal/compiler.h>

#define SIFIVE_NB2UART_BUSY	-1
#define SIFIVE_NB2UART_TXERR	-2

#define UART_REG(offset)   (((unsigned long)control_base + offset))
#define UART_REGB(offset)  (__METAL_ACCESS_ONCE((__metal_io_u8  *)UART_REG(offset)))
#define UART_REGW(offset)  (__METAL_ACCESS_ONCE((__metal_io_u32 *)UART_REG(offset)))

#define DIV_ROUND_CLOSEST(x, divisor)(			\
{							\
	typeof(x) __x = x;				\
	typeof(divisor) __d = divisor;			\
	(((typeof(x))-1) > 0 ||				\
	 ((typeof(divisor))-1) > 0 || (__x) > 0) ?	\
		(((__x) + ((__d) / 2)) / (__d)) :	\
		(((__x) - ((__d) / 2)) / (__d));	\
}							\
)

struct __metal_driver_vtable_sifive_nb2uart0 {
	const struct metal_uart_vtable uart;
};

struct __metal_driver_sifive_nb2uart0;


/*
 * These are the definitions for the Modem Control Register
 */
#define UART_MCR_DTR	0x01		/* DTR   */
#define UART_MCR_RTS	0x02		/* RTS   */
#define UART_MCR_OUT1	0x04		/* Out 1 */
#define UART_MCR_OUT2	0x08		/* Out 2 */
#define UART_MCR_LOOP	0x10		/* Enable loopback test mode */
#define UART_MCR_AFE	0x20		/* Enable auto-RTS/CTS */
#define UART_MCR_DMA_EN	0x04
#define UART_MCR_TX_DFR	0x08

/*
 * These are the definitions for the Line Control Register
 *
 * Note: if the word length is 5 bits (UART_LCR_WLEN5), then setting
 * UART_LCR_STOP will select 1.5 stop bits, not 2 stop bits.
 */
#define UART_LCR_WLS_MSK 0x03		/* character length select mask */
#define UART_LCR_WLS_5	0x00		/* 5 bit character length */
#define UART_LCR_WLS_6	0x01		/* 6 bit character length */
#define UART_LCR_WLS_7	0x02		/* 7 bit character length */
#define UART_LCR_WLS_8	0x03		/* 8 bit character length */
#define UART_LCR_STB	0x04		/* # stop Bits, off=1, on=1.5 or 2) */
#define UART_LCR_PEN	0x08		/* Parity eneble */
#define UART_LCR_EPS	0x10		/* Even Parity Select */
#define UART_LCR_STKP	0x20		/* Stick Parity */
#define UART_LCR_SBRK	0x40		/* Set Break */
#define UART_LCR_BKSE	0x80		/* Bank select enable */
#define UART_LCR_DLAB	0x80		/* Divisor latch access bit */

/*
 * These are the definitions for the Line Status Register
 */
#define UART_LSR_DR	0x01		/* Data ready */
#define UART_LSR_OE	0x02		/* Overrun */
#define UART_LSR_PE	0x04		/* Parity error */
#define UART_LSR_FE	0x08		/* Framing error */
#define UART_LSR_BI	0x10		/* Break */
#define UART_LSR_THRE	0x20		/* Xmit holding register empty */
#define UART_LSR_TEMT	0x40		/* Xmitter empty */
#define UART_LSR_ERR	0x80		/* Error */

#define UART_MSR_DCD	0x80		/* Data Carrier Detect */
#define UART_MSR_RI	0x40		/* Ring Indicator */
#define UART_MSR_DSR	0x20		/* Data Set Ready */
#define UART_MSR_CTS	0x10		/* Clear to Send */
#define UART_MSR_DDCD	0x08		/* Delta DCD */
#define UART_MSR_TERI	0x04		/* Trailing edge ring indicator */
#define UART_MSR_DDSR	0x02		/* Delta DSR */
#define UART_MSR_DCTS	0x01		/* Delta CTS */

/*
 * These are the definitions for the Interrupt Identification Register
 */
#define UART_IIR_NO_INT	0x01	/* No interrupts pending */
#define UART_IIR_ID	0x06	/* Mask for the interrupt ID */
#define UART_IIR_MSI	0x00	/* Modem status interrupt */
#define UART_IIR_THRI	0x02	/* Transmitter holding register empty */
#define UART_IIR_RDI	0x04	/* Receiver data interrupt */
#define UART_IIR_RLSI	0x06	/* Receiver line status interrupt */

/*
 * These are the definitions for the Interrupt Enable Register
 */
#define UART_IER_MSI	0x08	/* Enable Modem status interrupt */
#define UART_IER_RLSI	0x04	/* Enable receiver line status interrupt */
#define UART_IER_THRI	0x02	/* Enable Transmitter holding register int. */
#define UART_IER_RDI	0x01	/* Enable receiver data interrupt */

/* UART Status Fields */
#define UART_BUSY               (1 << 0)
#define UART_TFNF 		(1 << 1)
#define UART_RFNE		(1 << 3)

/* Line Status Fields */
#define UART_THRE		(1 << 5)

/* Line Control Fields */
#define UART_DLAB		(1 << 7)

/* FIFO Control Fields */
#define UART_FIFOE		(1 << 0)
#define UART_RFIFOR		(1 << 1)
#define UART_XFIFOR		(1 << 2)

/* useful defaults for LCR */
#define UART_LCR_8N1	0x03

/*! @brief Enumeration type for UART line control settings. */
typedef enum
{
    uart_line_5n1 = 0x00,   /* 5 data bits, no parity, 1 stop bit */
    uart_line_5n1_5 = 0x04, /* 5 data bits, no parity, 1.5 stop bits */
    uart_line_5e1 = 0x18,   /* 5 data bits, even parity, 1 stop bit */
    uart_line_5e1_5 = 0x1c, /* 5 data bits, even parity, 1.5 stop bits */
    uart_line_5o1 = 0x08,   /* 5 data bits, odd parity, 1 stop bit */
    uart_line_5o1_5 = 0x0c, /* 5 data bits, odd parity, 1.5 stop bits */
    uart_line_6n1 = 0x01,   /* 6 data bits, no parity, 1 stop bit */
    uart_line_6n2 = 0x05,   /* 6 data bits, no parity, 2 stop bits */
    uart_line_6e1 = 0x19,   /* 6 data bits, even parity, 1 stop bit */
    uart_line_6e2 = 0x1d,   /* 6 data bits, even parity, 2 stop bits */
    uart_line_6o1 = 0x09,   /* 6 data bits, odd parity, 1 stop bit */
    uart_line_6o2 = 0x0d,   /* 6 data bits, odd parity, 2 stop bits */
    uart_line_7n1 = 0x02,   /* 7 data bits, no parity, 1 stop bit */
    uart_line_7n2 = 0x06,   /* 7 data bits, no parity, 2 stop bits */
    uart_line_7e1 = 0x1a,   /* 7 data bits, even parity, 1 stop bit */
    uart_line_7e2 = 0x1e,   /* 7 data bits, even parity, 2 stop bits */
    uart_line_7o1 = 0x0a,   /* 7 data bits, odd parity, 1 stop bit */
    uart_line_7o2 = 0x0e,   /* 7 data bits, odd parity, 2 stop bits */
    uart_line_8n1 = 0x03,   /* 8 data bits, no parity, 1 stop bit */
    uart_line_8n2 = 0x07,   /* 8 data bits, no parity, 2 stop bits */
    uart_line_8e1 = 0x1b,   /* 8 data bits, even parity, 1 stop bit */
    uart_line_8e2 = 0x1f,   /* 8 data bits, even parity, 2 stop bits */
    uart_line_8o1 = 0x0b,   /* 8 data bits, odd parity, 1 stop bit */
    uart_line_8o2 = 0x0f    /* 8 data bits, odd parity, 2 stop bits */
} dw_uart_line_control;

__METAL_DECLARE_VTABLE(__metal_driver_vtable_sifive_nb2uart0)

struct __metal_driver_sifive_nb2uart0 {
	struct metal_uart uart;
	unsigned long baud_rate;
	uint32_t divisor;             /* Clock divisor value */
	dw_uart_line_control setting; /* Line control configuration */
	metal_clock_callback pre_rate_change_callback;
	metal_clock_callback post_rate_change_callback;
};

#endif
