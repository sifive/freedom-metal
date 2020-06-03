/* Copyright 2019 SiFive, Inc */
/* SPDX-License-Identifier: Apache-2.0 */

#ifndef METAL__I2C_H
#define METAL__I2C_H

#include <assert.h>
#include <metal/generated/i2c.h>
#include <stdint.h>

/*! @brief Enums to enable/disable stop condition. */
typedef enum {
    METAL_I2C_STOP_DISABLE = 0,
    METAL_I2C_STOP_ENABLE = 1
} metal_i2c_stop_bit_t;

/*! @brief Enums to set up I2C device modes. */
typedef enum { METAL_I2C_SLAVE = 0, METAL_I2C_MASTER = 1 } metal_i2c_mode_t;

/*! @brief A handle for a I2C device. */
struct metal_i2c {
    uint32_t __i2c_index;
};

/*! @brief Get a handle for a I2C device.
 * @param index The index of the desired I2C device.
 * @return A handle to the I2C device, or NULL if the device does not exist.*/
inline const struct metal_i2c metal_i2c_get_device(uint32_t index) {
	assert(index < __METAL_DT_NUM_I2CS);
	return (const struct metal_i2c) { index };
}

/*! @brief Initialize a I2C device with a certain baud rate.
 * @param i2c The handle for the I2C device to initialize.
 * @param baud_rate The baud rate for the I2C device to operate at.
 * @param mode I2C operation mode.
 */
void metal_i2c_init(struct metal_i2c i2c, unsigned int baud_rate,
                    metal_i2c_mode_t mode);

/*! @brief Perform a I2C write.
 * @param i2c The handle for the I2C device to perform the write operation.
 * @param addr The I2C slave address for the write operation.
 * @param len The number of bytes to transfer.
 * @param buf The buffer to send over the I2C bus. Must be len bytes long.
 * @param stop_bit Enable / Disable STOP condition.
 * @return 0 if the write succeeds.
 */
int metal_i2c_write(struct metal_i2c i2c, unsigned int addr, unsigned int len,
                    unsigned char buf[], metal_i2c_stop_bit_t stop_bit);

/*! @brief Perform a I2C read.
 * @param i2c The handle for the I2C device to perform the read operation.
 * @param addr The I2C slave address for the read operation.
 * @param len The number of bytes to transfer.
 * @param buf The buffer to store data from I2C bus. Must be len bytes long.
 * @param stop_bit Enable / Disable STOP condition.
 * @return 0 if the read succeeds.
 */
int metal_i2c_read(struct metal_i2c i2c, unsigned int addr, unsigned int len,
                   unsigned char buf[], metal_i2c_stop_bit_t stop_bit);

/*! @brief Performs back to back I2C write and read operations.
 * @param i2c The handle for the I2C device to perform the transfer operation.
 * @param addr The I2C slave address for the transfer operation.
 * @param txbuf The data buffer to be transmitted over I2C bus.
 * @param txlen The number of bytes to write over I2C.
 * @param rxbuf The buffer to store data received over I2C bus.
 * @param rxlen The number of bytes to read over I2C.
 * @return 0 if the transfer succeeds.
 */
int metal_i2c_transfer(struct metal_i2c i2c, unsigned int addr,
                       unsigned char txbuf[], unsigned int txlen,
                       unsigned char rxbuf[], unsigned int rxlen);

/*! @brief Get the current baud rate of the I2C device.
 * @param i2c The handle for the I2C device.
 * @return The baud rate in Hz.
 */
int metal_i2c_get_baud_rate(struct metal_i2c i2c);

/*! @brief Set the current baud rate of the I2C device.
 * @param i2c The handle for the I2C device.
 * @param baud_rate The desired baud rate of the I2C device.
 * @return 0 If the baud rate is successfully changed.
 */
int metal_i2c_set_baud_rate(struct metal_i2c i2c, unsigned int baud_rate);

#endif
