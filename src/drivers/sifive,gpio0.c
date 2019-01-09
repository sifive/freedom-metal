/* Copyright 2018 SiFive, Inc */
/* SPDX-License-Identifier: Apache-2.0 */

#include <mee/drivers/sifive,gpio0.h>
#include <mee/io.h>

#define GPIO_INPUT_VAL  (0x00)
#define GPIO_INPUT_EN   (0x04)
#define GPIO_OUTPUT_EN  (0x08)
#define GPIO_OUTPUT_VAL (0x0C)

#define GPIO_IOF_EN     (0x38)
#define GPIO_IOF_SEL    (0x3C)

long __mee_driver_sifive_gpio0_disable_input(const struct __mee_driver_sifive_gpio0 *gpio, long source)
{
    __MEE_ACCESS_ONCE((__mee_io_u32 *)(gpio->base + GPIO_INPUT_EN))  &= ~source;
}

long __mee_driver_sifive_gpio0_output(const struct __mee_driver_sifive_gpio0 *gpio)
{
    return __MEE_ACCESS_ONCE((__mee_io_u32 *)(gpio->base + GPIO_OUTPUT_VAL));
}

long __mee_driver_sifive_gpio0_enable_output(const struct __mee_driver_sifive_gpio0 *gpio, long source)
{
    __MEE_ACCESS_ONCE((__mee_io_u32 *)(gpio->base + GPIO_OUTPUT_EN))  |= source;
}

long __mee_driver_sifive_gpio0_output_set(const struct __mee_driver_sifive_gpio0 *gpio, long value)
{
    __MEE_ACCESS_ONCE((__mee_io_u32 *)(gpio->base + GPIO_OUTPUT_VAL)) |= value;
}

long __mee_driver_sifive_gpio0_output_clear(const struct __mee_driver_sifive_gpio0 *gpio, long value)
{
    __MEE_ACCESS_ONCE((__mee_io_u32 *)(gpio->base + GPIO_OUTPUT_VAL)) &= ~value;
}

long __mee_driver_sifive_gpio0_output_toggle(const struct __mee_driver_sifive_gpio0 *gpio, long value)
{
    __MEE_ACCESS_ONCE((__mee_io_u32 *)(gpio->base + GPIO_OUTPUT_VAL)) =
	__MEE_ACCESS_ONCE((__mee_io_u32 *)(gpio->base + GPIO_OUTPUT_VAL)) ^ value;
}

long __mee_driver_sifive_gpio0_enable_io(const struct __mee_driver_sifive_gpio0 *gpio, long source, long dest)
{
    __MEE_ACCESS_ONCE((__mee_io_u32 *)(gpio->base + GPIO_IOF_SEL)) &= ~source;
    __MEE_ACCESS_ONCE((__mee_io_u32 *)(gpio->base + GPIO_IOF_EN))  |= dest;
}
