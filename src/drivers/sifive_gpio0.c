/* Copyright 2018 SiFive, Inc */
/* SPDX-License-Identifier: Apache-2.0 */

#include <metal/machine/platform.h>

#ifdef METAL_SIFIVE_GPIO0

#include <metal/drivers/sifive_gpio0.h>
#include <metal/io.h>
#include <metal/machine.h>

int metal_gpio_enable_input(struct metal_gpio *ggpio, int pin) {
    long base = __metal_driver_sifive_gpio0_base(ggpio);

    __METAL_ACCESS_ONCE(
        (__metal_io_u32 *)(base + METAL_SIFIVE_GPIO0_INPUT_EN)) |= (1 << pin);

    return 0;
}

int metal_gpio_disable_input(struct metal_gpio *ggpio, int pin) {
    long base = __metal_driver_sifive_gpio0_base(ggpio);

    __METAL_ACCESS_ONCE(
        (__metal_io_u32 *)(base + METAL_SIFIVE_GPIO0_INPUT_EN)) &= ~(1 << pin);

    return 0;
}

int metal_gpio_disable_output(struct metal_gpio *ggpio, int pin) {
    long base = __metal_driver_sifive_gpio0_base(ggpio);

    __METAL_ACCESS_ONCE(
        (__metal_io_u32 *)(base + METAL_SIFIVE_GPIO0_OUTPUT_EN)) &= ~(1 << pin);

    return 0;
}

int metal_gpio_enable_output(struct metal_gpio *ggpio, int pin) {
    long base = __metal_driver_sifive_gpio0_base(ggpio);

    __METAL_ACCESS_ONCE(
        (__metal_io_u32 *)(base + METAL_SIFIVE_GPIO0_OUTPUT_EN)) |= (1 << pin);

    return 0;
}

int metal_gpio_set_pin(struct metal_gpio *ggpio, int pin, int value) {
    long base = __metal_driver_sifive_gpio0_base(ggpio);

    if (value == 1) {
        __METAL_ACCESS_ONCE(
            (__metal_io_u32 *)(base + METAL_SIFIVE_GPIO0_PORT)) |= (1 << pin);
        return 0;
    }
    return metal_gpio_clear_pin(ggpio, pin);
}

int metal_gpio_get_input_pin(struct metal_gpio *ggpio, int pin) {
    long base = __metal_driver_sifive_gpio0_base(ggpio);

    return __METAL_ACCESS_ONCE(
               (__metal_io_u32 *)(base + METAL_SIFIVE_GPIO0_VALUE)) &
           (1 << pin);
}

int metal_gpio_get_output_pin(struct metal_gpio *ggpio, int pin) {
    long base = __metal_driver_sifive_gpio0_base(ggpio);

    return __METAL_ACCESS_ONCE(
               (__metal_io_u32 *)(base + METAL_SIFIVE_GPIO0_PORT)) &
           (1 << pin);
}

int metal_gpio_clear_pin(struct metal_gpio *ggpio, int pin) {
    long base = __metal_driver_sifive_gpio0_base(ggpio);

    __METAL_ACCESS_ONCE((__metal_io_u32 *)(base + METAL_SIFIVE_GPIO0_PORT)) &=
        ~(1 << pin);

    return 0;
}

int metal_gpio_toggle_pin(struct metal_gpio *ggpio, int pin) {
    long base = __metal_driver_sifive_gpio0_base(ggpio);

    __METAL_ACCESS_ONCE((__metal_io_u32 *)(base + METAL_SIFIVE_GPIO0_PORT)) =
        __METAL_ACCESS_ONCE(
            (__metal_io_u32 *)(base + METAL_SIFIVE_GPIO0_PORT)) ^
        (1 << pin);

    return 0;
}

int metal_gpio_enable_pinmux(struct metal_gpio *ggpio, long pin_mask,
                             long io_function_mask) {
    long base = __metal_driver_sifive_gpio0_base(ggpio);

    __METAL_ACCESS_ONCE(
        (__metal_io_u32 *)(base + METAL_SIFIVE_GPIO0_IOF_SEL)) |= pin_mask;

    __METAL_ACCESS_ONCE((__metal_io_u32 *)(base + METAL_SIFIVE_GPIO0_IOF_EN)) &=
        ~pin_mask;
    __METAL_ACCESS_ONCE((__metal_io_u32 *)(base + METAL_SIFIVE_GPIO0_IOF_EN)) |=
        io_function_mask;

    return 0;
}

int metal_gpio_disable_pinmux(struct metal_gpio *ggpio, long pin_mask) {
    long base = __metal_driver_sifive_gpio0_base(ggpio);

    __METAL_ACCESS_ONCE((__metal_io_u32 *)(base + METAL_SIFIVE_GPIO0_IOF_EN)) &=
        ~pin_mask;

    return 0;
}

