/* Copyright 2018 SiFive, Inc */
/* SPDX-License-Identifier: Apache-2.0 */

#ifndef METAL__IO_H
#define METAL__IO_H

/* This macro enforces that the compiler will not elide the given access. */
#define __METAL_ACCESS_ONCE(x) (*(__typeof__(*x) volatile *)(x))

#define __METAL_GET_FIELD(reg, mask)                                           \
    (((reg) & (mask)) / ((mask) & ~((mask) << 1)))

/* Set field with mask for a given value */
#define __METAL_SET_FIELD(reg, mask, val)                                      \
    (((reg) & ~(mask)) | (((val) * ((mask) & ~((mask) << 1))) & (mask)))

#define __METAL_MIN(a, b) ((a) < (b) ? (a) : (b))
#define __METAL_MAX(a, b) ((a) > (b) ? (a) : (b))

/* Allows users to specify arbitrary fences. */
#define __METAL_IO_FENCE(pred, succ)                                           \
    __asm__ volatile("fence " #pred "," #succ ::: "memory");

/* Types that explicitly describe an address as being used for memory-mapped
 * IO.  These should only be accessed via __METAL_ACCESS_ONCE. */
typedef unsigned char __metal_io_u8;
typedef unsigned short __metal_io_u16;
typedef unsigned int __metal_io_u32;
#if __riscv_xlen >= 64
typedef unsigned long __metal_io_u64;
#endif

#endif
