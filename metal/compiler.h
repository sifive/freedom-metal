/* Copyright 2018 SiFive, Inc */
/* SPDX-License-Identifier: Apache-2.0 */

#ifndef MEE__COMPILER_H
#define MEE__COMPILER_H

#define __MEE_DECLARE_VTABLE(type)                        \
    asm(".weak " #type);                                  \
    const struct type type                                \

#define __MEE_GET_FIELD(reg, mask)                        \
    (((reg) & (mask)) / ((mask) & ~((mask) << 1)))

/* Set field with mask for a given value */
#define __MEE_SET_FIELD(reg, mask, val) \
        (((reg) & ~(mask)) | (((val) * ((mask) & ~((mask) << 1))) & (mask)))

#endif
