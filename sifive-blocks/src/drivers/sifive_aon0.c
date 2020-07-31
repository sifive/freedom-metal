/* SPDX-License-Identifier: Apache-2.0 */

#include <metal/machine/platform.h>

#ifdef METAL_SIFIVE_AON0

#include <limits.h>
#include <metal/generated/sifive_aon0.h>
#include <metal/io.h>
#include <metal/rtc.h>

/* RTCCFG */
#define RTCCFG_RTCSCALE_MASK 0xF
#define RTCCFG_ENALWAYS (1 << 12)
#define RTCCFG_IP0 (1 << 28)

/* RTCCMP0 */
#define METAL_RTCCMP0_MAX UINT32_MAX

#define RTC_REGW(offset)                                                       \
    __METAL_ACCESS_ONCE(                                                       \
        (__metal_io_u32 *)(METAL_SIFIVE_AON0_0_BASE_ADDRESS + (offset)))

uint64_t sifive_aon0_get_rtc_rate(struct metal_rtc rtc) {
    return metal_clock_get_rate_hz(AON_CLOCK);
}

uint64_t sifive_aon0_set_rtc_rate(struct metal_rtc rtc, const uint64_t rate) {
    return metal_clock_set_rate_hz(AON_CLOCK, rate);
}

uint64_t sifive_aon0_get_compare(struct metal_rtc rtc) {
    const uint32_t shift = __METAL_GET_FIELD(RTC_REGW(METAL_SIFIVE_AON0_RTCCFG),
                                             RTCCFG_RTCSCALE_MASK);

    return ((uint64_t)RTC_REGW(METAL_SIFIVE_AON0_RTCCMP0) << shift);
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
    RTC_REGW(METAL_SIFIVE_AON0_RTCCFG) = __METAL_SET_FIELD(
        RTC_REGW(METAL_SIFIVE_AON0_RTCCFG), RTCCFG_RTCSCALE_MASK, shift);

    /* Set the value of rtccmp0 */
    RTC_REGW(METAL_SIFIVE_AON0_RTCCMP0) = (uint32_t)comp_shifted;

    return sifive_aon0_get_compare(rtc);
}

uint64_t sifive_aon0_get_count(struct metal_rtc rtc) {
    uint64_t high, low = 0;

    do {
        high = RTC_REGW(METAL_SIFIVE_AON0_RTCCOUNTHI);
        low = RTC_REGW(METAL_SIFIVE_AON0_RTCCOUNTLO);
    } while (high != RTC_REGW(METAL_SIFIVE_AON0_RTCCOUNTHI));

    return ((high << 32) | low);
}

uint64_t sifive_aon0_set_count(struct metal_rtc rtc, const uint64_t count) {
    RTC_REGW(METAL_SIFIVE_AON0_RTCCOUNTHI) = (UINT_MAX & (count >> 32));
    RTC_REGW(METAL_SIFIVE_AON0_RTCCOUNTLO) = (UINT_MAX & count);

    return sifive_aon0_get_count(rtc);
}

int sifive_aon0_rtc_run(struct metal_rtc rtc,
                        const enum metal_rtc_run_option option) {
    switch (option) {
    default:
    case METAL_RTC_STOP:
        RTC_REGW(METAL_SIFIVE_AON0_RTCCFG) &= ~(RTCCFG_ENALWAYS);
        break;
    case METAL_RTC_RUN:
        RTC_REGW(METAL_SIFIVE_AON0_RTCCFG) |= RTCCFG_ENALWAYS;
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

#endif

typedef int no_empty_translation_units;
