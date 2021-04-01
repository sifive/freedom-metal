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


.section .text.metal.init.sbrcease
.global __sbrcease
.type __sbrcease, @function
__sbrcease:
    .cfi_startproc
    /** Save caller ra */
    mv s0, ra

    /** __asm__ __volatile__("csrr %0, mhartid" : "=r"(mhartid)); */
	csrr a5,mhartid
	/** Save running core identifier in s1 register */
	sext.w s1,a5

    /** Retrieve the 3 tiles information from SCR/OTP_SCR_SKU_ENABLE register - 0x4f0010018 */
	lui a6, 0x4f001
	slli a6, a6, 0x4
	addi a6, a6, 24
	/** Read value of SCR/OTP_SCR_SKU_ENABLE */
	lw t1, 0(a6)
	/** Mask this value t retrieve tile3/2/1 information */
	andi t1, t1 ,7
	/** Store tile3/2/1 is s2 register*/
	sext.w s2, t1

	/** Filter tile3 bit from s2 stored value */
	mv t2, s2
	andi t2, t2, 1
	sext.w t2, t2
	/** Check if tile3 is targeted */
	beqz t2, 1f
	/** If targeted then retrieved stored HartId */
	mv t2, s1
	/** Check if it's core3 */
	li t3, 3
	/** Check if tile3 is the running one */
	bne t2, t3, 1f
	/** Special opcode - "cease" feature  */
	.word 0x30500073
	nop
	nop
1:
	/** Filter tile2 bit from s2 stored value */
	mv t2, s2
	andi t2, t2, 2
	sext.w t2, t2
	/** Check if tile2 is targeted */
	beqz t2, 2f
	/** If targeted then retrieved stored HartId */
	mv t1, s1
	/** Check if it's core2 */
	li t3, 2
	/** Check if tile2 is the running one */
	bne t1, t3, 2f
	/** Special opcode - "cease" feature */
	.word 0x30500073
	nop
	nop
2:
	/** Filter tile1 bit from s2 stored value */
	mv t2, s2
	andi t2, t2, 4
	sext.w t2, t2
	/** Check if tile1 is targeted */
	beqz t2, 3f
	/** If targeted then retrieved stored HartId */
	mv t1, s1
	/** Check if it's core1 */
	li t3, 1
	/** Check if tile1 is the running one */
	bne t1, t3, 3f
	/** Special opcode - "cease" feature */
	.word 0x30500073
	nop
	nop
3:
    /** Restore caller ra */
    mv ra, s0
	ret

     .cfi_endproc
