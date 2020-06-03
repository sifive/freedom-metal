/* Copyright 2019 SiFive, Inc */
/* SPDX-License-Identifier: Apache-2.0 */

#ifdef METAL_SIFIVE_RTC0

#include <metal/drivers/sifive_rtc0.h>
#include <metal/io.h>

/* RTCCFG */
#define METAL_RTCCFG_RTCSCALE_MASK 0xF
#define METAL_RTCCFG_ENALWAYS (1 << 12)
#define METAL_RTCCFG_IP0 (1 << 28)

/* RTCCMP0 */
#define METAL_RTCCMP0_MAX UINT32_MAX

#define RTC_REG(base, offset) (((unsigned long)base + offset))
#define RTC_REGW(base, offset)                                                 \
    (__METAL_ACCESS_ONCE((__metal_io_u32 *)RTC_REG(base, offset)))

uint64_t metal_rtc_get_rate(const struct metal_rtc *const rtc) {
    struct metal_clock clock = dt_rtc_data[get_index(rtc)].clock;
    return metal_clock_get_rate_hz(clock);
}

uint64_t metal_rtc_set_rate(const struct metal_rtc *const rtc,
                            const uint64_t rate) {
    struct metal_clock clock = dt_rtc_data[get_index(rtc)].clock;
    return metal_clock_get_rate_hz(clock);
}

uint64_t metal_rtc_get_compare(const struct metal_rtc *const rtc) {
    uintptr_t base = dt_rtc_data[get_inedx(rtc)].base_addr;

    const uint32_t shift =
        RTC_REGW(base, METAL_SIFIVE_RTC0_RTCCFG) & METAL_RTCCFG_RTCSCALE_MASK;

    return ((uint64_t)RTC_REGW(base, METAL_SIFIVE_RTC0_RTCCMP0) << shift);
}

uint64_t metal_rtc_set_compare(const struct metal_rtc *const rtc,
                               const uint64_t compare) {
    uintptr_t base = dt_rtc_data[get_inedx(rtc)].base_addr;

    /* Determine the bit shift and shifted value to store in
     * rtccmp0/rtccfg.scale */
    uint32_t shift = 0;
    uint64_t comp_shifted = compare;
    while (comp_shifted > METAL_RTCCMP0_MAX) {
        shift += 1;
        comp_shifted = comp_shifted >> shift;
    }

    /* Set the value of rtccfg.scale */
    uint32_t cfg = RTC_REGW(base, METAL_SIFIVE_RTC0_RTCCFG);
    cfg &= ~(METAL_RTCCFG_RTCSCALE_MASK);
    cfg |= (METAL_RTCCFG_RTCSCALE_MASK & shift);
    RTC_REGW(base, METAL_SIFIVE_RTC0_RTCCFG) = cfg;

    /* Set the value of rtccmp0 */
    RTC_REGW(base, METAL_SIFIVE_RTC0_RTCCMP0) = (uint32_t)comp_shifted;

    return metal_rtc_get_compare(rtc);
}

uint64_t metal_rtc_get_count(const struct metal_rtc *const rtc) {
    uintptr_t base = dt_rtc_data[get_inedx(rtc)].base_addr;

    uint64_t count = RTC_REGW(base, METAL_SIFIVE_RTC0_RTCCOUNTHI);
    count <<= 32;
    count |= RTC_REGW(base, METAL_SIFIVE_RTC0_RTCCOUNTLO);

    return count;
}

uint64_t metal_rtc_set_count(const struct metal_rtc *const rtc,
                             const uint64_t count) {
    uintptr_t base = dt_rtc_data[get_inedx(rtc)].base_addr;

    RTC_REGW(base, METAL_SIFIVE_RTC0_RTCCOUNTHI) = (UINT_MAX & (count >> 32));
    RTC_REGW(base, METAL_SIFIVE_RTC0_RTCCOUNTLO) = (UINT_MAX & count);

    return metal_rtc_get_count(rtc);
}

int metal_rtc_run(const struct metal_rtc *const rtc,
                  const enum metal_rtc_run_option option) {
    uintptr_t base = dt_rtc_data[get_inedx(rtc)].base_addr;

    switch (option) {
    default:
    case METAL_RTC_STOP:
        RTC_REGW(base, METAL_SIFIVE_RTC0_RTCCFG) &= ~(METAL_RTCCFG_ENALWAYS);
        break;
    case METAL_RTC_RUN:
        RTC_REGW(base, METAL_SIFIVE_RTC0_RTCCFG) |= METAL_RTCCFG_ENALWAYS;
        break;
    }

    return 0;
}

struct metal_interrupt *
metal_rtc_get_interrupt(const struct metal_rtc *const rtc) {
    return dt_rtc_data[get_index(rtc)].interrupt_parent;
}

int metal_rtc_get_interrupt_id(const struct metal_rtc *const rtc) {
    return dt_rtc_data[get_index(rtc)].interrupt_id;
}

#endif

typedef int no_empty_translation_units;
