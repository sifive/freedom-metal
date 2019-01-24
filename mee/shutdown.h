/* Copyright 2018 SiFive, Inc */
/* SPDX-License-Identifier: Apache-2.0 */

#ifndef MEE__SHUTDOWN_H
#define MEE__SHUTDOWN_H

/*!
 * @file shutdown.h
 * @brief API for shutting down a machine
 */

struct __mee_shutdown;

struct __mee_shutdown_vtable {
    void (*exit)(const struct __mee_shutdown *sd, int code) __attribute__((noreturn));
};

struct __mee_shutdown {
    const struct __mee_shutdown_vtable *vtable;
};

inline void __mee_shutdown_exit(const struct __mee_shutdown *sd, int code) __attribute__((noreturn));
inline void __mee_shutdown_exit(const struct __mee_shutdown *sd, int code) { sd->vtable->exit(sd, code); }

/*!
 * @brief The public MEE shutdown interface
 *
 * Shuts down the machine, if the machine enables an interface for
 * shutting down. When no interface is provided, will cause the machine
 * to spin indefinitely.
 *
 * @param code The return code to set. 0 indicates program success.
 */
void mee_shutdown(int code) __attribute__((noreturn));

#endif
