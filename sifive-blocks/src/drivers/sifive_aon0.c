/* SPDX-License-Identifier: Apache-2.0 */

#include <metal/machine/platform.h>

#ifdef METAL_SIFIVE_AON0

#include <limits.h>
#include <metal/generated/sifive_aon0.h>
#include <metal/io.h>
#include <metal/rtc.h>
#include <metal/watchdog.h>

/* WDOGCFG */
#define METAL_WDOGCFG_SCALE_MASK 7
#define METAL_WDOGCFG_RSTEN (1 << 8)
#define METAL_WDOGCFG_ZEROCMP (1 << 9)
#define METAL_WDOGCFG_ENALWAYS (1 << 12)
#define METAL_WDOGCFG_COREAWAKE (1 << 13)
#define METAL_WDOGCFG_IP (1 << 28)

/* WDOGCMP */
#define METAL_WDOGCMP_MASK 0xFFFF

/* RTCCFG */
#define RTCCFG_RTCSCALE_MASK 0xF
#define RTCCFG_ENALWAYS (1 << 12)
#define RTCCFG_IP0 (1 << 28)

/* RTCCMP0 */
#define METAL_RTCCMP0_MAX UINT32_MAX

#define AON_REGW(offset)                                                       \
    __METAL_ACCESS_ONCE(                                                       \
        (__metal_io_u32 *)(METAL_SIFIVE_AON0_0_BASE_ADDRESS + (offset)))

/* All writes to watchdog registers must be precedded by a write of
 * a magic number to WDOGKEY */
#define WDOG_UNLOCK                                                            \
    (AON_REGW(METAL_SIFIVE_AON0_WDOGKEY) = METAL_SIFIVE_AON0_MAGIC_KEY)

#define WDOG_UNLOCK_REGW(offset)                                               \
    WDOG_UNLOCK;                                                               \
    AON_REGW(offset)

/* RTC API */

uint64_t sifive_aon0_get_rtc_rate(struct metal_rtc rtc) {
    return metal_clock_get_rate_hz(AON_CLOCK);
}

uint64_t sifive_aon0_set_rtc_rate(struct metal_rtc rtc, const uint64_t rate) {
    return metal_clock_set_rate_hz(AON_CLOCK, rate);
}

uint64_t sifive_aon0_get_compare(struct metal_rtc rtc) {
    const uint32_t shift = __METAL_GET_FIELD(AON_REGW(METAL_SIFIVE_AON0_RTCCFG),
                                             RTCCFG_RTCSCALE_MASK);

    return ((uint64_t)AON_REGW(METAL_SIFIVE_AON0_RTCCMP0) << shift);
}

uint64_t sifive_aon0_set_compare(struct metal_rtc rtc, const uint64_t compare) {
    /* Determine the bit shift and shifted value to store in
     * rtccmp0/rtccfg.scale */
    uint32_t shift = 0;
    uint64_t comp_shifted = compare;
    while (comp_shifted > METAL_RTCCMP0_MAX) {
        shift += 1;
        comp_shifted = comp_shifted >> shift;
    }

    /* Set the value of rtccfg.scale */
    AON_REGW(METAL_SIFIVE_AON0_RTCCFG) = __METAL_SET_FIELD(
        AON_REGW(METAL_SIFIVE_AON0_RTCCFG), RTCCFG_RTCSCALE_MASK, shift);

    /* Set the value of rtccmp0 */
    AON_REGW(METAL_SIFIVE_AON0_RTCCMP0) = (uint32_t)comp_shifted;

    return sifive_aon0_get_compare(rtc);
}

uint64_t sifive_aon0_get_count(struct metal_rtc rtc) {
    uint64_t high, low = 0;

    do {
        high = AON_REGW(METAL_SIFIVE_AON0_RTCCOUNTHI);
        low = AON_REGW(METAL_SIFIVE_AON0_RTCCOUNTLO);
    } while (high != AON_REGW(METAL_SIFIVE_AON0_RTCCOUNTHI));

    return ((high << 32) | low);
}

uint64_t sifive_aon0_set_count(struct metal_rtc rtc, const uint64_t count) {
    AON_REGW(METAL_SIFIVE_AON0_RTCCOUNTHI) = (UINT_MAX & (count >> 32));
    AON_REGW(METAL_SIFIVE_AON0_RTCCOUNTLO) = (UINT_MAX & count);

    return sifive_aon0_get_count(rtc);
}

int sifive_aon0_rtc_run(struct metal_rtc rtc,
                        const enum metal_rtc_run_option option) {
    switch (option) {
    default:
    case METAL_RTC_STOP:
        AON_REGW(METAL_SIFIVE_AON0_RTCCFG) &= ~(RTCCFG_ENALWAYS);
        break;
    case METAL_RTC_RUN:
        AON_REGW(METAL_SIFIVE_AON0_RTCCFG) |= RTCCFG_ENALWAYS;
        break;
    }

    return 0;
}

int sifive_aon0_enable_rtc_interrupt(struct metal_rtc rtc) {
    metal_interrupt_enable(AON_INTERRUPT_PARENT, AON_RTC_INTERRUPT_ID);
}

int sifive_aon0_disable_rtc_interrupt(struct metal_rtc rtc) {
    metal_interrupt_disable(AON_INTERRUPT_PARENT, AON_RTC_INTERRUPT_ID);
}

/* Watchdog API */

int sifive_aon0_feed(struct metal_watchdog wdog) {

    AON_REGW(METAL_SIFIVE_AON0_WDOGFEED) = METAL_SIFIVE_AON0_MAGIC_KEY;

    return 0;
}

