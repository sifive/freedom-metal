/* Copyright 2020 SiFive, Inc */
/* SPDX-License-Identifier: Apache-2.0 */

#ifndef METAL__DRIVERS__SIFIVE_DEFS_H
#define METAL__DRIVERS__SIFIVE_DEFS_H

/*!
 * @file sifive_defs.h
 * @brief Common define for Sifive drivers
 */

/* following defines should be used for structure members */
#define __IM                                                                   \
    volatile const /**< Defines 'read only' structure member permissions */
#define __OM                                                                   \
    volatile /**< Defines 'write only' structure member permissions            \
              */
#define __IOM                                                                  \
    volatile /**< Defines 'read / write' structure member permissions */

#endif /* METAL__DRIVERS__SIFIVE_DEFS_H */