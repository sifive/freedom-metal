/* Copyright 2020 SiFive, Inc */
/* SPDX-License-Identifier: Apache-2.0 */

#include <metal/machine/platform.h>

#ifdef METAL_SIFIVE_NB2GPIO0

#include <metal/drivers/sifive_nb2gpio0.h>
#include <metal/io.h>
#include <metal/machine.h>

int __metal_driver_sifive_nb2gpio0_enable_input(struct metal_gpio *ggpio, nb2_port_name port, long source)
{
	long base = __metal_driver_sifive_nb2gpio0_base(ggpio);

	/* Set Port = 0 for INPUT */
	switch(port)
	{
		case METAL_GPIO_PORT_A:
			__METAL_ACCESS_ONCE((__metal_io_u32 *)(base + METAL_SIFIVE_NB2GPIO0_GPIO_SWPORTA_DDR))  &= ~source;
			break;

		case METAL_GPIO_PORT_B:
			__METAL_ACCESS_ONCE((__metal_io_u32 *)(base + METAL_SIFIVE_NB2GPIO0_GPIO_SWPORTB_DDR))  &= ~source;
			break;

		case METAL_GPIO_PORT_C:
			__METAL_ACCESS_ONCE((__metal_io_u32 *)(base + METAL_SIFIVE_NB2GPIO0_GPIO_SWPORTC_DDR))  &= ~source;
			break;

		case METAL_GPIO_PORT_D:
			__METAL_ACCESS_ONCE((__metal_io_u32 *)(base + METAL_SIFIVE_NB2GPIO0_GPIO_SWPORTD_DDR))  &= ~source;
			break;

		default:
			break;
	}
	
	return 0;
}

int __metal_driver_sifive_nb2gpio0_disable_input(struct metal_gpio *ggpio, long source)
{
	/* Disable is not required for this IP, its ether INPUT OR OUTPUT */
	/* 0 = INPUT , 1 = OUTPUT */

	return 0;
}

long __metal_driver_sifive_nb2gpio0_input(struct metal_gpio *ggpio)
{
	long base = __metal_driver_sifive_nb2gpio0_base(ggpio);

	return __METAL_ACCESS_ONCE((__metal_io_u32 *)(base + METAL_SIFIVE_NB2GPIO0_GPIO_SWPORTA_DR));
}

long __metal_driver_sifive_nb2gpio0_output(struct metal_gpio *ggpio)
{
	long base = __metal_driver_sifive_nb2gpio0_base(ggpio);

	return __METAL_ACCESS_ONCE((__metal_io_u32 *)(base + METAL_SIFIVE_NB2GPIO0_GPIO_SWPORTA_DR));
}


int __metal_driver_sifive_nb2gpio0_disable_output(struct metal_gpio *ggpio, long source)
{
	/* Disable is not required for this IP, its ether INPUT OR OUTPUT */
	/* 0 = INPUT , 1 = OUTPUT */

	return 0;
}

int __metal_driver_sifive_nb2gpio0_enable_output(struct metal_gpio *ggpio, nb2_port_name port, long source)
{
	long base = __metal_driver_sifive_nb2gpio0_base(ggpio);

	/* Set Port = 1 for OUTPUT */
	switch(port)
	{
		case METAL_GPIO_PORT_A:
			__METAL_ACCESS_ONCE((__metal_io_u32 *)(base + METAL_SIFIVE_NB2GPIO0_GPIO_SWPORTA_DDR)) |= source;
			break;

		case METAL_GPIO_PORT_B:
			__METAL_ACCESS_ONCE((__metal_io_u32 *)(base + METAL_SIFIVE_NB2GPIO0_GPIO_SWPORTB_DDR)) |= source;
			break;

		case METAL_GPIO_PORT_C:
			__METAL_ACCESS_ONCE((__metal_io_u32 *)(base + METAL_SIFIVE_NB2GPIO0_GPIO_SWPORTC_DDR)) |= source;
			break;

		case METAL_GPIO_PORT_D:
			__METAL_ACCESS_ONCE((__metal_io_u32 *)(base + METAL_SIFIVE_NB2GPIO0_GPIO_SWPORTD_DDR)) |= source;
			break;

		default:
			break;
	}

	return 0;
}

int __metal_driver_sifive_nb2gpio0_output_set(struct metal_gpio *ggpio, nb2_port_name port, long value)
{
	long base = __metal_driver_sifive_nb2gpio0_base(ggpio);

	switch(port)
	{
		case METAL_GPIO_PORT_A:
			__METAL_ACCESS_ONCE((__metal_io_u32 *)(base + METAL_SIFIVE_NB2GPIO0_GPIO_SWPORTA_DR)) |= value;
			break;

		case METAL_GPIO_PORT_B:
			__METAL_ACCESS_ONCE((__metal_io_u32 *)(base + METAL_SIFIVE_NB2GPIO0_GPIO_SWPORTB_DR)) |= value;
			break;

		case METAL_GPIO_PORT_C:
			__METAL_ACCESS_ONCE((__metal_io_u32 *)(base + METAL_SIFIVE_NB2GPIO0_GPIO_SWPORTC_DR)) |= value;
			break;

		case METAL_GPIO_PORT_D:
			__METAL_ACCESS_ONCE((__metal_io_u32 *)(base + METAL_SIFIVE_NB2GPIO0_GPIO_SWPORTD_DR)) |= value;
			break;

		default:
			break;
	}

	return 0;
}

