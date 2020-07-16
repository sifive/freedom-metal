/* Copyright 2020 SiFive, Inc */
/* SPDX-License-Identifier: Apache-2.0 */

#ifndef METAL__INTERRUPT_VECTOR_HANDLERS_H
#define METAL__INTERRUPT_VECTOR_HANDLERS_H

/*! @file interrupt_vector_handlers.h
 *  @brief Vectored interrupt handler declarations
 */

/*!
 * @brief Default interrupt vector handler, that can be overriden by user
 * @param None
 * @return None
 */
void __attribute__((weak, interrupt)) metal_interrupt_vector_handler(void);

/*!
 * @brief Metal Software interrupt vector handler, that can be overriden by user
 * @param None
 * @return None
 */
void __attribute__((weak, interrupt))
metal_software_interrupt_vector_handler(void);

/*!
 * @brief Metal Timer interrupt vector handler, that can be overriden by user
 * @param None
 * @return None
 */
void __attribute__((weak, interrupt))
metal_timer_interrupt_vector_handler(void);

/*!
 * @brief Metal External interrupt vector handler, that can be overriden by user
 * @param None
 * @return None
 */
void __attribute__((weak, interrupt))
metal_external_interrupt_vector_handler(void);

/*!
 * @brief Metal Local 0 interrupt vector handler, that can be overriden by user
 * @param None
 * @return None
 */
void __attribute__((weak, interrupt)) metal_lc0_interrupt_vector_handler(void);

/*!
 * @brief Metal Local 1 interrupt vector handler, that can be overriden by user
 * @param None
 * @return None
 */
void __attribute__((weak, interrupt)) metal_lc1_interrupt_vector_handler(void);

/*!
 * @brief Metal Local 2 interrupt vector handler, that can be overriden by user
 * @param None
 * @return None
 */
void __attribute__((weak, interrupt)) metal_lc2_interrupt_vector_handler(void);

/*!
 * @brief Metal Local 3 interrupt vector handler, that can be overriden by user
 * @param None
 * @return None
 */
void __attribute__((weak, interrupt)) metal_lc3_interrupt_vector_handler(void);

/*!
 * @brief Metal Local 4 interrupt vector handler, that can be overriden by user
 * @param None
 * @return None
 */
void __attribute__((weak, interrupt)) metal_lc4_interrupt_vector_handler(void);

/*!
 * @brief Metal Local 5 interrupt vector handler, that can be overriden by user
 * @param None
 * @return None
 */
void __attribute__((weak, interrupt)) metal_lc5_interrupt_vector_handler(void);

/*!
 * @brief Metal Local 6 interrupt vector handler, that can be overriden by user
 * @param None
 * @return None
 */
void __attribute__((weak, interrupt)) metal_lc6_interrupt_vector_handler(void);

/*!
 * @brief Metal Local 7 interrupt vector handler, that can be overriden by user
 * @param None
 * @return None
 */
void __attribute__((weak, interrupt)) metal_lc7_interrupt_vector_handler(void);

/*!
 * @brief Metal Local 8 interrupt vector handler, that can be overriden by user
 * @param None
 * @return None
 */
void __attribute__((weak, interrupt)) metal_lc8_interrupt_vector_handler(void);

/*!
 * @brief Metal Local 9 interrupt vector handler, that can be overriden by user
 * @param None
 * @return None
 */
void __attribute__((weak, interrupt)) metal_lc9_interrupt_vector_handler(void);

/*!
 * @brief Metal Local 10 interrupt vector handler, that can be overriden by user
 * @param None
 * @return None
 */
void __attribute__((weak, interrupt)) metal_lc10_interrupt_vector_handler(void);

/*!
 * @brief Metal Local 11 interrupt vector handler, that can be overriden by user
 * @param None
 * @return None
 */
void __attribute__((weak, interrupt)) metal_lc11_interrupt_vector_handler(void);

/*!
 * @brief Metal Local 12 interrupt vector handler, that can be overriden by user
 * @param None
 * @return None
 */
void __attribute__((weak, interrupt)) metal_lc12_interrupt_vector_handler(void);

/*!
 * @brief Metal Local 13 interrupt vector handler, that can be overriden by user
 * @param None
 * @return None
 */
void __attribute__((weak, interrupt)) metal_lc13_interrupt_vector_handler(void);

/*!
 * @brief Metal Local 14 interrupt vector handler, that can be overriden by user
 * @param None
 * @return None
 */
void __attribute__((weak, interrupt)) metal_lc14_interrupt_vector_handler(void);

/*!
 * @brief Metal Local 15 interrupt vector handler, that can be overriden by user
 * @param None
 * @return None
 */
void __attribute__((weak, interrupt)) metal_lc15_interrupt_vector_handler(void);

#endif
