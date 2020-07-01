/* Copyright 2018 SiFive, Inc */
/* SPDX-License-Identifier: Apache-2.0 */

#ifndef METAL__UART_H
#define METAL__UART_H

/*!
 * @file uart.h
 * @brief API for UART serial ports
 */

#include <assert.h>
#include <metal/generated/uart.h>
#include <metal/interrupt.h>
#include <stdint.h>

/*!
 * @brief Handle for a UART serial device
 */
struct metal_uart {
    uint32_t __uart_index;
};

#define METAL_UART_INVALID_INDEX UINT32_MAX

/*! @brief Get a handle for a UART device
 * @param device_num The index of the desired UART device
 * @return A handle to the UART device, or NULL if the device does not exist*/
inline const struct metal_uart metal_uart_get_device(uint32_t index) {
    assert(index < __METAL_DT_NUM_UARTS);
    return (const struct metal_uart) { index };
}

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
void metal_uart_init(struct metal_uart uart, uint32_t baud_rate);

/*!
 * @brief Output a character over the UART
 * @param uart The UART device handle
 * @param c The character to send over the UART
 * @return 0 upon success
 */
int metal_uart_putc(struct metal_uart uart, int c);

/*!
 * @brief Test, determine if tx output is blocked(full/busy)
 * @param uart The UART device handle
 * @return 0 not blocked
 */
int metal_uart_txready(struct metal_uart uart);

/*!
 * @brief Read a character sent over the UART
 * @param uart The UART device handle
 * @param c The varible to hold the read character
 * @return 0 upon success
 *
 * If "c == -1" no char was ready.
 * If "c != -1" then C == byte value (0x00 to 0xff)
 */
int metal_uart_getc(struct metal_uart uart, int *c);

/*!
 * @brief Get the baud rate of the UART peripheral
 * @param uart The UART device handle
 * @return The current baud rate of the UART
 */
int metal_uart_get_baud_rate(struct metal_uart uart);

/*!
 * @brief Set the baud rate of the UART peripheral
 * @param uart The UART device handle
 * @param baud_rate The baud rate to configure
 * @return the new baud rate of the UART
 */
int metal_uart_set_baud_rate(struct metal_uart uart, int baud_rate);

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
struct metal_interrupt
metal_uart_interrupt_controller(struct metal_uart uart);

/*!
 * @brief Get the interrupt ID of the UART controller
 * @param uart The UART device handle
 * @return The UART interrupt id
 */
int metal_uart_get_interrupt_id(struct metal_uart uart);

/*!
 * @brief Enable the UART transmit interrupt
 * @param uart The UART device handle
 * @return 0 upon success
 */
int metal_uart_transmit_interrupt_enable(struct metal_uart uart);

/*!
 * @brief Disable the UART transmit interrupt
 * @param uart The UART device handle
 * @return 0 upon success
 */
int metal_uart_transmit_interrupt_disable(struct metal_uart uart);

/*!
 * @brief Enable the UART receive interrupt
 * @param uart The UART device handle
 * @return 0 upon success
 */
int metal_uart_receive_interrupt_enable(struct metal_uart uart);

/*!
 * @brief Disable the UART receive interrupt
 * @param uart The UART device handle
 * @return 0 upon success
 */
int metal_uart_receive_interrupt_disable(struct metal_uart uart);

/*!
 * @brief Set the transmit watermark level of the UART controller
 * @param uart The UART device handle
 * @param level The UART transmit watermark level
 * @return 0 upon success
 */
int metal_uart_set_transmit_watermark(struct metal_uart uart, size_t level);

/*!
 * @brief Get the transmit watermark level of the UART controller
 * @param uart The UART device handle
 * @return The UART transmit watermark level
 */
size_t metal_uart_get_transmit_watermark(struct metal_uart uart);

/*!
 * @brief Set the receive watermark level of the UART controller
 * @param uart The UART device handle
 * @param level The UART transmit watermark level
 * @return 0 upon success
 */
int metal_uart_set_receive_watermark(struct metal_uart uart, size_t level);

/*!
 * @brief Get the receive watermark level of the UART controller
 * @param uart The UART device handle
 * @return The UART transmit watermark level
 */
size_t metal_uart_get_receive_watermark(struct metal_uart uart);

/*!
 * @brief Callback to allow the UART to prepare for a change of clock rate
 * @param uart The UART device handle
 *
 * For internal use only.
 */
void _metal_uart_pre_rate_change_callback(struct metal_uart uart);

/*!
 * @brief Callback to allow the UART to adapt to a new clock rate
 * @param uart The UART device handle
 *
 * For internal use only.
 */
void _metal_uart_post_rate_change_callback(struct metal_uart uart);

#endif