int __metal_driver_sifive_nb2gpio0_output_clear(struct metal_gpio *ggpio, nb2_port_name port, long value)
{
	long base = __metal_driver_sifive_nb2gpio0_base(ggpio);

        switch(port)
	{
		case METAL_GPIO_PORT_A:
			__METAL_ACCESS_ONCE((__metal_io_u32 *)(base + METAL_SIFIVE_NB2GPIO0_GPIO_SWPORTA_DR)) &= ~value;
			break;

		case METAL_GPIO_PORT_B:
			__METAL_ACCESS_ONCE((__metal_io_u32 *)(base + METAL_SIFIVE_NB2GPIO0_GPIO_SWPORTB_DR)) &= ~value;
			break;

		case METAL_GPIO_PORT_C:
			__METAL_ACCESS_ONCE((__metal_io_u32 *)(base + METAL_SIFIVE_NB2GPIO0_GPIO_SWPORTC_DR)) &= ~value;
			break;

		case METAL_GPIO_PORT_D:
			__METAL_ACCESS_ONCE((__metal_io_u32 *)(base + METAL_SIFIVE_NB2GPIO0_GPIO_SWPORTD_DR)) &= ~value;
			break;

		default:
			break;
	}

	return 0;
}

int __metal_driver_sifive_nb2gpio0_output_toggle(struct metal_gpio *ggpio, nb2_port_name port, long value)
{
	long base = __metal_driver_sifive_nb2gpio0_base(ggpio);

	switch(port)
	{
		case METAL_GPIO_PORT_A:
			__METAL_ACCESS_ONCE((__metal_io_u32 *)(base + METAL_SIFIVE_NB2GPIO0_GPIO_SWPORTA_DR)) =
				__METAL_ACCESS_ONCE((__metal_io_u32 *)(base + METAL_SIFIVE_NB2GPIO0_GPIO_SWPORTA_DR)) ^ value;
			break;

		case METAL_GPIO_PORT_B:
			__METAL_ACCESS_ONCE((__metal_io_u32 *)(base + METAL_SIFIVE_NB2GPIO0_GPIO_SWPORTB_DR)) =
				__METAL_ACCESS_ONCE((__metal_io_u32 *)(base + METAL_SIFIVE_NB2GPIO0_GPIO_SWPORTB_DR)) ^ value;
			break;

		case METAL_GPIO_PORT_C:
			__METAL_ACCESS_ONCE((__metal_io_u32 *)(base + METAL_SIFIVE_NB2GPIO0_GPIO_SWPORTC_DR)) =
				__METAL_ACCESS_ONCE((__metal_io_u32 *)(base + METAL_SIFIVE_NB2GPIO0_GPIO_SWPORTC_DR)) ^ value;
			break;

		case METAL_GPIO_PORT_D:
			__METAL_ACCESS_ONCE((__metal_io_u32 *)(base + METAL_SIFIVE_NB2GPIO0_GPIO_SWPORTD_DR)) =
				__METAL_ACCESS_ONCE((__metal_io_u32 *)(base + METAL_SIFIVE_NB2GPIO0_GPIO_SWPORTD_DR)) ^ value;
			break;

		default:
			break;
	}

	return 0;
}

int __metal_driver_sifive_nb2gpio0_enable_io(struct metal_gpio *ggpio, long source, long dest)
{
	/* Not implemented */
   	 return 0;
}

int __metal_driver_sifive_nb2gpio0_disable_io(struct metal_gpio *ggpio, long source)
{
	/* Not implemented */
    	return 0;
}

