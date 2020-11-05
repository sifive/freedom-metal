/* Copyright 2020 SiFive, Inc */
/* SPDX-License-Identifier: Apache-2.0 */

#include <metal/hca.h>

uint32_t metal_hca_getrev(struct metal_hca hca)
    __attribute__((weak));
uint32_t metal_hca_getrev(struct metal_hca hca)
{ return 0; }
