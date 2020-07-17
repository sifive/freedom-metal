#define _POSIX_MONOTONIC_CLOCK 200809L
#define _POSIX_TIMERS
#include <errno.h>
#include <metal/cpu.h>
#include <time.h>

#ifdef MTIME_RATE_HZ_DEF
#undef MTIME_RATE_HZ
#define MTIME_RATE_HZ MTIME_RATE_HZ_DEF
#endif

#ifndef MTIME_RATE_HZ
#define MTIME_RATE_HZ 32768
#endif

int clock_getres(clockid_t clk_id, struct timespec *res) {
    switch (clk_id) {
    case CLOCK_MONOTONIC:
        res->tv_sec = 0;
        res->tv_nsec = 1000000000 / MTIME_RATE_HZ;
        return 0;
        break;
    default:
        errno = EINVAL;
        return -1;
    }
}

int clock_gettime(clockid_t clk_id, struct timespec *tp) {
    unsigned long long ticks;
    struct metal_cpu cpu = (struct metal_cpu){0};

    switch (clk_id) {
    case CLOCK_MONOTONIC:
        ticks = metal_cpu_get_mtime(cpu);
        tp->tv_sec = ticks / MTIME_RATE_HZ;
        tp->tv_nsec = ((ticks % (MTIME_RATE_HZ)) * 1000000000) / MTIME_RATE_HZ;
        return 0;
        break;
    default:
        errno = EINVAL;
        return -1;
    }
}

int clock_settime(clockid_t clk_id, const struct timespec *tp) {
    errno = EINVAL;
    return -1;
}
