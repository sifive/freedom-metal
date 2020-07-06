/* Copyright 2019 SiFive, Inc */
/* SPDX-License-Identifier: Apache-2.0 */

#ifndef METAL__GPIO_H
#define METAL__GPIO_H

#include <assert.h>
#include <metal/interrupt.h>
#include <metal/generated/gpio.h>
#include <stdint.h>

/*!
 * @file gpio.h
 * @brief API for manipulating general-purpose input/output
 */

enum metal_gpio_int_type {
	METAL_GPIO_INT_DISABLE = 0,
	METAL_GPIO_INT_RISING = 1,
	METAL_GPIO_INT_FALLING = 2,
	METAL_GPIO_INT_BOTH_EDGE = 3,
	METAL_GPIO_INT_LOW = 4,
	METAL_GPIO_INT_HIGH = 5,
	METAL_GPIO_INT_BOTH_LEVEL = 6,
	METAL_GPIO_INT_MAX = 7,
};

/*!
 * @struct metal_gpio
 * @brief The handle for a GPIO interface
 */
struct metal_gpio {
    uint32_t __gpio_index;
};

#define METAL_GPIO_INVALID_INDEX UINT32_MAX

/*! @brief Get a handle for a GPIO device
 * @param device_num The index of the desired GPIO device
 * @return A handle to the GPIO device */
inline const struct metal_gpio metal_gpio_get_device(uint32_t index) {
	assert(index < __METAL_DT_NUM_GPIOS);
    return (const struct metal_gpio) { index };
}

/*!
 * @brief enable input on a pin
 * @param gpio The handle for the GPIO interface
 * @param pin The pin number indexed from 0
 * @return 0 if the input is successfully enabled
 */
int metal_gpio_enable_input(struct metal_gpio gpio, int pin);

/*!
 * @brief Disable input on a pin
 * @param gpio The handle for the GPIO interface
 * @param pin The pin number indexed from 0
 * @return 0 if the input is successfully disabled
 */
int metal_gpio_disable_input(struct metal_gpio gpio, int pin);

/*!
 * @brief Enable output on a pin
 * @param gpio The handle for the GPIO interface
 * @param pin The pin number indexed from 0
 * @return 0 if the output is successfully enabled
 */
int metal_gpio_enable_output(struct metal_gpio gpio, int pin);

/*!
 * @brief Disable output on a pin
 * @param gpio The handle for the GPIO interface
 * @param pin The pin number indexed from 0
 * @return 0 if the output is successfully disabled
 */
int metal_gpio_disable_output(struct metal_gpio gpio, int pin);

/*!
 * @brief Set the output value of a GPIO pin
 * @param gpio The handle for the GPIO interface
 * @param pin The pin number indexed from 0
 * @param value The value to set the pin to
 * @return 0 if the output is successfully set
 */
int metal_gpio_set_pin(struct metal_gpio gpio, int pin, int value);

/*!
 * @brief Get the value of the GPIO pin
 * @param gpio The handle for the GPIO interface
 * @param pin The pin number indexed from 0
 * @return The value of the GPIO pin
 */
int metal_gpio_get_input_pin(struct metal_gpio gpio, int pin);

/*!
 * @brief Get the value of the GPIO pin
 * @param gpio The handle for the GPIO interface
 * @param pin The pin number indexed from 0
 * @return The value of the GPIO pin
 */
int metal_gpio_get_output_pin(struct metal_gpio gpio, int pin);

/*!
 * @brief Clears the value of the GPIO pin
 * @param gpio The handle for the GPIO interface
 * @param pin The pin number indexed from 0
 * @return 0 if the pin is successfully cleared
 */
int metal_gpio_clear_pin(struct metal_gpio gpio, int pin);

/*!
 * @brief Toggles the value of the GPIO pin
 * @param gpio The handle for the GPIO interface
 * @param pin The pin number indexed from 0
 * @return 0 if the pin is successfully toggled
 */
int metal_gpio_toggle_pin(struct metal_gpio gpio, int pin);

/*!
 * @brief Enables and sets the pinmux for a GPIO pin
 * @param gpio The handle for the GPIO interface
 * @param pin The bitmask for the pin to enable pinmux on
 * @param io_function The IO function to set
 * @return 0 if the pinmux is successfully set
 */
int metal_gpio_enable_pinmux(struct metal_gpio gpio, long pin_mask,
                             long io_function_mask);

/*!
 * @brief Disables the pinmux for a GPIO pin
 * @param gpio The handle for the GPIO interface
 * @param pin The bitmask for the pin to disable pinmux on
 * @return 0 if the pinmux is successfully set
 */
int metal_gpio_disable_pinmux(struct metal_gpio gpio, long pin_mask);

/*!
 * @brief Config gpio interrupt type
 * @param gpio The handle for the GPIO interface
 * @param pin The bitmask for the pin to enable gpio interrupt
 * @param intr_type The interrupt type
 * @return 0 if the interrupt mode is setup properly
 */
int metal_gpio_config_interrupt(struct metal_gpio gpio, int pin,
                                enum metal_gpio_int_type int_type);

/*!
 * @brief Clear gpio interrupt status
 * @param gpio The handle for the GPIO interface
 * @param pin The bitmask for the pin to clear gpio interrupt
 * @param intr_type The interrupt type to be clear
 * @return 0 if the interrupt is cleared
 */
int metal_gpio_clear_interrupt(struct metal_gpio gpio, int pin, enum metal_gpio_int_type int_type);

#endif
