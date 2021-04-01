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


.section .text.metal.init.sbrs3
.global __sbrs3
.type __sbrs3, @function
__sbrs3:
    .cfi_startproc
    /** Save caller ra */
    mv s0, ra

	/** Retrieve S3 state from SCR/S21_S3_SYS_POWER_STATE register */
	/** Store SCR regsiter base address in a5 regsiter */
	lui a5, 0x4f001
	slli a5, a5, 0x4
	/** Store S21_S3_SYS_POWER_STATE register address in a7 regsiter */
	addi a7, a5, 436
	/** Read value of SCR/S21_S3_SYS_POWER_STATE and store it into s1 register */
	lw s1, 0(a7)

	/** Check if pcss_scr_s21_s3_sys_power_state bit has been set */
	mv a2, s1
	andi a2, a2, 1
	sext.w a2, a2
	/** If pcss_scr_s21_s3_sys_power_state bit has not been set then exit */
	beqz a2, 1f

	/** Here means pcss_scr_s21_s3_sys_power_state bit has been set, then clear it */
	/** Retrieve value stored in s1 */
	mv a4, s1
	/** Logical s1 AND 0xfffffffffffffffe to clear bit */
	andi a4, s1, -2
	sext.w a4, a4
	/** Write value back into S21_S3_SYS_POWER_STATE register then */
	sw	a4, 0(a7)

	/** SCR base address is stored in a5 */
	/** Store DDR_JUMP_REG0 resgiter address in t5 register */
	addi t5, a5, 996
	/** Read value of SCR/DDR_JUMP_REG0 and store it into t0 register */
	lw t0, 0(t5)
	/** Mask retrieve value in t0 with 0x00000000ffffffff */
	slli t0, t0, 0x20
	srli t0, t0, 0x20
	/** Store t0 value in s7 for future processing*/
	sd t0, s7

	/** SCR base address is stored in a5 */
	/** Store DDR_JUMP_REG1 resgiter address in t6 register */
	addi t6, a5, 1000
	/** Read value of SCR/DDR_JUMP_REG1 and store it into t0 register */
	ld t0, 0(t6)
	/** Left shift value in t0 and put it in t1*/
	slli t1, t0, 0x20
	/** Create mask 0xffffffff00000000 in t2 register */
	li t2, -1
	slli t2, t2, 0x20
	/** Apply mask to value stored in t1 and store result in s8 */
	and s8, t2, t1
	/** Combine s7 and s8 in s10  */
	or s10, s7, s8
	/** Jump at address store in s10*/
	/** ld a5, s7
	jr a5 */
	jr s10
	nop
	nop
	nop
	nop
	/** It should not return, but if so, it's security breach then reset U74 core complex */
	call __metal_after_main
	nop
	nop
1:
    /** Restore caller ra */
    mv ra, s0
	ret

    .cfi_endproc
