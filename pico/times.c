#include <metal/timer.h>
#include <sys/time.h>
#include <sys/times.h>

/*
 * Simple enough to return the tick count by
 * just getting it from the hardware.
 */

clock_t times(struct tms *buf) {
    unsigned long long mcc;
    metal_timer_get_cyclecount(0, &mcc);
    buf->tms_stime = 0;
    buf->tms_cutime = 0;
    buf->tms_cstime = 0;
    return buf->tms_utime = mcc;
}
