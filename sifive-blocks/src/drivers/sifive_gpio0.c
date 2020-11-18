/* Copyright 2018 SiFive, Inc */
/* SPDX-License-Identifier: Apache-2.0 */

#include <metal/platform.h>

#ifdef METAL_SIFIVE_GPIO0

#include <metal/drivers/sifive_gpio0_regs.h>
#include <metal/gpio.h>
#include <metal/io.h>
#include <metal/private/metal_private_sifive_gpio0.h>

#define get_index(gpio) ((gpio).__gpio_index)

int sifive_gpio0_enable_input(struct metal_gpio gpio, int pin) {
    GPIO_Type *GPIO_regs = (GPIO_Type *)dt_gpio_data[get_index(gpio)].base_addr;

    GPIO_regs->INPUT_EN |= (1 << pin);

    return 0;
}

int sifive_gpio0_disable_input(struct metal_gpio gpio, int pin) {
    GPIO_Type *GPIO_regs = (GPIO_Type *)dt_gpio_data[get_index(gpio)].base_addr;

    GPIO_regs->INPUT_EN &= ~(1 << pin);

    return 0;
}

int sifive_gpio0_disable_output(struct metal_gpio gpio, int pin) {
    GPIO_Type *GPIO_regs = (GPIO_Type *)dt_gpio_data[get_index(gpio)].base_addr;

    GPIO_regs->OUTPUT_EN &= ~(1 << pin);

    return 0;
}

int sifive_gpio0_enable_output(struct metal_gpio gpio, int pin) {
    GPIO_Type *GPIO_regs = (GPIO_Type *)dt_gpio_data[get_index(gpio)].base_addr;

    GPIO_regs->OUTPUT_EN |= (1 << pin);

    return 0;
}

int sifive_gpio0_set_pin(struct metal_gpio gpio, int pin, int value) {
    GPIO_Type *GPIO_regs = (GPIO_Type *)dt_gpio_data[get_index(gpio)].base_addr;

    if (value == 1) {
        GPIO_regs->OUTPUT_VALUE |= (1 << pin);
        return 0;
    }
    return sifive_gpio0_clear_pin(gpio, pin);
}

int sifive_gpio0_get_input_pin(struct metal_gpio gpio, int pin) {
    GPIO_Type *GPIO_regs = (GPIO_Type *)dt_gpio_data[get_index(gpio)].base_addr;

    return (GPIO_regs->INPUT_VALUE & (1 << pin));
}

int sifive_gpio0_get_output_pin(struct metal_gpio gpio, int pin) {
    GPIO_Type *GPIO_regs = (GPIO_Type *)dt_gpio_data[get_index(gpio)].base_addr;

    return (GPIO_regs->OUTPUT_VALUE & (1 << pin));
}

int sifive_gpio0_clear_pin(struct metal_gpio gpio, int pin) {
    GPIO_Type *GPIO_regs = (GPIO_Type *)dt_gpio_data[get_index(gpio)].base_addr;

    GPIO_regs->OUTPUT_VALUE &= ~(1 << pin);

    return 0;
}

int sifive_gpio0_toggle_pin(struct metal_gpio gpio, int pin) {
    GPIO_Type *GPIO_regs = (GPIO_Type *)dt_gpio_data[get_index(gpio)].base_addr;

    GPIO_regs->OUTPUT_VALUE ^= (1 << pin);

    return 0;
}

int sifive_gpio0_enable_pinmux(struct metal_gpio gpio, long pin_mask,
                               long io_function_mask) {
    GPIO_Type *GPIO_regs = (GPIO_Type *)dt_gpio_data[get_index(gpio)].base_addr;

    GPIO_regs->IOF_SEL |= pin_mask;

    GPIO_regs->IOF_EN &= ~pin_mask;
    GPIO_regs->IOF_EN |= io_function_mask;

    return 0;
}