int metal_gpio_config_int(struct metal_gpio *ggpio, int pin, int intr_type) {
    long base = __metal_driver_sifive_gpio0_base(ggpio);

    switch (intr_type) {
    case METAL_GPIO_INT_DISABLE:
        __METAL_ACCESS_ONCE(
            (__metal_io_u32 *)(base + METAL_SIFIVE_GPIO0_RISE_IE)) &=
            ~(1 << pin);
        __METAL_ACCESS_ONCE(
            (__metal_io_u32 *)(base + METAL_SIFIVE_GPIO0_FALL_IE)) &=
            ~(1 << pin);
        __METAL_ACCESS_ONCE(
            (__metal_io_u32 *)(base + METAL_SIFIVE_GPIO0_HIGH_IE)) &=
            ~(1 << pin);
        __METAL_ACCESS_ONCE((
            __metal_io_u32 *)(base + METAL_SIFIVE_GPIO0_LOW_IE)) &= ~(1 << pin);
        break;
    case METAL_GPIO_INT_RISING:
        __METAL_ACCESS_ONCE((
            __metal_io_u32 *)(base + METAL_SIFIVE_GPIO0_RISE_IE)) |= (1 << pin);
        break;
    case METAL_GPIO_INT_FALLING:
        __METAL_ACCESS_ONCE((
            __metal_io_u32 *)(base + METAL_SIFIVE_GPIO0_FALL_IE)) |= (1 << pin);
        break;
    case METAL_GPIO_INT_BOTH_EDGE:
        __METAL_ACCESS_ONCE((
            __metal_io_u32 *)(base + METAL_SIFIVE_GPIO0_RISE_IE)) |= (1 << pin);
        __METAL_ACCESS_ONCE((
            __metal_io_u32 *)(base + METAL_SIFIVE_GPIO0_FALL_IE)) |= (1 << pin);
        break;
    case METAL_GPIO_INT_HIGH:
        __METAL_ACCESS_ONCE((
            __metal_io_u32 *)(base + METAL_SIFIVE_GPIO0_HIGH_IE)) |= (1 << pin);
        break;
    case METAL_GPIO_INT_LOW:
        __METAL_ACCESS_ONCE(
            (__metal_io_u32 *)(base + METAL_SIFIVE_GPIO0_LOW_IE)) |= (1 << pin);
        break;
    case METAL_GPIO_INT_BOTH_LEVEL:
        __METAL_ACCESS_ONCE((
            __metal_io_u32 *)(base + METAL_SIFIVE_GPIO0_HIGH_IE)) |= (1 << pin);
        __METAL_ACCESS_ONCE(
            (__metal_io_u32 *)(base + METAL_SIFIVE_GPIO0_LOW_IE)) |= (1 << pin);
        break;
    case METAL_GPIO_INT_MAX:
        __METAL_ACCESS_ONCE((
            __metal_io_u32 *)(base + METAL_SIFIVE_GPIO0_RISE_IE)) |= (1 << pin);
        __METAL_ACCESS_ONCE((
            __metal_io_u32 *)(base + METAL_SIFIVE_GPIO0_FALL_IE)) |= (1 << pin);
        __METAL_ACCESS_ONCE((
            __metal_io_u32 *)(base + METAL_SIFIVE_GPIO0_HIGH_IE)) |= (1 << pin);
        __METAL_ACCESS_ONCE(
            (__metal_io_u32 *)(base + METAL_SIFIVE_GPIO0_LOW_IE)) |= (1 << pin);
        break;
    }
    return 0;
}

int metal_gpio_clear_int(struct metal_gpio *ggpio, int pin, int intr_type) {
    long base = __metal_driver_sifive_gpio0_base(ggpio);

    switch (intr_type) {
    case METAL_GPIO_INT_RISING:
        __METAL_ACCESS_ONCE((
            __metal_io_u32 *)(base + METAL_SIFIVE_GPIO0_RISE_IP)) |= (1 << pin);
        break;
    case METAL_GPIO_INT_FALLING:
        __METAL_ACCESS_ONCE((
            __metal_io_u32 *)(base + METAL_SIFIVE_GPIO0_FALL_IP)) |= (1 << pin);
        break;
    case METAL_GPIO_INT_BOTH_EDGE:
        __METAL_ACCESS_ONCE((
            __metal_io_u32 *)(base + METAL_SIFIVE_GPIO0_RISE_IP)) |= (1 << pin);
        __METAL_ACCESS_ONCE((
            __metal_io_u32 *)(base + METAL_SIFIVE_GPIO0_FALL_IP)) |= (1 << pin);
        break;
    case METAL_GPIO_INT_HIGH:
        __METAL_ACCESS_ONCE((
            __metal_io_u32 *)(base + METAL_SIFIVE_GPIO0_HIGH_IP)) |= (1 << pin);
        break;
    case METAL_GPIO_INT_LOW:
        __METAL_ACCESS_ONCE(
            (__metal_io_u32 *)(base + METAL_SIFIVE_GPIO0_LOW_IP)) |= (1 << pin);
        break;
    case METAL_GPIO_INT_BOTH_LEVEL:
        __METAL_ACCESS_ONCE((
            __metal_io_u32 *)(base + METAL_SIFIVE_GPIO0_HIGH_IP)) |= (1 << pin);
        __METAL_ACCESS_ONCE(
            (__metal_io_u32 *)(base + METAL_SIFIVE_GPIO0_LOW_IP)) |= (1 << pin);
        break;
    case METAL_GPIO_INT_MAX:
        __METAL_ACCESS_ONCE((
            __metal_io_u32 *)(base + METAL_SIFIVE_GPIO0_RISE_IP)) |= (1 << pin);
        __METAL_ACCESS_ONCE((
            __metal_io_u32 *)(base + METAL_SIFIVE_GPIO0_FALL_IP)) |= (1 << pin);
        __METAL_ACCESS_ONCE((
            __metal_io_u32 *)(base + METAL_SIFIVE_GPIO0_HIGH_IP)) |= (1 << pin);
        __METAL_ACCESS_ONCE(
            (__metal_io_u32 *)(base + METAL_SIFIVE_GPIO0_LOW_IP)) |= (1 << pin);
        break;
    }
    return 0;
}

struct metal_interrupt *
metal_gpio_interrupt_controller(struct metal_gpio *gpio) {
    return __metal_driver_sifive_gpio0_interrupt_parent(gpio);
}

int metal_gpio_get_interrupt_id(struct metal_gpio *gpio, int pin) {
    int irq;
    irq = __metal_driver_sifive_gpio0_interrupt_lines(gpio, pin);
    return irq;
}

#endif /* METAL_SIFIVE_GPIO0 */

typedef int no_empty_translation_units;
