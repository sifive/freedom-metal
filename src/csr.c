/* Copyright 2019 SiFive, Inc */
/* SPDX-License-Identifier: Apache-2.0 */

#include <metal/csr.h>

extern __inline__ int metal_csr_read(int csr, uintptr_t *value);
extern __inline__ int metal_csr_write(int csr, uintptr_t value);
