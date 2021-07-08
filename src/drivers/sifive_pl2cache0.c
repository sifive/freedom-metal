/* Copyright 2021 SiFive, Inc */
/* SPDX-License-Identifier: Apache-2.0 */

#include <metal/machine/platform.h>

#ifdef METAL_SIFIVE_PL2CACHE0

#include <metal/drivers/sifive_pl2cache0.h>
#include <metal/init.h>
#include <metal/machine.h>
#include <stdint.h>

/* Macros to access memory mapped registers */
#define REGW(_x_) *(volatile uint32_t *)(pl2cache_base[hartid] + (_x_))

#define REGD(_x_) *(volatile uint64_t *)(pl2cache_base[hartid] + (_x_))

/* Macros to specify register bit shift */
#define REG_SHIFT_4 4
#define REG_SHIFT_8 8
#define REG_SHIFT_16 16
#define REG_SHIFT_24 24

#define SIFIVE_PL2CACHE0_BYTE_MASK 0xFFUL

/* Array of base addresses with HART IDs as the index */
unsigned long pl2cache_base[] = METAL_SIFIVE_PL2CACHE0_BASE_ADDR;

void sifive_pl2cache0_set_cleanEvictenale_bit(bool val) {
    sifive_pl2cache0_configbits tmp;
    int hartid;
    __asm__ volatile("csrr %0, mhartid" : "=r"(hartid));

    tmp.w = REGW(METAL_SIFIVE_PL2CACHE0_CONFIGBITS);
    tmp.b.cleanEvictEnable = val;
    REGW(METAL_SIFIVE_PL2CACHE0_CONFIGBITS) = tmp.w;
}

void sifive_pl2cache0_init(void) {
#ifndef METAL_SIFIVE_CCACHE0
    // (L3 CACHE) CCACHE0 is not present
    sifive_pl2cache0_set_cleanEvictenale_bit(0);
#else
    sifive_pl2cache0_set_cleanEvictenale_bit(1);
#endif /* METAL_SIFIVE_CCACHE0 */
}

void sifive_pl2cache0_get_config(sifive_pl2cache0_config *config) {
    if (config) /* Check for NULL */
    {
        uint32_t val;
        int hartid;
        __asm__ volatile("csrr %0, mhartid" : "=r"(hartid));

        val = REGW(METAL_SIFIVE_PL2CACHE0_CONFIG);

        /* Populate cache configuration data */
        config->num_bank = (val & SIFIVE_PL2CACHE0_BYTE_MASK);
        config->num_ways = ((val >> REG_SHIFT_8) & SIFIVE_PL2CACHE0_BYTE_MASK);
        /* no. of sets, block size is 2's power of register value
        (2 << (value-1)) */
        config->num_sets =
            2 << (((val >> REG_SHIFT_16) & SIFIVE_PL2CACHE0_BYTE_MASK) - 1);
        config->block_size =
            2 << (((val >> REG_SHIFT_24) & SIFIVE_PL2CACHE0_BYTE_MASK) - 1);
    }
}

uint32_t sifive_pl2cache0_get_enabled_ways(void) {
    sifive_pl2cache0_config config = {0, 0, 0, 0};
    sifive_pl2cache0_get_config(&config);
    return config.num_ways;
}

int sifive_pl2cache0_set_enabled_ways(uint32_t ways) {
    sifive_pl2cache0_config config = {0, 0, 0, 0};
    sifive_pl2cache0_get_config(&config);
    return ways <= config.num_ways ? 0 : -1;
}

void sifive_pl2cache0_inject_ecc_error(uint32_t bitindex,
                                       sifive_pl2cache0_ecc_errtarget_t target,
                                       sifive_pl2cache0_ecc_errdirection_t dir,
                                       sifive_pl2cache0_ecc_errtype_t type) {
    int hartid;
    __asm__ volatile("csrr %0, mhartid" : "=r"(hartid));

    /* Induce ECC error at given bit index and location */
    REGW(METAL_SIFIVE_PL2CACHE0_ECCINJECTERROR) =
        (uint32_t)(((target & 0x03) << (REG_SHIFT_16 + 0)) |
                   ((dir & 0x01) << (REG_SHIFT_16 + 2u)) |
                   ((type & 0x01) << (REG_SHIFT_16 + 3u)) | (bitindex & 0xFF));
}

