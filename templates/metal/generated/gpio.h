#ifndef __METAL_DT_GPIO__H
#define __METAL_DT_GPIO__H

{% include 'template_comment.h' %}

#define __METAL_DT_NUM_GPIOS {{ gpios|length }}

{% set compatible_string = to_snakecase(gpios[0].compatible[0]) %}
#define {{ compatible_string }}_enable_input metal_gpio_enable_input
#define {{ compatible_string }}_disable_input metal_gpio_disable_input
#define {{ compatible_string }}_enable_output metal_gpio_enable_output
#define {{ compatible_string }}_disable_output metal_gpio_disable_output
#define {{ compatible_string }}_set_pin metal_gpio_set_pin
#define {{ compatible_string }}_input_pin metal_gpio_get_input_pin
#define {{ compatible_string }}_output_pin metal_gpio_get_output_pin
#define {{ compatible_string }}_clear_pin metal_gpio_clear_pin
#define {{ compatible_string }}_toggle_pin metal_gpio_toggle_pin
#define {{ compatible_string }}_enable_pinmux metal_gpio_enable_pinmux
#define {{ compatible_string }}_disable_pinmux metal_gpio_disable_pinmux
#define {{ compatible_string }}_config_interrupt metal_gpio_config_interrupt
#define {{ compatible_string }}_clear_interrupt metal_gpio_clear_interrupt

#endif /* ! __METAL_DT_GPIO__H */
