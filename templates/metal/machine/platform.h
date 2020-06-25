#ifndef __METAL_PLATFORM__H
#define __METAL_PLATFORM__H

{% include 'template_comment.h' %}

{% for device in devices %}
#include <metal/machine/{{ to_snakecase(device) }}.h>
{% endfor %}

#endif /* ! __METAL_PLATFORM__H */
