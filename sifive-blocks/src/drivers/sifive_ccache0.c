/* Copyright 2019 SiFive, Inc */
/* SPDX-License-Identifier: Apache-2.0 */

#include <metal/platform.h>

#ifdef METAL_SIFIVE_CCACHE0

#include <assert.h>
#include <metal/drivers/sifive_ccache0.h>
#include <metal/init.h>
#include <metal/io.h>
#include <stddef.h>

#define L2_CONFIG_WAYS_SHIFT 8
#define L2_CONFIG_WAYS_MASK (0xFF << L2_CONFIG_WAYS_SHIFT)

#define L2_REGW(offset)                                                        \
    __METAL_ACCESS_ONCE(                                                       \
        (__metal_io_u32 *)(METAL_SIFIVE_CCACHE0_0_BASE_ADDRESS + (offset)))
#define L2_REGD(offset)                                                        \
    __METAL_ACCESS_ONCE(                                                       \
        (volatile uint64_t *)(METAL_SIFIVE_CCACHE0_0_BASE_ADDRESS + (offset)))

/* Linker symbols to calculate LIM allocated size */
extern char metal_segment_lim_target_start, metal_segment_lim_target_end;

void sifive_ccache0_init(uint32_t ways) {
    sifive_ccache0_set_enabled_ways(ways);
}

METAL_CONSTRUCTOR(init_sifive_ccache0) {
    size_t lim_size = 0;
    if (&metal_segment_lim_target_end > &metal_segment_lim_target_start) {
        lim_size =
            &metal_segment_lim_target_end - &metal_segment_lim_target_start;
    }

    const uint32_t available_ways = __METAL_GET_FIELD(
        L2_REGW(METAL_SIFIVE_CCACHE0_CONFIG), L2_CONFIG_WAYS_MASK);

    const size_t way_size = METAL_SIFIVE_CCACHE0_0_CACHE_BLOCK_SIZE *
                            METAL_SIFIVE_CCACHE0_0_CACHE_SETS *
                            METAL_SIFIVE_CCACHE0_0_BANK_COUNT;

    uint32_t ways_to_disable = 0;
    if (way_size != 0) {
        /* Divide lim_size by way_size, rounding up */
        ways_to_disable = (lim_size + way_size - 1) / way_size;
    }

    sifive_ccache0_init(available_ways - ways_to_disable);
}

uint32_t sifive_ccache0_get_enabled_ways(void) {
    const uint32_t way_enable = L2_REGW(METAL_SIFIVE_CCACHE0_WAYENABLE);

    /* The stored number is the index, so add one */
    return (0xFF & way_enable) + 1;
}

uint32_t sifive_ccache0_set_enabled_ways(uint32_t ways) {
    /* We can't decrease the number of enabled ways */
    if (sifive_ccache0_get_enabled_ways() > ways) {
        return sifive_ccache0_get_enabled_ways();
    }

    /* The stored value is the index, so subtract one */
    const uint32_t value = 0xFF & (ways - 1);

    /* Set the number of enabled ways */
    L2_REGW(METAL_SIFIVE_CCACHE0_WAYENABLE) = value;

    return sifive_ccache0_get_enabled_ways();
}

void sifive_ccache0_inject_ecc_error(uint32_t bitindex,
                                     sifive_ccache0_ecc_errtype_t type) {
    /* Induce ECC error at given bit index and location */
    L2_REGW(METAL_SIFIVE_CCACHE0_ECCINJECTERROR) =
        (uint32_t)(((type & 0x01) << 16) | (bitindex & 0xFF));
}

void sifive_ccache0_flush(uintptr_t flush_addr) {
    /* Block memory access until operation completed */
    __asm volatile("fence rw, io" : : : "memory");

#if __riscv_xlen == 32
    L2_REGW(METAL_SIFIVE_CCACHE0_FLUSH32) = flush_addr >> REG_SHIFT_4;
#else
    L2_REGD(METAL_SIFIVE_CCACHE0_FLUSH64) = flush_addr;
#endif

    __asm volatile("fence io, rw" : : : "memory");
}

uintptr_t sifive_ccache0_get_ecc_fix_addr(sifive_ccache0_ecc_errtype_t type) {
    uintptr_t addr = 0;

    switch (type) {
        /* Get most recently ECC corrected address */
    case SIFIVE_CCACHE0_DATA:
        addr = (uintptr_t)L2_REGD(METAL_SIFIVE_CCACHE0_DATECCFIXLOW);
        break;

    case SIFIVE_CCACHE0_DIR:
        addr = (uintptr_t)L2_REGD(METAL_SIFIVE_CCACHE0_DIRECCFIXLOW);
        break;
    }

    return addr;
}

uint32_t sifive_ccache0_get_ecc_fix_count(sifive_ccache0_ecc_errtype_t type) {
    uint32_t count = 0;

    switch (type) {
        /* Get number of times ECC errors were corrected */
    case SIFIVE_CCACHE0_DATA:
        count = L2_REGW(METAL_SIFIVE_CCACHE0_DATECCFIXCOUNT);
        break;

    case SIFIVE_CCACHE0_DIR:
        count = L2_REGW(METAL_SIFIVE_CCACHE0_DIRECCFIXCOUNT);
        break;
    }

    return count;
}

