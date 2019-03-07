/* Copyright 2018 SiFive, Inc */
/* SPDX-License-Identifier: Apache-2.0 */

#include <metal/drivers/sifive,gpio0.h>
#include <metal/io.h>

#define GPIO_INPUT_VAL  (0x00)
#define GPIO_INPUT_EN   (0x04)
#define GPIO_OUTPUT_EN  (0x08)
#define GPIO_OUTPUT_VAL (0x0C)

#define GPIO_IOF_EN     (0x38)
#define GPIO_IOF_SEL    (0x3C)

int __metal_driver_sifive_gpio0_disable_input(struct metal_gpio *ggpio, long source)
{
    struct __metal_driver_sifive_gpio0 *gpio = (void *)ggpio;

    __METAL_ACCESS_ONCE((__metal_io_u32 *)(gpio->base + GPIO_INPUT_EN))  &= ~source;

    return 0;
}

long __metal_driver_sifive_gpio0_output(struct metal_gpio *ggpio)
{
    struct __metal_driver_sifive_gpio0 *gpio = (void *)ggpio;

    return __METAL_ACCESS_ONCE((__metal_io_u32 *)(gpio->base + GPIO_OUTPUT_VAL));
}

int __metal_driver_sifive_gpio0_enable_output(struct metal_gpio *ggpio, long source)
{
    struct __metal_driver_sifive_gpio0 *gpio = (void *)ggpio;

    __METAL_ACCESS_ONCE((__metal_io_u32 *)(gpio->base + GPIO_OUTPUT_EN))  |= source;

    return 0;
}

int __metal_driver_sifive_gpio0_output_set(struct metal_gpio *ggpio, long value)
{
    struct __metal_driver_sifive_gpio0 *gpio = (void *)ggpio;

    __METAL_ACCESS_ONCE((__metal_io_u32 *)(gpio->base + GPIO_OUTPUT_VAL)) |= value;

    return 0;
}

int __metal_driver_sifive_gpio0_output_clear(struct metal_gpio *ggpio, long value)
{
    struct __metal_driver_sifive_gpio0 *gpio = (void *)ggpio;

    __METAL_ACCESS_ONCE((__metal_io_u32 *)(gpio->base + GPIO_OUTPUT_VAL)) &= ~value;

    return 0;
}

int __metal_driver_sifive_gpio0_output_toggle(struct metal_gpio *ggpio, long value)
{
    struct __metal_driver_sifive_gpio0 *gpio = (void *)ggpio;

    __METAL_ACCESS_ONCE((__metal_io_u32 *)(gpio->base + GPIO_OUTPUT_VAL)) =
	__METAL_ACCESS_ONCE((__metal_io_u32 *)(gpio->base + GPIO_OUTPUT_VAL)) ^ value;

    return 0;
}

int __metal_driver_sifive_gpio0_enable_io(struct metal_gpio *ggpio, long source, long dest)
{
    struct __metal_driver_sifive_gpio0 *gpio = (void *)ggpio;

    __METAL_ACCESS_ONCE((__metal_io_u32 *)(gpio->base + GPIO_IOF_SEL)) &= ~source;
    __METAL_ACCESS_ONCE((__metal_io_u32 *)(gpio->base + GPIO_IOF_EN))  |= dest;

    return 0;
}
