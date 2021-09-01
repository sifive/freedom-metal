/* Copyright 2021 SiFive, Inc */
/* SPDX-License-Identifier: Apache-2.0 */

#include <metal/machine/platform.h>

#ifdef METAL_SIFIVE_GPIO2

#include <metal/drivers/sifive_gpio2.h>
#include <metal/io.h>
#include <metal/machine.h>

int __metal_driver_sifive_gpio2_enable_input(struct metal_gpio *ggpio,
                                             long source) {
    long base = __metal_driver_sifive_gpio2_base(ggpio);

    __METAL_ACCESS_ONCE(
        (__metal_io_u32 *)(base + METAL_SIFIVE_GPIO2_INPUT_EN)) |= source;

    return 0;
}

int __metal_driver_sifive_gpio2_disable_input(struct metal_gpio *ggpio,
                                              long source) {
    long base = __metal_driver_sifive_gpio2_base(ggpio);

    __METAL_ACCESS_ONCE(
        (__metal_io_u32 *)(base + METAL_SIFIVE_GPIO2_INPUT_EN)) &= ~source;

    return 0;
}

long __metal_driver_sifive_gpio2_input(struct metal_gpio *ggpio) {
    long base = __metal_driver_sifive_gpio2_base(ggpio);

    return __METAL_ACCESS_ONCE(
        (__metal_io_u32 *)(base + METAL_SIFIVE_GPIO2_VALUE));
}

long __metal_driver_sifive_gpio2_output(struct metal_gpio *ggpio) {
    long base = __metal_driver_sifive_gpio2_base(ggpio);

#if 0
    return __METAL_ACCESS_ONCE(
        (__metal_io_u32 *)(base + METAL_SIFIVE_GPIO2_PORT));
#endif
    return 0;
}

int __metal_driver_sifive_gpio2_disable_output(struct metal_gpio *ggpio,
                                               long source) {
    long base = __metal_driver_sifive_gpio2_base(ggpio);

    __METAL_ACCESS_ONCE(
        (__metal_io_u32 *)(base + METAL_SIFIVE_GPIO2_OUTPUT_EN)) &= ~source;

    return 0;
}

int __metal_driver_sifive_gpio2_enable_output(struct metal_gpio *ggpio,
                                              long source) {
    long base = __metal_driver_sifive_gpio2_base(ggpio);

    __METAL_ACCESS_ONCE(
        (__metal_io_u32 *)(base + METAL_SIFIVE_GPIO2_OUTPUT_EN)) |= source;

    return 0;
}

int __metal_driver_sifive_gpio2_output_set(struct metal_gpio *ggpio,
                                           long value) {
    long base = __metal_driver_sifive_gpio2_base(ggpio);

    __METAL_ACCESS_ONCE(
        (__metal_io_u32 *)(base + METAL_SIFIVE_GPIO2_OUTPUT_VALUE)) |= value;

    return 0;
}

int __metal_driver_sifive_gpio2_output_clear(struct metal_gpio *ggpio,
                                             long value) {
    long base = __metal_driver_sifive_gpio2_base(ggpio);

    __METAL_ACCESS_ONCE(
        (__metal_io_u32 *)(base + METAL_SIFIVE_GPIO2_OUTPUT_VALUE)) &= ~value;

    return 0;
}

int __metal_driver_sifive_gpio2_output_toggle(struct metal_gpio *ggpio,
                                              long value) {
    long base = __metal_driver_sifive_gpio2_base(ggpio);

    __METAL_ACCESS_ONCE(
        (__metal_io_u32 *)(base + METAL_SIFIVE_GPIO2_OUTPUT_VALUE)) =
        __METAL_ACCESS_ONCE(
            (__metal_io_u32 *)(base + METAL_SIFIVE_GPIO2_OUTPUT_VALUE)) ^
        value;

    return 0;
}

int __metal_driver_sifive_gpio2_enable_io(struct metal_gpio *ggpio, long source,
                                          long dest) {
    long base = __metal_driver_sifive_gpio2_base(ggpio);

    __METAL_ACCESS_ONCE((__metal_io_u32 *)(base + METAL_SIFIVE_GPIO2_IOF_EN)) |=
        dest;

    return 0;
}

