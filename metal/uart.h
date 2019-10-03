/* Copyright 2018 SiFive, Inc */
/* SPDX-License-Identifier: Apache-2.0 */

#ifndef METAL__UART_H
#define METAL__UART_H

/*!
 * @file uart.h
 * @brief API for UART serial ports
 */

#include <metal/interrupt.h>

struct metal_uart;
#undef getc
#undef putc
struct metal_uart_vtable {
    void (*init)(struct metal_uart *uart, int baud_rate);
    int (*putc)(struct metal_uart *uart, int c);
    int (*txready)(struct metal_uart *uart);
    int (*rxready)(struct metal_uart *uart);
    int (*getc)(struct metal_uart *uart, int *c);
    int (*get_baud_rate)(struct metal_uart *uart);
    int (*set_baud_rate)(struct metal_uart *uart, int baud_rate);
    struct metal_interrupt *(*controller_interrupt)(struct metal_uart *uart);
    int (*get_interrupt_id)(struct metal_uart *uart);
    int (*tx_interrupt_enable)(struct metal_uart *uart);
    int (*tx_interrupt_disable)(struct metal_uart *uart);
    int (*rx_interrupt_enable)(struct metal_uart *uart);
    int (*rx_interrupt_disable)(struct metal_uart *uart);
    int (*set_tx_watermark)(struct metal_uart *uart, size_t length);
    size_t (*get_tx_watermark)(struct metal_uart *uart);
    int (*set_rx_watermark)(struct metal_uart *uart, size_t length);
    size_t (*get_rx_watermark)(struct metal_uart *uart);
};

/*!
 * @brief Enum type for describing the cause of a UART callaback
 */
typedef enum {
    METAL_UART_TX_DONE,
    METAL_UART_RX_DONE,
} metal_uart_callback_cause;

/*!
 * @brief Function signature for UART callbacks
 */
typedef void (*metal_uart_callback_t)(struct metal_uart *uart,
                                      metal_uart_callback_cause);

struct _metal_uart_async_buf {
    int in_progress;
    char *buf;
    size_t len;
    size_t index;
    metal_uart_callback_t cb;
};

/*!
 * @brief Handle for a UART serial device
 */
struct metal_uart {
    const struct metal_uart_vtable *vtable;
    struct _metal_uart_async_buf _tx_async;
    struct _metal_uart_async_buf _rx_async;
};

/*! @brief Get a handle for a UART device
 * @param device_num The index of the desired UART device
 * @return A handle to the UART device, or NULL if the device does not exist*/
struct metal_uart *metal_uart_get_device(unsigned int device_num);

/*!
 * @brief Initialize UART device

 * Initialize the UART device described by the UART handle. This function must
 be called before any
 * other method on the UART can be invoked. It is invalid to initialize a UART
 more than once.
 *
 * @param uart The UART device handle
 * @param baud_rate the baud rate to set the UART to
 */
__inline__ void metal_uart_init(struct metal_uart *uart, int baud_rate) {
    uart->vtable->init(uart, baud_rate);
}

/*!
 * @brief Output a character over the UART
 * @param uart The UART device handle
 * @param c The character to send over the UART
 * @return 0 upon success
 */
__inline__ int metal_uart_putc(struct metal_uart *uart, int c) {
    return uart->vtable->putc(uart, c);
}

/*!
 * @brief Test, determine if tx output is blocked(full/busy)
 * @param uart The UART device handle
 * @return 0 not blocked
 */
__inline__ int metal_uart_txready(struct metal_uart *uart) {
    return uart->vtable->txready(uart);
}

/*!
 * @brief Test, determine if the uart has characters to be read
 * @param uart The UART device handle
 * @return 0 if the receive FIFO has characters
 */
__inline__ int metal_uart_rxready(struct metal_uart *uart) {
    return uart->vtable->rxready(uart);
}

/*!
 * @brief Read a character sent over the UART
 * @param uart The UART device handle
 * @param c The varible to hold the read character
 * @return 0 upon success
 *
 * If "c == -1" no char was ready.
 * If "c != -1" then C == byte value (0x00 to 0xff)
 */
__inline__ int metal_uart_getc(struct metal_uart *uart, int *c) {
    return uart->vtable->getc(uart, c);
}

/*!
 * @brief Get the baud rate of the UART peripheral
 * @param uart The UART device handle
 * @return The current baud rate of the UART
 */
__inline__ int metal_uart_get_baud_rate(struct metal_uart *uart) {
    return uart->vtable->get_baud_rate(uart);
}

/*!
 * @brief Set the baud rate of the UART peripheral
 * @param uart The UART device handle
 * @param baud_rate The baud rate to configure
 * @return the new baud rate of the UART
 */
__inline__ int metal_uart_set_baud_rate(struct metal_uart *uart,
                                        int baud_rate) {
    return uart->vtable->set_baud_rate(uart, baud_rate);
}

