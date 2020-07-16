/* Copyright (c) 2020 SiFive Inc. */
/* SPDX-License-Identifier: Apache-2.0 */

#include <stdint.h>

#if __riscv_xlen == 32
typedef uint32_t riscv_xlen_t;
#else
typedef uint64_t riscv_xlen_t;
#endif

#if __riscv_xlen == 32
#define RISCV_MCAUSE_INTERRUPT 0x80000000UL
#define RISCV_MCAUSE_CODE_MASK 0x000003FFUL
#else
#define RISCV_MCAUSE_INTERRUPT 0x8000000000000000ULL
#define RISCV_MCAUSE_CODE_MASK 0x00000000000003FFULL
#endif

#define RISCV_MCAUSE_IS_INTERRUPT(mcause) ((mcause)&RISCV_MCAUSE_INTERRUPT)
#define RISCV_MCAUSE_IS_EXCEPTION(mcause) (!(RISCV_MCAUSE_IS_INTERRUPT(mcause))
#define RISCV_MCAUSE_ID(mcause) ((mcause)&RISCV_MCAUSE_CODE_MASK)