int sifive_gpio0_disable_pinmux(struct metal_gpio gpio, long pin_mask) {
    GPIO_Type *GPIO_regs = (GPIO_Type *)dt_gpio_data[get_index(gpio)].base_addr;

    GPIO_regs->IOF_EN &= ~pin_mask;

    return 0;
}

int sifive_gpio0_config_interrupt(struct metal_gpio gpio, int pin,
                                  enum metal_gpio_int_type int_type) {
    GPIO_Type *GPIO_regs = (GPIO_Type *)dt_gpio_data[get_index(gpio)].base_addr;

    switch (int_type) {
    case METAL_GPIO_INT_DISABLE:
        GPIO_regs->RISE_IE &= ~(1 << pin);
        GPIO_regs->FALL_IE &= ~(1 << pin);
        GPIO_regs->HIGH_IE &= ~(1 << pin);
        GPIO_regs->LOW_IE &= ~(1 << pin);
        break;
    case METAL_GPIO_INT_RISING:
        GPIO_regs->RISE_IE |= (1 << pin);
        break;
    case METAL_GPIO_INT_FALLING:
        GPIO_regs->FALL_IE |= (1 << pin);
        break;
    case METAL_GPIO_INT_BOTH_EDGE:
        GPIO_regs->RISE_IE |= (1 << pin);
        GPIO_regs->FALL_IE |= (1 << pin);
        break;
    case METAL_GPIO_INT_HIGH:
        GPIO_regs->HIGH_IE |= (1 << pin);
        break;
    case METAL_GPIO_INT_LOW:
        GPIO_regs->LOW_IE |= (1 << pin);
        break;
    case METAL_GPIO_INT_BOTH_LEVEL:
        GPIO_regs->HIGH_IE |= (1 << pin);
        GPIO_regs->LOW_IE |= (1 << pin);
        break;
    case METAL_GPIO_INT_MAX:
        GPIO_regs->RISE_IE |= (1 << pin);
        GPIO_regs->FALL_IE |= (1 << pin);
        GPIO_regs->HIGH_IE |= (1 << pin);
        GPIO_regs->LOW_IE |= (1 << pin);
        break;
    }

    struct metal_interrupt intc =
        dt_gpio_data[get_index((gpio))].interrupt_parent;
    int id = dt_gpio_data[get_index((gpio))].interrupt_id_base + pin;

    return metal_interrupt_enable(intc, id);
}

int sifive_gpio0_clear_interrupt(struct metal_gpio gpio, int pin,
                                 enum metal_gpio_int_type int_type) {
    GPIO_Type *GPIO_regs = (GPIO_Type *)dt_gpio_data[get_index(gpio)].base_addr;

    switch (int_type) {
    case METAL_GPIO_INT_RISING:
        GPIO_regs->RISE_IP |= (1 << pin);
        break;
    case METAL_GPIO_INT_FALLING:
        GPIO_regs->FALL_IP |= (1 << pin);
        break;
    case METAL_GPIO_INT_BOTH_EDGE:
        GPIO_regs->RISE_IP |= (1 << pin);
        GPIO_regs->FALL_IP |= (1 << pin);
        break;
    case METAL_GPIO_INT_HIGH:
        GPIO_regs->HIGH_IP |= (1 << pin);
        break;
    case METAL_GPIO_INT_LOW:
        GPIO_regs->LOW_IP |= (1 << pin);
        break;
    case METAL_GPIO_INT_BOTH_LEVEL:
        GPIO_regs->HIGH_IP |= (1 << pin);
        GPIO_regs->LOW_IP |= (1 << pin);
        break;
    case METAL_GPIO_INT_MAX:
        GPIO_regs->RISE_IP |= (1 << pin);
        GPIO_regs->FALL_IP |= (1 << pin);
        GPIO_regs->HIGH_IP |= (1 << pin);
        GPIO_regs->LOW_IP |= (1 << pin);
        break;
    default:
        return -1;
    }
    return 0;
}

#endif /* METAL_SIFIVE_GPIO0 */

typedef int no_empty_translation_units;
