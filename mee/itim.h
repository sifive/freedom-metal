/* Copyright 2018 SiFive, Inc */
/* SPDX-License-Identifier: Apache-2.0 */

#ifndef MEE__ITIM_H
#define MEE__ITIM_H

/*! @file itim.h
 *
 * API for manipulating ITIM allocation
 */


/*! @def MEE_PLACE_IN_ITIM
 * @brief Link a function into the ITIM
 *
 * Link a function into the ITIM (Instruction Tightly Integrated
 * Memory) if the ITIM is present on the target device.
 */
#define MEE_PLACE_IN_ITIM	__attribute__((section(".itim")))

#endif
