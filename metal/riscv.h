/* Copyright (c) 2020 SiFive Inc. */
/* Copyright © 2020 Keith Packard */
/*
 * Licensed under the Apache License, Version 2.0 (the “License”); you
 * may not use this file except in compliance with the License. A copy
 * of this license can be found in the LICENSE.Apache2 file included
 * with this software. You may also obtain a copy of the License at
 *
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an “AS IS” BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or
 * implied. See the License for the specific language governing
 * permissions and limitations under the License.
 */
/* SPDX-License-Identifier: Apache-2.0 */

#ifndef __METAL_RISCV__H
#define __METAL_RISCV__H

#include <stdint.h>

#if __riscv_xlen == 32
typedef uint32_t riscv_xlen_t;
#else
typedef uint64_t riscv_xlen_t;
#endif

#if __riscv_flen == 32
typedef uint32_t riscv_flen_t;
#elif __riscv_flen == 64
typedef uint64_t riscv_flen_t;
#endif

#define RISCV_MTVEC_MODE_MASK 0x3
#define RISCV_MTVEC_MODE_DIRECT 0x0
#define RISCV_MTVEC_MODE_VECTORED 0x1
#define RISCV_MTVEC_MODE_CLIC_DIRECT 0x2
#define RISCV_MTVEC_MODE_CLIC_VECTORED 0x3

#if __riscv_xlen == 32
#define RISCV_MCAUSE_INTERRUPT 0x80000000UL
#define RISCV_MCAUSE_CODE_MASK 0x000003FFUL
#else
#define RISCV_MCAUSE_INTERRUPT 0x8000000000000000ULL
#define RISCV_MCAUSE_CODE_MASK 0x00000000000003FFULL
#endif
#define RISCV_MCAUSE_ID_STORE_AMO_ACCESS_FAULT 7

#define RISCV_MCAUSE_IS_INTERRUPT(mcause) ((mcause)&RISCV_MCAUSE_INTERRUPT)
#define RISCV_MCAUSE_IS_EXCEPTION(mcause) (!(RISCV_MCAUSE_IS_INTERRUPT(mcause))
#define RISCV_MCAUSE_ID(mcause) ((mcause)&RISCV_MCAUSE_CODE_MASK)

#define RISCV_MACHINE_SOFTWARE_INTERRUPT_ID 3
#define RISCV_MACHINE_TIMER_INTERRUPT_ID 7
#define RISCV_MACHINE_EXTERNAL_INTERRUPT_ID 11

#define RISCV_MSTATUS_UIE (1 << 0)
#define RISCV_MSTATUS_SIE (1 << 1)
#define RISCV_MSTATUS_MIE (1 << 3)
#define RISCV_MSTATUS_UPIE (1 << 4)
#define RISCV_MSTATUS_SPIE (1 << 5)
#define RISCV_MSTATUS_MPIE (1 << 7)
#define RISCV_MSTATUS_MPP_OFFSET 11
#define RISCV_MSTATUS_MPP_MASK (3 << RISCV_MSTATUS_MPP_OFFSET)

#define RISCV_MIE_MSIE (1 << 3)
#define RISCV_MIE_MTIE (1 << 7)
#define RISCV_MIE_MEIE (1 << 11)

#define RISCV_MIP_MSIP (1 << 3)
#define RISCV_MIP_MTIP (1 << 7)
#define RISCV_MIP_MEIP (1 << 11)

#define RISCV_MISA_A_EXTENSIONS 0x0001
#define RISCV_MISA_C_EXTENSIONS 0x0004
#define RISCV_MISA_D_EXTENSIONS 0x0008
#define RISCV_MISA_E_EXTENSIONS 0x0010
#define RISCV_MISA_F_EXTENSIONS 0x0020
#define RISCV_MISA_G_EXTENSIONS 0x0040
#define RISCV_MISA_I_EXTENSIONS 0x0100
#define RISCV_MISA_M_EXTENSIONS 0x1000
#define RISCV_MISA_N_EXTENSIONS 0x2000
#define RISCV_MISA_Q_EXTENSIONS 0x10000
#define RISCV_MISA_S_EXTENSIONS 0x40000
#define RISCV_MISA_U_EXTENSIONS 0x100000
#define RISCV_MISA_V_EXTENSIONS 0x200000
#define RISCV_MISA_XL32_EXTENSIONS 0x40000000UL
#define RISCV_MISA_XL64_EXTENSIONS 0x8000000000000000UL
#define RISCV_MISA_XL128_EXTENSIONS 0xC000000000000000UL

#define RISCV_INSTRUCTION_LENGTH_MASK 3
#define RISCV_INSTRUCTION_NOT_COMPRESSED 3

#endif /* __METAL_RISCV__H */
