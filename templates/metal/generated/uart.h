#ifndef __METAL_DT_UART__H
#define __METAL_DT_UART__H

{% include 'template_comment.h' %}

#define __METAL_DT_NUM_UARTS {{ uarts|length }}

{% set compat_string = to_snakecase(uarts[0].compatible[0]) %}
#define metal_uart_init {{ compat_string }}_init
#define metal_uart_putc {{ compat_string }}_putc
#define metal_uart_getc {{ compat_string }}_getc
#define metal_uart_get_baud_rate {{ compat_string }}_get_baud_rate
#define metal_uart_set_baud_rate {{ compat_string }}_set_baud_rate

#endif /* ! __METAL_DT_UART__H */