int __metal_driver_sifive_gpio2_disable_io(struct metal_gpio *ggpio,
                                           long source) {
    long base = __metal_driver_sifive_gpio2_base(ggpio);

    __METAL_ACCESS_ONCE((__metal_io_u32 *)(base + METAL_SIFIVE_GPIO2_IOF_EN)) &=
        ~source;

    return 0;
}

int __metal_driver_sifive_gpio2_config_int(struct metal_gpio *ggpio,
                                           long source, int intr_type) {
    long base = __metal_driver_sifive_gpio2_base(ggpio);

    switch (intr_type) {
    case METAL_GPIO_INT_DISABLE:
        __METAL_ACCESS_ONCE(
            (__metal_io_u32 *)(base + METAL_SIFIVE_GPIO2_RISE_IE)) &= ~source;
        __METAL_ACCESS_ONCE(
            (__metal_io_u32 *)(base + METAL_SIFIVE_GPIO2_FALL_IE)) &= ~source;
        __METAL_ACCESS_ONCE(
            (__metal_io_u32 *)(base + METAL_SIFIVE_GPIO2_HIGH_IE)) &= ~source;
        __METAL_ACCESS_ONCE(
            (__metal_io_u32 *)(base + METAL_SIFIVE_GPIO2_LOW_IE)) &= ~source;
        break;
    case METAL_GPIO_INT_RISING:
        __METAL_ACCESS_ONCE(
            (__metal_io_u32 *)(base + METAL_SIFIVE_GPIO2_RISE_IE)) |= source;
        break;
    case METAL_GPIO_INT_FALLING:
        __METAL_ACCESS_ONCE(
            (__metal_io_u32 *)(base + METAL_SIFIVE_GPIO2_FALL_IE)) |= source;
        break;
    case METAL_GPIO_INT_BOTH_EDGE:
        __METAL_ACCESS_ONCE(
            (__metal_io_u32 *)(base + METAL_SIFIVE_GPIO2_RISE_IE)) |= source;
        __METAL_ACCESS_ONCE(
            (__metal_io_u32 *)(base + METAL_SIFIVE_GPIO2_FALL_IE)) |= source;
        break;
    case METAL_GPIO_INT_HIGH:
        __METAL_ACCESS_ONCE(
            (__metal_io_u32 *)(base + METAL_SIFIVE_GPIO2_HIGH_IE)) |= source;
        break;
    case METAL_GPIO_INT_LOW:
        __METAL_ACCESS_ONCE(
            (__metal_io_u32 *)(base + METAL_SIFIVE_GPIO2_LOW_IE)) |= source;
        break;
    case METAL_GPIO_INT_BOTH_LEVEL:
        __METAL_ACCESS_ONCE(
            (__metal_io_u32 *)(base + METAL_SIFIVE_GPIO2_HIGH_IE)) |= source;
        __METAL_ACCESS_ONCE(
            (__metal_io_u32 *)(base + METAL_SIFIVE_GPIO2_LOW_IE)) |= source;
        break;
    case METAL_GPIO_INT_MAX:
        __METAL_ACCESS_ONCE(
            (__metal_io_u32 *)(base + METAL_SIFIVE_GPIO2_RISE_IE)) |= source;
        __METAL_ACCESS_ONCE(
            (__metal_io_u32 *)(base + METAL_SIFIVE_GPIO2_FALL_IE)) |= source;
        __METAL_ACCESS_ONCE(
            (__metal_io_u32 *)(base + METAL_SIFIVE_GPIO2_HIGH_IE)) |= source;
        __METAL_ACCESS_ONCE(
            (__metal_io_u32 *)(base + METAL_SIFIVE_GPIO2_LOW_IE)) |= source;
        break;
    }
    return 0;
}

