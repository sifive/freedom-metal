/* Copyright 2018 SiFive, Inc */
/* SPDX-License-Identifier: Apache-2.0 */

#ifndef METAL__CACHE_H
#define METAL__CACHE_H

/*!
 * @file cache.h
 *
 * @brief API for configuring caches
 */
#include <stdbool.h>
#include <stdint.h>

/*!
 * @brief Check if dcache is supported on the core
 * @return 1 if dcache is present
 */
bool metal_dcache_l1_available(void);

/*!
 * @brief Flush dcache for L1 on the requested core with write back
 * @param address The virtual address of cacheline to invalidate
 * @return None
 */
void metal_dcache_l1_flush(uintptr_t address);

/*!
 * @brief Discard dcache for L1 on the requested core with no write back
 * @param address The virtual address of cacheline to invalidate
 * @return None
 */
void metal_dcache_l1_discard(uintptr_t address);

/*!
 * @brief Check if icache is supported on the core
 * @return 1 if icache is present
 */
bool metal_icache_l1_available(void);

#endif
