/* Copyright 2021 SiFive, Inc */
/* SPDX-License-Identifier: Apache-2.0 */

#ifndef METAL__DRIVERS__SIFIVE_L2PF1_H
#define METAL__DRIVERS__SIFIVE_L2PF1_H

/*!
 * @file sifive_l2pf1.h
 *
 * @brief API for configuring the SiFive L2 stride prefetcher.
 */

#include <stdint.h>

/*! @brief L2 stride prefetcher configuration. */
typedef struct {
    /* Enable hardware prefetcher support for scalar Loads. */
    uint8_t ScalarLoadSupportEn;

    /* Initial prefetch distance. */
    uint8_t Dist;

    /* Maximum allowed prefetch Distance. */
    uint32_t MaxAllowedDist;

    /*
     * Threshold when SPF distance increase changes
     * from linear (+1) to exponential (*2).
     */
    uint8_t LinToExpThrd;

    /* Enable prefetches to cross pages. */
    uint8_t CrossPageEn;

    /*
     * Threshold for forgiving loads with
     * mismatching strides when SPF is in trained state.
     */
    uint8_t ForgiveThrd;

    /*
     * Threshold no. of Fullness (L2 MSHRs used/ total available) to stop
     * sending hits.
     */
    uint8_t QFullnessThrd;

    /* No. of CacheHits for evicting SPF entry. */
    uint8_t HitCacheThrd;

    /* Threshold no. of MSHR hits for increasing SPF distance. */
    uint8_t HitMSHRThrd;

    /* Size of the comparison window for address matching. */
    uint8_t Window;

    /* Prefetch-enable for Scalar Stores. */
    uint8_t ScalarStoreSupportEn;

    /* Prefetch-enable for Vector Loads (Gets). */
    uint8_t VectorLoadSupportEn;

    /* Prefetch-enable for Vector Stores (Puts). */
    uint8_t VectorStoreSupportEn;
} sifive_l2pf1_config;

/*
 * ! @brief Disable L2 hardware stride prefetcher unit.
 * @param None.
 * @return None.
 */
void sifive_l2pf1_disable(void);

/*
 * ! @brief Get currently active L2 stride prefetcher configuration.
 * @param config Pointer to user specified configuration structure.
 * @return None.
 */
void sifive_l2pf1_get_config(sifive_l2pf1_config *config);

/*
 * ! @brief Enables fine grain access to L2 stride prefetcher configuration.
 * @param config Pointer to user structure with values to be set.
 * @return None.
 */
void sifive_l2pf1_set_config(sifive_l2pf1_config *config);

/*
 * ! @brief Initialize L2 hardware stride prefetcher unit.
 * @param None.
 * @return None.
 */
void sifive_l2pf1_init(void);

#endif
