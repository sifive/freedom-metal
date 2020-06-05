/* Copyright 2018 SiFive, Inc */
/* SPDX-License-Identifier: Apache-2.0 */

#ifndef METAL__SHUTDOWN_H
#define METAL__SHUTDOWN_H

/*!
 * @file shutdown.h
 * @brief API for shutting down a machine
 */

#include <stdint.h>

/*!
 * @brief The public METAL shutdown interface
 *
 * Shuts down the machine, if the machine enables an interface for
 * shutting down. When no interface is provided, will cause the machine
 * to spin indefinitely.
 *
 * @param code The return code to set. 0 indicates program success.
 */
void metal_shutdown(int code) __attribute__((noreturn));

#endif
