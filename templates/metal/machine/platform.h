#ifndef __METAL_PLATFORM__H
#define __METAL_PLATFORM__H

{% include 'template_comment.h' %}

{% for api in devices %}
    {% for device in devices[api] %}
#include <metal/machine/{{ to_snakecase(device) }}.h>
    {% endfor %}
{% endfor %}

#endif /* ! __METAL_PLATFORM__H */
