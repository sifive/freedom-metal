#ifndef __METAL_PLATFORM__H
#define __METAL_PLATFORM__H

{% include 'template_comment.h' %}

{% for device in devices %}
  #define {{ device.define }}
  {% for reg in device.registers %}
    #define {{ reg.define }} {{ reg.address }}
  {% endfor %}

  {% for instance in device.instances %}
	{% for property in instance.properties %}
      #define {{ property.define }} {{ property.value }}
	{% endfor %}
  {% endfor %}
{% endfor %}

#endif /* ! __METAL_PLATFORM__H */