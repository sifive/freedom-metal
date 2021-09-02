/* Copyright 2020 SiFive, Inc */
/* SPDX-License-Identifier: Apache-2.0 */

#ifndef METAL__DRIVERS__SIFIVE_L2PF0_H
#define METAL__DRIVERS__SIFIVE_L2PF0_H

/*!
 * @file sifive_l2pf0.h
 *
 * @brief API for configuring the SiFive L2 prefetcher.
 */

#include <stdint.h>

/**
 * Structure type Basic Control Register
 */
typedef union _L2PF0_BCR {
    struct {
        uint32_t
            ScalaLoadSupportEn : 1; /**< bit: 0 - Enable hardware prefetcher */
        uint32_t CrossPageOptmDis : 1; /**< bit: 1 - Only works when CrossPageEn
                                          ===0  */
        uint32_t Dist : 6; /**< bit: 2..7 - Initial prefetch distance */
        uint32_t MaxAllowedDist : 6; /**< bit: 8..13 - Maximum allowed Distance:
                                        1<< maxAllowedDistance */
        uint32_t LinToExpThrd : 6;   /**< bit: 14..19 - Linear to exponential
                                        threshold */
        uint32_t AgeOutEn : 1;       /**< bit: 20 - Ageout mechanism enable */
        uint32_t NumLdsToAgeOut : 7; /**< bit: 21..27 - Num of non-matching
                                        loads to edge out an entry */
        uint32_t
            CrossPageEn : 1; /**< bit:  28 - Enable prefetches to cross pages */
        uint32_t _reserved0 : 3; /**< (reserved) */
    } b;                         /**< Structure used for bit access */
    uint32_t w;                  /**< Structure used for word access */
} sifive_l2pf0_bcr;

/* L2PF0 Basic Control Register*/
#define L2PF0_BCR_SCALALOADSUPPORTEN_Pos 0U
#define L2PF0_BCR_SCALALOADSUPPORTEN_Msk                                       \
    (1U << L2PF0_BCR_SCALALOADSUPPORTEN_Pos)

#define L2PF0_BCR_CROSSPAGEOPTMDIS_Pos 1U
#define L2PF0_BCR_CROSSPAGEOPTMDIS_Msk (1U << L2PF0_BCR_CROSSPAGEOPTMDIS_Pos)

#define L2PF0_BCR_DIST_Pos 2U
#define L2PF0_BCR_DIST_Msk (63U << L2PF0_BCR_DIST_Pos)

#define L2PF0_BCR_MAXALLOWEDDIST_Pos 8U
#define L2PF0_BCR_MAXALLOWEDDIST_Msk (63U << L2PF0_BCR_MAXALLOWEDDIST_Pos)

#define L2PF0_BCR_LINTOEXPTHRD_Pos 14U
#define L2PF0_BCR_LINTOEXPTHRD_Msk (63U << L2PF0_BCR_LINTOEXPTHRD_Pos)

#define L2PF0_BCR_AGEOUTEN_Pos 20U
#define L2PF0_BCR_AGEOUTEN_Msk (1U << L2PF0_BCR_AGEOUTEN_Pos)

#define L2PF0_BCR_NUMLDSTOAGEOUT_Pos 21U
#define L2PF0_BCR_NUMLDSTOAGEOUT_Msk (127U << L2PF0_BCR_NUMLDSTOAGEOUT_Pos)

#define L2PF0_BCR_CROSSPAGEEN_Pos 28U
#define L2PF0_BCR_CROSSPAGEEN_Msk (1U << L2PF0_BCR_CROSSPAGEEN_Pos)

/**
 * Structure type L2 User Bits Control Register
 */
typedef union _L2PF0_L2UBCR {
    struct {
        uint32_t QFullnessThrd : 4; /**< bit: 0..3 - Threshold # of Fullness (L2
                                       MSHRs used/ total available) to stop
                                       sending hits */
        uint32_t HitCacheThrd : 5;  /**< bit 4..8 - (1 << hitCacheThrd) of
                                       CacheHits for evicting SPF entry */
        uint32_t HitMSHRThrd : 4; /**> bit 9..12 - Threshold # of MSHR hits for
                                     increasing SPF distance */
        uint32_t Window : 6; /**< bit: 13..18 - Size of the comparison window
                                for address matching */
    } b;                     /**< Structure used for bit access */
    uint32_t w;              /**< Structure used for word access */
} sifive_l2pf0_l2ubcr;

/* L2 User Bits Control Register*/
#define L2PF0_L2UBCR_QFULLNESTHRD_Pos 0U
#define L2PF0_L2UBCR_QFULLNESSTHRD_Msk (15U << L2PF0_L2UBCR_QFULLNESTHRD_Pos)

#define L2PF0_L2UBCR_HITCACHETHRD_Pos 4U
#define L2PF0_L2UBCR_HITCACHETHRD_Msk (31U << L2PF0_L2UBCR_HITCACHETHRD_Pos)

#define L2PF0_L2UBCR_HITMSHRTHRD_Pos 9U
#define L2PF0_L2UBCR_HITMSHRTHRD_Msk (15U << L2PF0_L2UBCR_HITMSHRTHRD_Pos)

#define L2PF0_L2UBCR_WINDOW_Pos 13U
#define L2PF0_L2UBCR_WINDOW_Msk (63U << L2PF0_L2UBCR_WINDOW_Pos)

/*! @brief L2 prefetcher configuration */
typedef struct {
    /* Basic Control Register */
    sifive_l2pf0_bcr bcr;

    /* L2 user Bits Control Register */
    sifive_l2pf0_l2ubcr l2ubcr;

} sifive_l2pf0_config;

/*! @brief Enable L2 hardware prefetcher unit.
 * @param None.
 * @return None.*/
void sifive_l2pf0_enable(void);

/*! @brief Disable L2 hardware prefetcher unit.
 * @param None.
 * @return None.*/
void sifive_l2pf0_disable(void);

/*! @brief Get currently active L2 prefetcher configuration.
 * @param config Pointer to user specified configuration structure.
 * @return None.*/
void sifive_l2pf0_get_config(sifive_l2pf0_config *config);

/*! @brief Enables fine grain access to L2 prefetcher configuration.
 * @param config Pointer to user structure with values to be set.
 * @return None.*/
void sifive_l2pf0_set_config(sifive_l2pf0_config *config);

#endif