uintptr_t sifive_ccache0_get_ecc_fail_addr(sifive_ccache0_ecc_errtype_t type) {
    uintptr_t addr = 0;

    switch (type) {
        /*  Get address location of most recent uncorrected ECC error */
    case SIFIVE_CCACHE0_DATA:
        addr = (uintptr_t)L2_REGD(METAL_SIFIVE_CCACHE0_DATECCFAILLOW);
        break;

    case SIFIVE_CCACHE0_DIR:
        addr = (uintptr_t)L2_REGD(METAL_SIFIVE_CCACHE0_DIRECCFAILLOW);
        break;
    }

    return addr;
}

uint32_t sifive_ccache0_get_ecc_fail_count(sifive_ccache0_ecc_errtype_t type) {
    uint32_t count = 0;

    switch (type) {
        /* Get number of times ECC errors were not corrected */
    case SIFIVE_CCACHE0_DATA:
        count = L2_REGW(METAL_SIFIVE_CCACHE0_DATECCFAILCOUNT);
        break;

    case SIFIVE_CCACHE0_DIR:
        count = L2_REGW(METAL_SIFIVE_CCACHE0_DIRECCFAILCOUNT);
        break;
    }

    return count;
}

uint64_t sifive_ccache0_get_way_mask(uint32_t master_id) {
    uint64_t val = 0;

    /* Get way mask for given master ID */
    val = L2_REGD(METAL_SIFIVE_CCACHE0_WAYMASK0 + master_id * 8);

    return val;
}

int sifive_ccache0_set_way_mask(uint32_t master_id, uint64_t waymask) {

    /* Set way mask for given master ID */
    L2_REGD(METAL_SIFIVE_CCACHE0_WAYMASK0 + master_id * 8) = waymask;

    return 0;
}

void sifive_ccache0_set_pmevent_selector(uint32_t counter, uint64_t mask) {

#if METAL_SIFIVE_CCACHE0_0_PERFMON_COUNTERS > 0
    if (counter < METAL_SIFIVE_CCACHE0_0_PERFMON_COUNTERS) {

        /* Set event selector for specified L2 event counter */
        L2_REGD(METAL_SIFIVE_CCACHE0_PMEVENTSELECT0 + counter * 8) = mask;
    }
#endif
    return;
}

uint64_t sifive_ccache0_get_pmevent_selector(uint32_t counter) {
    uint64_t val = 0;

#if METAL_SIFIVE_CCACHE0_0_PERFMON_COUNTERS > 0
    if (counter < METAL_SIFIVE_CCACHE0_0_PERFMON_COUNTERS) {

        /* Get event selector for specified L2 event counter */
        val = L2_REGD(METAL_SIFIVE_CCACHE0_PMEVENTSELECT0 + counter * 8);
    }
#endif
    return val;
}

void sifive_ccache0_clr_pmevent_counter(uint32_t counter) {

#if METAL_SIFIVE_CCACHE0_0_PERFMON_COUNTERS > 0
    if (counter < METAL_SIFIVE_CCACHE0_0_PERFMON_COUNTERS) {
        /* Clear specified L2 event counter */
        L2_REGD(METAL_SIFIVE_CCACHE0_PMEVENTCOUNTER0 + counter * 8) = 0;
    }
#endif
    return;
}

uint64_t sifive_ccache0_get_pmevent_counter(uint32_t counter) {
#if __riscv_xlen == 32
    uint32_t vh = 0, vh1 = 0, vl = 0;
#else
    uint64_t val = 0;
#endif
#if METAL_SIFIVE_CCACHE0_0_PERFMON_COUNTERS > 0
    if (counter < METAL_SIFIVE_CCACHE0_0_PERFMON_COUNTERS) {
        /* Set counter register offset */
        counter *= 8;

#if __riscv_xlen == 32
        do {
            vh = L2_REGW(METAL_SIFIVE_CCACHE0_PMEVENTCOUNTER0 + counter + 4);
            vl = L2_REGW(METAL_SIFIVE_CCACHE0_PMEVENTCOUNTER0 + counter);
            vh1 = L2_REGW(METAL_SIFIVE_CCACHE0_PMEVENTCOUNTER0 + counter + 4);
        } while (vh != vh1);
#else
        val = L2_REGD(METAL_SIFIVE_CCACHE0_PMEVENTCOUNTER0 + counter);
#endif
    }
#endif
#if __riscv_xlen == 32
    return ((((unsigned long long)vh) << 32) | vl);
#else
    return val;
#endif
}

void sifive_ccache0_set_client_filter(uint64_t mask) {

    /* Set clients to be excluded from performance monitoring */
    L2_REGD(METAL_SIFIVE_CCACHE0_PMCLIENTFILTER) = mask;
}

uint64_t sifive_ccache0_get_client_filter(void) {
    uint64_t val = 0;

    /* Get currently active client filter mask */
    val = L2_REGD(METAL_SIFIVE_CCACHE0_PMCLIENTFILTER);

    return val;
}

#endif

typedef int no_empty_translation_units;
