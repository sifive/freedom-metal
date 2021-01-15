/* Copyright 2020 SiFive, Inc */
/* SPDX-License-Identifier: Apache-2.0 */

#ifndef SIFIVE_DEFS_H
#define SIFIVE_DEFS_H

/*!
 * @file sifive_defs.h
 * @brief Common define for Sifive drivers
 */

#pragma GCC diagnostic push
/* Accept CMSIS syntax */
#pragma GCC diagnostic ignored "-Wreserved-id-macro"

/* following defines should be used for structure members */
#define __IM                                                                   \
    volatile const /**< Defines 'read only' structure member permissions */
#define __OM                                                                   \
    volatile /**< Defines 'write only' structure member permissions            \
              */
#define __IOM                                                                  \
    volatile /**< Defines 'read / write' structure member permissions */

#pragma GCC diagnostic pop

#endif /* SIFIVE_DEFS_H */