int __metal_driver_sifive_nb2gpio0_config_int(struct metal_gpio *ggpio, long source, int intr_type)
{
	long base = __metal_driver_sifive_nb2gpio0_base(ggpio);

	switch (intr_type)
	{
		case METAL_GPIO_INT_DISABLE:
			__METAL_ACCESS_ONCE((__metal_io_u32 *)(base + METAL_SIFIVE_NB2GPIO0_GPIO_INTEN)) &= ~source;
			break;

		case METAL_GPIO_INT_ENABLE:
			__METAL_ACCESS_ONCE((__metal_io_u32 *)(base + METAL_SIFIVE_NB2GPIO0_GPIO_INTEN)) &= ~source;
			__METAL_ACCESS_ONCE((__metal_io_u32 *)(base + METAL_SIFIVE_NB2GPIO0_GPIO_INTEN)) |= source;
			break;

		case METAL_GPIO_INT_MASK_DISABLE:
			__METAL_ACCESS_ONCE((__metal_io_u32 *)(base + METAL_SIFIVE_NB2GPIO0_GPIO_INTMASK)) &= ~source;
			break;

		case METAL_GPIO_INT_MASK_ENABLE:
			__METAL_ACCESS_ONCE((__metal_io_u32 *)(base + METAL_SIFIVE_NB2GPIO0_GPIO_INTMASK)) &= ~source;
			__METAL_ACCESS_ONCE((__metal_io_u32 *)(base + METAL_SIFIVE_NB2GPIO0_GPIO_INTMASK)) |= source;
			break;

		case METAL_GPIO_INTTYPE_LEVEL:
			__METAL_ACCESS_ONCE((__metal_io_u32 *)(base + METAL_SIFIVE_NB2GPIO0_GPIO_INTTYPE_LEVEL)) &= ~source;
			break;

		case METAL_GPIO_INTTYPE_EDGE:
			__METAL_ACCESS_ONCE((__metal_io_u32 *)(base + METAL_SIFIVE_NB2GPIO0_GPIO_INTTYPE_LEVEL)) &= ~source;
			__METAL_ACCESS_ONCE((__metal_io_u32 *)(base + METAL_SIFIVE_NB2GPIO0_GPIO_INTTYPE_LEVEL)) |= source;
			break;

		case METAL_GPIO_INT_RISING:
			__METAL_ACCESS_ONCE((__metal_io_u32 *)(base + METAL_SIFIVE_NB2GPIO0_GPIO_INT_POLARITY)) &= ~source;
			__METAL_ACCESS_ONCE((__metal_io_u32 *)(base + METAL_SIFIVE_NB2GPIO0_GPIO_INT_POLARITY)) |= source;
			break;

		case METAL_GPIO_INT_FALLING:
			__METAL_ACCESS_ONCE((__metal_io_u32 *)(base + METAL_SIFIVE_NB2GPIO0_GPIO_INT_POLARITY)) &= ~source;
			break;

		case METAL_GPIO_INT_HIGH:
			__METAL_ACCESS_ONCE((__metal_io_u32 *)(base + METAL_SIFIVE_NB2GPIO0_GPIO_INT_POLARITY)) &= ~source;
			__METAL_ACCESS_ONCE((__metal_io_u32 *)(base + METAL_SIFIVE_NB2GPIO0_GPIO_INT_POLARITY)) |= source;
			break;

		case METAL_GPIO_INT_LOW:
			__METAL_ACCESS_ONCE((__metal_io_u32 *)(base + METAL_SIFIVE_NB2GPIO0_GPIO_INT_POLARITY)) &= ~source;
			break;

		default:
			break;
	}

	return 0;
}

int __metal_driver_sifive_nb2gpio0_clear_int(struct metal_gpio *ggpio, long source, int intr_clr)
{
	long base = __metal_driver_sifive_nb2gpio0_base(ggpio);

	if (intr_clr)
	{
		__METAL_ACCESS_ONCE((__metal_io_u32 *)(base + METAL_SIFIVE_NB2GPIO0_GPIO_PORTA_EOI)) &= ~source;
		__METAL_ACCESS_ONCE((__metal_io_u32 *)(base + METAL_SIFIVE_NB2GPIO0_GPIO_PORTA_EOI)) |= source;
	}

	return 0;
}

struct metal_interrupt *
__metal_driver_gpio_interrupt_controller(struct metal_gpio *gpio)
{
	return __metal_driver_sifive_nb2gpio0_interrupt_parent(gpio);
}

int __metal_driver_gpio_get_interrupt_id(struct metal_gpio *gpio, int pin)
{
	int irq;
	irq = __metal_driver_sifive_nb2gpio0_interrupt_lines(gpio, pin);
	return irq;
}

__METAL_DEFINE_VTABLE(__metal_driver_vtable_sifive_nb2gpio0) = {
    .gpio.disable_input  = __metal_driver_sifive_nb2gpio0_disable_input,
    .gpio.enable_input   = __metal_driver_sifive_nb2gpio0_enable_input,
    .gpio.input          = __metal_driver_sifive_nb2gpio0_input,
    .gpio.output         = __metal_driver_sifive_nb2gpio0_output,
    .gpio.disable_output = __metal_driver_sifive_nb2gpio0_disable_output,
    .gpio.enable_output  = __metal_driver_sifive_nb2gpio0_enable_output,
    .gpio.output_set     = __metal_driver_sifive_nb2gpio0_output_set,
    .gpio.output_clear   = __metal_driver_sifive_nb2gpio0_output_clear,
    .gpio.output_toggle  = __metal_driver_sifive_nb2gpio0_output_toggle,
    .gpio.enable_io      = __metal_driver_sifive_nb2gpio0_enable_io,
    .gpio.disable_io     = __metal_driver_sifive_nb2gpio0_disable_io,
    .gpio.config_int     = __metal_driver_sifive_nb2gpio0_config_int,
    .gpio.clear_int      = __metal_driver_sifive_nb2gpio0_clear_int,
    .gpio.interrupt_controller = __metal_driver_gpio_interrupt_controller,
    .gpio.get_interrupt_id = __metal_driver_gpio_get_interrupt_id,
};

#endif /* METAL_SIFIVE_NB2GPIO0 */

typedef int no_empty_translation_units;
