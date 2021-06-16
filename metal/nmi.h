/* Copyright 2018 SiFive, Inc */
/* SPDX-License-Identifier: Apache-2.0 */

#ifndef METAL__NMI_H
#define METAL__NMI_H

/*! @file nmi.h
 *
 * API for manipulating NMI allocation
 */

#define _STR(x) #x

#define _RNMI_IRQ(ID) _STR(.rnmi_irq_##ID)
#define _RNMI_EXCP(ID) _STR(.rnmi_excp_##ID)

#define METAL_NMI_MNRET __asm__ volatile(".word 0x70200073")

/*! @def METAL_PLACE_IN_RNMI_IRQ
 * @brief Link a function into the RNMI interrupt handler.
 */
#define METAL_PLACE_IN_RNMI_IRQ(ID)                                            \
    __attribute__((section(_RNMI_IRQ(ID)), naked))

/*! @def METAL_PLACE_IN_RNMI_EXCP
 * @brief Link a function into the RNMI exception handler.
 */

#define METAL_PLACE_IN_RNMI_EXCP(ID)                                           \
    __attribute__((section(_RNMI_EXCP(ID)), interrupt))

#endif