int __metal_driver_sifive_gpio2_clear_int(struct metal_gpio *ggpio, long source,
                                          int intr_type) {
    long base = __metal_driver_sifive_gpio2_base(ggpio);

    switch (intr_type) {
    case METAL_GPIO_INT_RISING:
        __METAL_ACCESS_ONCE(
            (__metal_io_u32 *)(base + METAL_SIFIVE_GPIO2_RISE_IP)) |= source;
        break;
    case METAL_GPIO_INT_FALLING:
        __METAL_ACCESS_ONCE(
            (__metal_io_u32 *)(base + METAL_SIFIVE_GPIO2_FALL_IP)) |= source;
        break;
    case METAL_GPIO_INT_BOTH_EDGE:
        __METAL_ACCESS_ONCE(
            (__metal_io_u32 *)(base + METAL_SIFIVE_GPIO2_RISE_IP)) |= source;
        __METAL_ACCESS_ONCE(
            (__metal_io_u32 *)(base + METAL_SIFIVE_GPIO2_FALL_IP)) |= source;
        break;
    case METAL_GPIO_INT_HIGH:
        __METAL_ACCESS_ONCE(
            (__metal_io_u32 *)(base + METAL_SIFIVE_GPIO2_HIGH_IP)) |= source;
        break;
    case METAL_GPIO_INT_LOW:
        __METAL_ACCESS_ONCE(
            (__metal_io_u32 *)(base + METAL_SIFIVE_GPIO2_LOW_IP)) |= source;
        break;
    case METAL_GPIO_INT_BOTH_LEVEL:
        __METAL_ACCESS_ONCE(
            (__metal_io_u32 *)(base + METAL_SIFIVE_GPIO2_HIGH_IP)) |= source;
        __METAL_ACCESS_ONCE(
            (__metal_io_u32 *)(base + METAL_SIFIVE_GPIO2_LOW_IP)) |= source;
        break;
    case METAL_GPIO_INT_MAX:
        __METAL_ACCESS_ONCE(
            (__metal_io_u32 *)(base + METAL_SIFIVE_GPIO2_RISE_IP)) |= source;
        __METAL_ACCESS_ONCE(
            (__metal_io_u32 *)(base + METAL_SIFIVE_GPIO2_FALL_IP)) |= source;
        __METAL_ACCESS_ONCE(
            (__metal_io_u32 *)(base + METAL_SIFIVE_GPIO2_HIGH_IP)) |= source;
        __METAL_ACCESS_ONCE(
            (__metal_io_u32 *)(base + METAL_SIFIVE_GPIO2_LOW_IP)) |= source;
        break;
    }
    return 0;
}

struct metal_interrupt *
__metal_driver_gpio_interrupt_controller(struct metal_gpio *gpio) {
    return __metal_driver_sifive_gpio2_interrupt_parent(gpio);
}

int __metal_driver_gpio_get_interrupt_id(struct metal_gpio *gpio, int pin) {
    int irq;
    irq = __metal_driver_sifive_gpio2_interrupt_lines(gpio, pin);
    return irq;
}

__METAL_DEFINE_VTABLE(__metal_driver_vtable_sifive_gpio2) = {
    .gpio.disable_input = __metal_driver_sifive_gpio2_disable_input,
    .gpio.enable_input = __metal_driver_sifive_gpio2_enable_input,
    .gpio.input = __metal_driver_sifive_gpio2_input,
    .gpio.output = __metal_driver_sifive_gpio2_output,
    .gpio.disable_output = __metal_driver_sifive_gpio2_disable_output,
    .gpio.enable_output = __metal_driver_sifive_gpio2_enable_output,
    .gpio.output_set = __metal_driver_sifive_gpio2_output_set,
    .gpio.output_clear = __metal_driver_sifive_gpio2_output_clear,
    .gpio.output_toggle = __metal_driver_sifive_gpio2_output_toggle,
    .gpio.enable_io = __metal_driver_sifive_gpio2_enable_io,
    .gpio.disable_io = __metal_driver_sifive_gpio2_disable_io,
    .gpio.config_int = __metal_driver_sifive_gpio2_config_int,
    .gpio.clear_int = __metal_driver_sifive_gpio2_clear_int,
    .gpio.interrupt_controller = __metal_driver_gpio_interrupt_controller,
    .gpio.get_interrupt_id = __metal_driver_gpio_get_interrupt_id,
};

#endif /* METAL_SIFIVE_GPIO2 */

typedef int no_empty_translation_units;
