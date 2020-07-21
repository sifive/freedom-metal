/* Copyright 2019 SiFive, Inc */
/* SPDX-License-Identifier: Apache-2.0 */

#ifndef METAL__PRIVILEGE_H
#define METAL__PRIVILEGE_H

/*!
 * @file metal/privilege.h
 *
 * @brief API for manipulating the privilege mode of a RISC-V system
 *
 * Additional information about privilege modes on RISC-V systems can be found
 * by reading the RISC-V Privileged Architecture Specification v1.10.
 */

#include <stdint.h>
#include <metal/riscv.h>

enum metal_privilege_mode {
    METAL_PRIVILEGE_USER = 0,
    METAL_PRIVILEGE_SUPERVISOR = 1,
    METAL_PRIVILEGE_MACHINE = 3,
};

struct metal_register_file {
    riscv_xlen_t ra;
    riscv_xlen_t sp;
    riscv_xlen_t gp;
    riscv_xlen_t tp;

    riscv_xlen_t t0;
    riscv_xlen_t t1;
    riscv_xlen_t t2;

    riscv_xlen_t s0;
    riscv_xlen_t s1;

    riscv_xlen_t a0;
    riscv_xlen_t a1;
    riscv_xlen_t a2;
    riscv_xlen_t a3;
    riscv_xlen_t a4;
    riscv_xlen_t a5;
#ifndef __riscv_32e
    riscv_xlen_t a6;
    riscv_xlen_t a7;

    riscv_xlen_t s2;
    riscv_xlen_t s3;
    riscv_xlen_t s4;
    riscv_xlen_t s5;
    riscv_xlen_t s6;
    riscv_xlen_t s7;
    riscv_xlen_t s8;
    riscv_xlen_t s9;
    riscv_xlen_t s10;
    riscv_xlen_t s11;

    riscv_xlen_t t3;
    riscv_xlen_t t4;
    riscv_xlen_t t5;
    riscv_xlen_t t6;
#endif /* __riscv_32e */

#ifdef __riscv_flen
    riscv_flen_t ft0;
    riscv_flen_t ft1;
    riscv_flen_t ft2;
    riscv_flen_t ft3;
    riscv_flen_t ft4;
    riscv_flen_t ft5;
    riscv_flen_t ft6;
    riscv_flen_t ft7;

    riscv_flen_t fs0;
    riscv_flen_t fs1;

    riscv_flen_t fa0;
    riscv_flen_t fa1;
    riscv_flen_t fa2;
    riscv_flen_t fa3;
    riscv_flen_t fa4;
    riscv_flen_t fa5;
    riscv_flen_t fa6;
    riscv_flen_t fa7;

    riscv_flen_t fs2;
    riscv_flen_t fs3;
    riscv_flen_t fs4;
    riscv_flen_t fs5;
    riscv_flen_t fs6;
    riscv_flen_t fs7;
    riscv_flen_t fs8;
    riscv_flen_t fs9;
    riscv_flen_t fs10;
    riscv_flen_t fs11;

    riscv_flen_t ft8;
    riscv_flen_t ft9;
    riscv_flen_t ft10;
    riscv_flen_t ft11;
#endif /* __riscv_flen */
};

typedef void (*metal_privilege_entry_point_t)(void);

void metal_privilege_drop_to_mode(enum metal_privilege_mode mode,
                                  struct metal_register_file regfile,
                                  metal_privilege_entry_point_t entry_point);

#endif