/*!
 * @brief Get the interrupt controller of the UART peripheral
 *
 * Get the interrupt controller for the UART peripheral. The interrupt
 * controller must be initialized before any interrupts can be registered
 * or enabled with it.
 *
 * @param uart The UART device handle
 * @return The handle for the UART interrupt controller
 */
__inline__ struct metal_interrupt *
metal_uart_interrupt_controller(struct metal_uart *uart) {
    return uart->vtable->controller_interrupt(uart);
}

/*!
 * @brief Get the interrupt ID of the UART controller
 * @param uart The UART device handle
 * @return The UART interrupt id
 */
__inline__ int metal_uart_get_interrupt_id(struct metal_uart *uart) {
    return uart->vtable->get_interrupt_id(uart);
}

/*!
 * @brief Enable the UART transmit interrupt
 * @param uart The UART device handle
 * @return 0 upon success
 */
__inline__ int metal_uart_transmit_interrupt_enable(struct metal_uart *uart) {
    return uart->vtable->tx_interrupt_enable(uart);
}

/*!
 * @brief Disable the UART transmit interrupt
 * @param uart The UART device handle
 * @return 0 upon success
 */
__inline__ int metal_uart_transmit_interrupt_disable(struct metal_uart *uart) {
    return uart->vtable->tx_interrupt_disable(uart);
}

/*!
 * @brief Enable the UART receive interrupt
 * @param uart The UART device handle
 * @return 0 upon success
 */
__inline__ int metal_uart_receive_interrupt_enable(struct metal_uart *uart) {
    return uart->vtable->rx_interrupt_enable(uart);
}

/*!
 * @brief Disable the UART receive interrupt
 * @param uart The UART device handle
 * @return 0 upon success
 */
__inline__ int metal_uart_receive_interrupt_disable(struct metal_uart *uart) {
    return uart->vtable->rx_interrupt_disable(uart);
}

/*!
 * @brief Set the transmit watermark level of the UART controller
 * @param uart The UART device handle
 * @param level The UART transmit watermark level
 * @return 0 upon success
 */
__inline__ int metal_uart_set_transmit_watermark(struct metal_uart *uart,
                                                 size_t level) {
    return uart->vtable->set_tx_watermark(uart, level);
}

/*!
 * @brief Get the transmit watermark level of the UART controller
 * @param uart The UART device handle
 * @return The UART transmit watermark level
 */
__inline__ size_t metal_uart_get_transmit_watermark(struct metal_uart *uart) {
    return uart->vtable->get_tx_watermark(uart);
}

/*!
 * @brief Set the receive watermark level of the UART controller
 * @param uart The UART device handle
 * @param level The UART transmit watermark level
 * @return 0 upon success
 */
__inline__ int metal_uart_set_receive_watermark(struct metal_uart *uart,
                                                size_t level) {
    return uart->vtable->set_rx_watermark(uart, level);
}

/*!
 * @brief Get the receive watermark level of the UART controller
 * @param uart The UART device handle
 * @return The UART transmit watermark level
 */
__inline__ size_t metal_uart_get_receive_watermark(struct metal_uart *uart) {
    return uart->vtable->get_rx_watermark(uart);
}

/*!
 * @brief Send buffer asynchronously over UART
 *
 * Executes the callback when the send completes or if an error occurs
 * during transmission.
 *
 * The asynchronous behavior places some expectations on the interrupt
 * configuration of the platform:
 *  - metal_interrupt_enable() has been called on the metal_interrupt struct
 *    corresponding to the current CPU and the UART device being used
 *  - metal_uart_transmit_interrupt_enable/disable will be called by the UART
 *    driver
 *  - metal_uart_set_transmit_watermark controlled by the user
 *
 * @return -1 if the UART is already busy sending a buffer, 0 if the send begins
 */
int metal_uart_send_async(struct metal_uart *uart, char *buf, size_t len,
                          metal_uart_callback_t callback);

/*!
 * @brief Check if an asynchronous UART transmission is currently in-progress
 * @return 1 if a send is in progress, otherwise 0
 */
int metal_uart_send_async_busy(struct metal_uart *uart);

/*!
 * @brief Receive buffer asynchronously over UART
 *
 * Executes the callback when the buffer has been filled or if
 * an error occurs during receipt.
 *
 * The asynchronous behavior places some expectations on the interrupt
 * configuration of the platform:
 *  - metal_interrupt_enable() has been called on the metal_interrupt struct
 *    corresponding to the current CPU and the UART device being used
 *  - metal_uart_receive_interrupt_enable/disable will be called by the UART
 *    driver
 *  - metal_uart_set_receive_watermark controlled by the user
 *
 * @return -1 if the UART is already busy receiving a buffer, 0 if the receipt
 * begins
 */
int metal_uart_recv_async(struct metal_uart *uart, char *buf, size_t len,
                          metal_uart_callback_t callback);

/*!
 * @brief Check if an asynchronous UART receipt is currently in-progress
 * @return 1 if a receipt is in progress, otherwise 0
 */
int metal_uart_recv_async_busy(struct metal_uart *uart);

#endif