void sifive_pl2cache0_flush(uintptr_t flush_addr) {
    int hartid;
    __asm__ volatile("csrr %0, mhartid" : "=r"(hartid));

    /* Block memory access until operation completed */
    __asm volatile("fence rw, io" : : : "memory");

#if __riscv_xlen == 32
    REGW(METAL_SIFIVE_PL2CACHE0_CFLUSH64) = flush_addr;
    REGW(METAL_SIFIVE_PL2CACHE0_CFLUSH64 + sizeof(uint32_t)) = 0x13u << 24u;
#else
    REGD(METAL_SIFIVE_PL2CACHE0_CFLUSH64) = flush_addr | (0x13ull << 24u);
#endif

    __asm volatile("fence io, rw" : : : "memory");
}

uint32_t sifive_pl2cache0_get_flush_count(void) {
    int hartid;
    __asm__ volatile("csrr %0, mhartid" : "=r"(hartid));

    return REGW(METAL_SIFIVE_PL2CACHE0_FLUSHCOUNT) & 0xFFu;
}

void sifive_pl2cache0_set_pmevent_selector(uint32_t counter, uint64_t mask) {
#if METAL_SIFIVE_PL2CACHE0_PERFMON_COUNTERS > 0
    if (counter < METAL_SIFIVE_PL2CACHE0_PERFMON_COUNTERS) {
        int hartid;
        __asm__ volatile("csrr %0, mhartid" : "=r"(hartid));

        /* Set event selector for specified L2 event counter */
        REGD(METAL_SIFIVE_PL2CACHE0_PMEVENTSELECT0 + counter * 8) = mask;
    }
#endif
    return;
}

uint64_t sifive_pl2cache0_get_pmevent_selector(uint32_t counter) {
    uint64_t val = 0;

#if METAL_SIFIVE_PL2CACHE0_PERFMON_COUNTERS > 0
    if (counter < METAL_SIFIVE_PL2CACHE0_PERFMON_COUNTERS) {
        int hartid;
        __asm__ volatile("csrr %0, mhartid" : "=r"(hartid));

        /* Get event selector for specified L2 event counter */
        val = REGD(METAL_SIFIVE_PL2CACHE0_PMEVENTSELECT0 + counter * 8);
    }
#endif
    return val;
}

void sifive_pl2cache0_clr_pmevent_counter(uint32_t counter) {

#if METAL_SIFIVE_PL2CACHE0_PERFMON_COUNTERS > 0
    if (counter < METAL_SIFIVE_PL2CACHE0_PERFMON_COUNTERS) {
        int hartid;
        __asm__ volatile("csrr %0, mhartid" : "=r"(hartid));

        /* Clear specified L2 event counter */
        REGD(METAL_SIFIVE_PL2CACHE0_PMEVENTCOUNTER0 + counter * 8) = 0;
    }
#endif
    return;
}

uint64_t sifive_pl2cache0_get_pmevent_counter(uint32_t counter) {
#if __riscv_xlen == 32
    uint32_t vh = 0, vh1 = 0, vl = 0;
#else
    uint64_t val = 0;
#endif
#if METAL_SIFIVE_PL2CACHE0_PERFMON_COUNTERS > 0
    if (counter < METAL_SIFIVE_PL2CACHE0_PERFMON_COUNTERS) {
        int hartid;
        __asm__ volatile("csrr %0, mhartid" : "=r"(hartid));

        /* Set counter register offset */
        counter *= 8;

#if __riscv_xlen == 32
        do {
            vh = REGW(METAL_SIFIVE_PL2CACHE0_PMEVENTCOUNTER0 + counter + 4);
            vl = REGW(METAL_SIFIVE_PL2CACHE0_PMEVENTCOUNTER0 + counter);
            vh1 = REGW(METAL_SIFIVE_PL2CACHE0_PMEVENTCOUNTER0 + counter + 4);
        } while (vh != vh1);
#else
        val = REGD(METAL_SIFIVE_PL2CACHE0_PMEVENTCOUNTER0 + counter);
#endif
    }
#endif
#if __riscv_xlen == 32
    return ((((unsigned long long)vh) << 32) | vl);
#else
    return val;
#endif
}

void sifive_pl2cache0_set_client_filter(uint64_t mask) {
    int hartid;
    __asm__ volatile("csrr %0, mhartid" : "=r"(hartid));

    /* Set clients to be excluded from performance monitoring */
    REGD(METAL_SIFIVE_PL2CACHE0_PMCLIENTFILTER) = mask;
}

uint64_t sifive_pl2cache0_get_client_filter(void) {
    uint64_t val = 0;
    int hartid;
    __asm__ volatile("csrr %0, mhartid" : "=r"(hartid));

    /* Get currently active client filter mask */
    val = REGD(METAL_SIFIVE_PL2CACHE0_PMCLIENTFILTER);

    return val;
}

#endif

typedef int no_empty_translation_units;
