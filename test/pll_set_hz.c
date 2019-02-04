/* Copyright 2018 SiFive, Inc */
/* SPDX-License-Identifier: Apache-2.0 */

#include <stdio.h>
#include <metal/clock.h>

#define PLL_MIN_OUT 16000000
#define PLL_MAX_OUT 320000000

void main() {
#ifdef __METAL_DT_SIFIVE_FE310_G000_PLL_HANDLE
    struct metal_clock * pll = __ME_DT_SIFIVE_FE310_G000_PLL_HANDLE;

    printf("Test harness for FE310-G000 PLL Set Frequency\n\n");
    printf("Boot frequency is: %ld\n", metal_clock_get_rate_hz(pll));

    for(long requested = PLL_MIN_OUT; requested <= PLL_MAX_OUT; requested += 16000000)
    {
        /* fflush before we change clock rates */
        fflush(stdout);

        long actual = metal_clock_set_rate_hz(pll, requested);

        printf("Requested %9ld Hz, Actual %9ld Hz\n", requested, actual);
    }

    printf("--- Complete ---\n");
#else
    printf("Unable to run fe310-g000 PLL test case as there is no PLL\n");
#endif
}
