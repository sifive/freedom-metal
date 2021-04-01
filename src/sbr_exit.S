/* Copyright 2021 SiFive, Inc */
/* SPDX-License-Identifier: Apache-2.0 */

/*------------------------------------------------------------------*/

/**
 * Register		ABI Name	Description						Saver
 * x0			zero		Hard-wired zero					-
 * x1			ra			Return address					Caller
 * x2			sp			Stack pointer					Callee
 * x3			gp			Global pointer					-
 * x4			tp			Thread pointer					-
 * x5-7			t0-2		Temporaries						Caller
 * x8			s0/fp		Saved register/Frame pointer	Callee
 * x9			s1			Saved register					Callee
 * x10-11		a0-1		Function arguments/return values Caller
 * x12-17		a2-7		Function arguments				Caller
 * x18-27		s2-11		Saved registers					Callee
 * x28-31		t3-6		Temporaries						Caller
 */




.section .text.metal.init.wrap___metal_before_start
.global __wrap___metal_before_start
.type __wrap___metal_before_start, @function
__wrap___metal_before_start:
    .cfi_startproc

    /** Save caller ra */
    mv s0, ra
	/** Call Cease core management */
	call __sbrcease

	/** Then call S3 management */
	call __sbrs3

	/** At least, call real function */
	call __real___metal_before_start

    /** Restore caller ra */
    mv ra, s0
    ret

    .cfi_endproc

.section .text.metal.init.metal_after_main
.global __metal_after_main
.type __metal_after_main, @function
__metal_after_main:
    .cfi_startproc

    /** Inform the debugger that there is nowhere to backtrace past __metal_after_main. */
    .cfi_undefined ra

    /** CUSTOM registers area base address - 0x4f0010000 */
	lui a5, 0x4f001
	slli a5, a5, 0x4
	/** U74_POR_CONTROL register address - 0x4f001080c */
	addi a5, a5, 2060
	/** Get register value */
	lw t0,0(a5)
	/** Set pcss_cpuss_scr_u74_por_reset bit to '1' */
	ori t0, t0, 1
	sext.w t0, t0
	/** Set value in register */
	sw	t0, 0(a5)
	/** Waiting for RESET to apply */
	nop
	nop
	nop
	nop

	/** Special opcode - cease */
	.word 0x30500073
	nop
	nop

    .cfi_endproc
