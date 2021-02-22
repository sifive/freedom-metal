/* Copyright 2018 SiFive, Inc */
/* SPDX-License-Identifier: Apache-2.0 */

#ifndef METAL__DRIVERS__SIFIVE_NB2OTP_H
#define METAL__DRIVERS__SIFIVE_NB2OTP_H

#include <metal/io.h>
#include <metal/compiler.h>
#include <metal/clock.h>
#include <metal/interrupt.h>
#include <metal/otp.h>

struct __metal_driver_vtable_sifive_nb2otp {
    const struct metal_otp_vtable otp;
};

struct __metal_driver_sifive_nb2otp;

__METAL_DECLARE_VTABLE(__metal_driver_vtable_sifive_nb2otp);

struct __metal_driver_sifive_nb2otp {
    const struct metal_otp otp;
};

#endif
