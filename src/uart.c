/* Copyright 2018 SiFive, Inc */
/* SPDX-License-Identifier: Apache-2.0 */

#include <metal/machine.h>
#include <metal/uart.h>

extern __inline__ void metal_uart_init(struct metal_uart *uart, int baud_rate);
extern __inline__ int metal_uart_putc(struct metal_uart *uart, int c);
extern __inline__ int metal_uart_txready(struct metal_uart *uart);
extern __inline__ int metal_uart_rxready(struct metal_uart *uart);
extern __inline__ int metal_uart_getc(struct metal_uart *uart, int *c);
extern __inline__ int metal_uart_get_baud_rate(struct metal_uart *uart);
extern __inline__ int metal_uart_set_baud_rate(struct metal_uart *uart,
                                               int baud_rate);
extern __inline__ struct metal_interrupt *
metal_uart_interrupt_controller(struct metal_uart *uart);
extern __inline__ int metal_uart_get_interrupt_id(struct metal_uart *uart);
extern __inline__ int
metal_uart_transmit_interrupt_enable(struct metal_uart *uart);
extern __inline__ int
metal_uart_transmit_interrupt_disable(struct metal_uart *uart);
extern __inline__ int
metal_uart_receive_interrupt_enable(struct metal_uart *uart);
extern __inline__ int
metal_uart_receive_interrupt_disable(struct metal_uart *uart);
extern __inline__ int metal_uart_set_transmit_watermark(struct metal_uart *uart,
                                                        size_t level);
extern __inline__ size_t
metal_uart_get_transmit_watermark(struct metal_uart *uart);
extern __inline__ int metal_uart_set_receive_watermark(struct metal_uart *uart,
                                                       size_t level);
extern __inline__ size_t
metal_uart_get_receive_watermark(struct metal_uart *uart);

struct metal_uart *metal_uart_get_device(unsigned int device_num) {
#if __METAL_DT_MAX_UARTS > 0
    if (device_num < __METAL_DT_MAX_UARTS) {
        return (struct metal_uart *)__metal_uart_table[device_num];
    }
#endif

    return NULL;
}

void _metal_uart_async_callback(int id, void *priv) {
    struct metal_uart *uart = (struct metal_uart *)priv;

    if (uart->_tx_async.in_progress && metal_uart_txready(uart)) {
        while (metal_uart_txready(uart)) {
            metal_uart_putc(uart, uart->_tx_async.buf[uart->_tx_async.index]);

            uart->_tx_async.index += 1;
            if (uart->_tx_async.index >= uart->_tx_async.len) {
                uart->_tx_async.in_progress = 0;
                uart->_tx_async.cb(uart, METAL_UART_TX_DONE);
                metal_uart_transmit_interrupt_disable(uart);
                break;
            }
        }
    }

    if (uart->_rx_async.in_progress) {
        while (1) {
            int c;

            metal_uart_getc(uart, &c);
            if (c == -1) {
                /* Nothing to read */
                break;
            }

            uart->_rx_async.buf[uart->_rx_async.index] = (char)c;

            uart->_rx_async.index += 1;
            if (uart->_rx_async.index >= uart->_rx_async.len) {
                uart->_rx_async.in_progress = 0;
                uart->_rx_async.cb(uart, METAL_UART_RX_DONE);
                metal_uart_receive_interrupt_disable(uart);
                break;
            }
        }
    }
}

int metal_uart_send_async(struct metal_uart *uart, char *buf, size_t len,
                          metal_uart_callback_t callback) {
    if (metal_uart_send_async_busy(uart)) {
        return 1;
    }

    uart->_tx_async.in_progress = 1;
    uart->_tx_async.buf = buf;
    uart->_tx_async.len = len;
    uart->_tx_async.index = 0;
    uart->_tx_async.cb = callback;

    /* Register _metal_uart_async_callback as the interrupt handler for the
     * uart interrupt */
    struct metal_interrupt *uart_intr = metal_uart_interrupt_controller(uart);
    int uart_id = metal_uart_get_interrupt_id(uart);
    int rc = metal_interrupt_register_handler(
        uart_intr, uart_id, _metal_uart_async_callback, (void *)uart);
    if (rc != 0) {
        return 2;
    }

    metal_uart_transmit_interrupt_enable(uart);

    return 0;
}

int metal_uart_send_async_busy(struct metal_uart *uart) {
    return uart->_tx_async.in_progress;
}

int metal_uart_recv_async(struct metal_uart *uart, char *buf, size_t len,
                          metal_uart_callback_t callback) {

    if (metal_uart_recv_async_busy(uart)) {
        return 1;
    }

    uart->_rx_async.in_progress = 1;
    uart->_rx_async.buf = buf;
    uart->_rx_async.len = len;
    uart->_rx_async.index = 0;
    uart->_rx_async.cb = callback;

    /* Register _metal_uart_async_callback as the interrupt handler for the
     * uart interrupt */
    struct metal_interrupt *uart_intr = metal_uart_interrupt_controller(uart);
    int uart_id = metal_uart_get_interrupt_id(uart);
    int rc = metal_interrupt_register_handler(
        uart_intr, uart_id, _metal_uart_async_callback, (void *)uart);
    if (rc != 0) {
        return 2;
    }

    metal_uart_receive_interrupt_enable(uart);

    return 0;
}

int metal_uart_recv_async_busy(struct metal_uart *uart) {
    return uart->_rx_async.in_progress;
}