uint64_t sifive_aon0_get_wdog_rate(struct metal_watchdog wdog) {
    const uint64_t clock_rate = metal_clock_get_rate_hz(AON_CLOCK);

    if (clock_rate == 0)
        return -1;

    const unsigned int scale =
        (AON_REGW(METAL_SIFIVE_AON0_WDOGCFG) & METAL_WDOGCFG_SCALE_MASK);

    return clock_rate / (1 << scale);
}

uint64_t sifive_aon0_set_wdog_rate(struct metal_watchdog wdog,
                                   const uint64_t rate) {
    const uint64_t clock_rate = metal_clock_get_rate_hz(AON_CLOCK);

    if (rate >= clock_rate) {
        /* We can't scale the rate above the driving clock. Clear the scale
         * field and return the driving clock rate */
        WDOG_UNLOCK_REGW(METAL_SIFIVE_AON0_WDOGCFG) &=
            ~(METAL_WDOGCFG_SCALE_MASK);
        return clock_rate;
    }

    /* Look for the closest scale value */
    long min_diff = LONG_MAX;
    unsigned int min_scale = 0;
    for (int i = 0; i < METAL_WDOGCFG_SCALE_MASK; i++) {
        const uint64_t new_rate = clock_rate / (1 << i);

        uint64_t diff = rate - new_rate;
        if (diff < 0)
            diff *= -1;

        if (diff < min_diff) {
            min_diff = diff;
            min_scale = i;
        }
    }

    WDOG_UNLOCK_REGW(METAL_SIFIVE_AON0_WDOGCFG) &= ~(METAL_WDOGCFG_SCALE_MASK);
    WDOG_UNLOCK_REGW(METAL_SIFIVE_AON0_WDOGCFG) |=
        (METAL_WDOGCFG_SCALE_MASK & min_scale);

    return clock_rate / (1 << min_scale);
}

metal_watchdog_count_t sifive_aon0_get_timeout(struct metal_watchdog wdog) {
    return (AON_REGW(METAL_SIFIVE_AON0_WDOGCMP0) & METAL_WDOGCMP_MASK);
}

metal_watchdog_count_t
sifive_aon0_set_timeout(struct metal_watchdog wdog,
                        const metal_watchdog_count_t timeout) {
    /* Cap the timeout at the max value */
    const metal_watchdog_count_t set_timeout =
        timeout > METAL_WDOGCMP_MASK ? METAL_WDOGCMP_MASK : timeout;

    /* If we edit the timeout value in-place by masking the compare value to 0
     * and then writing it, we cause a spurious interrupt because the compare
     * value is temporarily 0. Instead, read the value into a local variable,
     * modify it there, and then write the whole register back */
    uint32_t wdogcmp = AON_REGW(METAL_SIFIVE_AON0_WDOGCMP0);

    wdogcmp &= ~(METAL_WDOGCMP_MASK);
    wdogcmp |= set_timeout;

    WDOG_UNLOCK_REGW(METAL_SIFIVE_AON0_WDOGCMP0) = wdogcmp;

    return set_timeout;
}

int sifive_aon0_set_result(struct metal_watchdog wdog,
                           const enum metal_watchdog_result result) {
    /* Turn off reset enable and counter reset */
    WDOG_UNLOCK_REGW(METAL_SIFIVE_AON0_WDOGCFG) &=
        ~(METAL_WDOGCFG_RSTEN | METAL_WDOGCFG_ZEROCMP);

    switch (result) {
    default:
    case METAL_WATCHDOG_NO_RESULT:
        break;
    case METAL_WATCHDOG_INTERRUPT:
        /* Reset counter to zero after match */
        WDOG_UNLOCK_REGW(METAL_SIFIVE_AON0_WDOGCFG) |= METAL_WDOGCFG_ZEROCMP;
        break;
    case METAL_WATCHDOG_FULL_RESET:
        WDOG_UNLOCK_REGW(METAL_SIFIVE_AON0_WDOGCFG) |= METAL_WDOGCFG_RSTEN;
        break;
    }

    return 0;
}

int sifive_aon0_wdog_run(struct metal_watchdog wdog,
                         const enum metal_watchdog_run_option option) {
    WDOG_UNLOCK_REGW(METAL_SIFIVE_AON0_WDOGCFG) &=
        ~(METAL_WDOGCFG_ENALWAYS | METAL_WDOGCFG_COREAWAKE);

    switch (option) {
    default:
    case METAL_WATCHDOG_STOP:
        break;
    case METAL_WATCHDOG_RUN_ALWAYS:
        /* Feed the watchdog before starting to reset counter */
        sifive_aon0_feed(wdog);

        WDOG_UNLOCK_REGW(METAL_SIFIVE_AON0_WDOGCFG) |= METAL_WDOGCFG_ENALWAYS;
        break;
    case METAL_WATCHDOG_RUN_AWAKE:
        /* Feed the watchdog before starting to reset counter */
        sifive_aon0_feed(wdog);

        WDOG_UNLOCK_REGW(METAL_SIFIVE_AON0_WDOGCFG) |= METAL_WDOGCFG_COREAWAKE;
        break;
    }

    return 0;
}

int sifive_aon0_enable_wdog_interrupt(struct metal_watchdog wdog) {
    metal_interrupt_enable(AON_INTERRUPT_PARENT, AON_WDOG_INTERRUPT_ID);
}

int sifive_aon0_disable_wdog_interrupt(struct metal_watchdog wdog) {
    metal_interrupt_disable(AON_INTERRUPT_PARENT, AON_WDOG_INTERRUPT_ID);
}

int sifive_aon0_clear_wdog_interrupt(struct metal_watchdog wdog) {
    /* Clear the interrupt pending bit */
    WDOG_UNLOCK_REGW(METAL_SIFIVE_AON0_WDOGCFG) &= ~(METAL_WDOGCFG_IP);

    return 0;
}

#endif

typedef int no_empty_translation_units;
