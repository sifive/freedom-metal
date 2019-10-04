/* Copyright 2019 SiFive, Inc */
/* SPDX-License-Identifier: Apache-2.0 */

#ifndef METAL__CSR_H
#define METAL__CSR_H

#include <metal/compiler.h>
#include <stddef.h>
#include <stdint.h>

/*!
 * @file csr.h
 * @brief A collection of APIs for get and set CSR registers
 */

/*!
 * @brief Read a given CSR register
 * @param crs register offset to read
 * @return value of the register read
 */
__inline__ int metal_csr_read(int csr, uintptr_t *value) {
    uintptr_t m;

    if (value == NULL) {
        return -1;
    }
    switch (csr) {
    // Standard User R/W
    case 0x00:
        __asm__ volatile("csrr %0, ustatus" : "=r"(m));
        break;
    case 0x01:
        __asm__ volatile("csrr %0, fflags" : "=r"(m));
        break;
    case 0x02:
        __asm__ volatile("csrr %0, frm" : "=r"(m));
        break;
    case 0x03:
        __asm__ volatile("csrr %0, fcsr" : "=r"(m));
        break;

    // Standard User RO
    case 0xC00:
        __asm__ volatile("csrr %0, cycle" : "=r"(m));
        break;
    case 0xC01:
        __asm__ volatile("csrr %0, time" : "=r"(m));
        break;
    case 0xC002:
        __asm__ volatile("csrr %0, instret" : "=r"(m));
        break;

    // Standard Supervisor R/W
    case 0x100:
        __asm__ volatile("csrr %0, sstatus" : "=r"(m));
        break;
    case 0x104:
        __asm__ volatile("csrr %0, sie" : "=r"(m));
        break;
    case 0x105:
        __asm__ volatile("csrr %0, stvec" : "=r"(m));
        break;

    // Tentative assignement for CLIC
    case 0x07:
        __asm__ volatile("csrr %0, 0x07" : "=r"(m));
        break;

    // Standard Machine R/W
    case 0x300:
        __asm__ volatile("csrr %0, mstatus" : "=r"(m));
        break;

    // Standard Machine RO
    case 0xF11:
        __asm__ volatile("csrr %0, mvendorid" : "=r"(m));
        break;

    default:
        return -1;
    }
    *value = m;
    return 0;
}

/*!
 * @brief Read a given CSR register
 * @param crs register offset to read
 * @return value of the register read
 */
__inline__ int metal_csr_write(int csr, uintptr_t value) {
    switch (csr) {
    // Standard User R/W
    case 0x00:
        __asm__ volatile("csrw ustatus, %0" : : "r"(value));
        break;
    case 0x01:
        __asm__ volatile("csrw fflags, %0" : : "r"(value));
        break;
    case 0x02:
        __asm__ volatile("csrw frm, %0" : : "r"(value));
        break;
    case 0x03:
        __asm__ volatile("csrw fcsr, %0" : : "r"(value));
        break;

    // Standard Supervisor R/W
    case 0x100:
        __asm__ volatile("csrw sstatus, %0" : : "r"(value));
        break;
    case 0x104:
        __asm__ volatile("csrw sie, %0" : : "r"(value));
        break;
    case 0x105:
        __asm__ volatile("csrw stvec, %0" : : "r"(value));
        break;

    // Tentative assignement for CLIC
    case 0x07:
        __asm__ volatile("csrw 0x07, %0" : : "r"(value));
        break;

    // Standard Machine R/W
    case 0x300:
        __asm__ volatile("csrw mstatus, %0" : : "r"(value));
        break;

    default:
        return -1;
    }
    return 0;
}

/*!
 * @brief Read a given CSR register without checking validity of CSR offset
 * @param crs Register label or hex value offset to read from
 * @param value Variable name of uintprt_t type to get the value
 */
#define METAL_CPU_GET_CSR(reg, value)                                          \
    __asm__ volatile("csrr %0, " #reg : "=r"(value));

/*!
 * @brief Write to a given CSR register without checking validity of CSR offset
 * @param crs Register label or hex value offset to write to
 * @param value Variable name of uintprt_t type to set the value
 */
#define METAL_CPU_SET_CSR(reg, value)                                          \
    __asm__ volatile("csrw " #reg ", %0" : : "r"(value));

#endif // METAL__CSR_H
