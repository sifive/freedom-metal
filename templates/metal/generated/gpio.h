#ifndef __METAL_DT_GPIO__H
#define __METAL_DT_GPIO__H

{% include 'template_comment.h' %}

#define __METAL_DT_NUM_GPIOS {{ gpios|length }}

{% set compatible_string = to_snakecase(gpios[0].compatible[0]) %}
#define metal_gpio_enable_input {{ compatible_string }}_enable_input
#define metal_gpio_disable_input {{ compatible_string }}_disable_input
#define metal_gpio_enable_output {{ compatible_string }}_enable_output
#define metal_gpio_disable_output {{ compatible_string }}_disable_output
#define metal_gpio_set_pin {{ compatible_string }}_set_pin
#define metal_gpio_input_pin {{ compatible_string }}_input_pin
#define metal_gpio_output_pin {{ compatible_string }}_output_pin
#define metal_gpio_clear_pin {{ compatible_string }}_clear_pin
#define metal_gpio_toggle_pin {{ compatible_string }}_toggle_pin
#define metal_gpio_enable_pinmux {{ compatible_string }}_enable_pinmux
#define metal_gpio_disable_pinmux {{ compatible_string }}_disable_pinmux
#define metal_gpio_config_interrupt {{ compatible_string }}_config_interrupt
#define metal_gpio_clear_interrupt {{ compatible_string }}_clear_interrupt

#endif /* ! __METAL_DT_GPIO__H */
