/* Copyright 2021 SiFive, Inc */
/* SPDX-License-Identifier: Apache-2.0 */

.section .text.metal.init.sbrcease
.global _sbrcease
_sbrcease:
    .cfi_startproc

    /** __asm__ __volatile__("csrr %0, mhartid" : "=r"(mhartid)); */
	csrr a5,mhartid
	sext.w s1,a5

    /**  tile = ( *((uint32_t*)( C_REG_SCR_BASE_ADDRESS + C_REG_OTP_SCR_SKU_ENABLE )) & ( C_REG_OTP_SCR_TILE3_ENABLE_MASK | C_REG_OTP_SCR_TILE2_ENABLE_MASK | C_REG_OTP_SCR_TILE1_ENABLE_MASK ) ); */
	lui a5,0x4f001
	slli a5,a5,0x4
	addi a5,a5,24
	lw a5,0(a5)
	andi a5,a5,7
	sext.w s2,a5

	/** if( ( tile & C_REG_OTP_SCR_TILE3_ENABLE_MASK ) && ( 3 == mhartid ) ) */
	mv a5,s2
	andi a5,a5,1
	sext.w a5,a5
	beqz a5,1f
	mv a4,s1
	li a5,3
	bne a4,a5,1f
	/** Special opcode - cease */
	.word 0x30500073
	nop
1:
	/** if( ( tile & C_REG_OTP_SCR_TILE2_ENABLE_MASK ) && ( 2 == mhartid ) ) */
	mv a5,s2
	andi a5,a5,2
	sext.w a5,a5
	beqz a5,2f
	mv a4,s1
	li a5,2
	bne a4,a5,2f
	/** Special opcode - cease */
	.word 0x30500073
	nop
2:
	/** if( ( tile & C_REG_OTP_SCR_TILE1_ENABLE_MASK ) && ( 1 == mhartid ) ) */
	mv a5,s2
	andi a5,a5,4
	sext.w a5,a5
	beqz a5,3f
	mv a4,s1
	li a5,1
	bne a4,a5,3f
	/** Special opcode - cease */
	.word 0x30500073
	nop
3:
	nop
	nop
	nop
	ret

     .cfi_endproc
