
#include <metal/drivers/{{ driver_string }}.h>

#define dt_clock_get_rate_hz(clock) __metal_driver_{{ driver_string }}_get_rate_hz((clock))
#define dt_clock_set_rate_hz(clock, rate) __metal_driver_{{ driver_string }}_set_rate_hz((clock), (rate))
#define dt_clock_register_pre_rate_change_callback(clock, cb) __metal_driver_{{ driver_string }}_register_pre_rate_change_callback(((clock), (cb))
#define dt_clock_register_post_rate_change_callback(clock, cb) __metal_driver_{{ driver_string }}_register_post_rate_change_callback((clock), (cb))
