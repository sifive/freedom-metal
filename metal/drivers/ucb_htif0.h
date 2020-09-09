/* Copyright 2020 SiFive, Inc */
/* SPDX-License-Identifier: Apache-2.0 */

#ifndef METAL__DRIVERS__UCB_HTIF0_H
#define METAL__DRIVERS__UCB_HTIF0_H

/*!
 * @brief Print a character over the HTIF interface
 * @param c The character to print
 * @return 0 upon success
 *
 * If the ucb,htif0 device is selected as the standard out path
 * in the Devicetree's chosen node, metal_tty_putc() (and therefore
 * all standard out from libc) is sent out over this function.
 *
 * The driver exposes this function itself so that applications can
 * manually print characters over the HTIF even when another device
 * is selected as the standard out path.
 */
int ucb_htif0_putc(int c);

#endif /* !METAL__DRIVERS_UCB_HTIF0_H */
