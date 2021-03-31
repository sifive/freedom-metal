/* Copyright 2021 SiFive, Inc */
/* SPDX-License-Identifier: Apache-2.0 */

.section .text.metal.init.sbrs3
.global _sbrs3
_sbrs3:
    .cfi_startproc

	/** Read S3 state */
	/** power = *((uint32_t*)( C_REG_SCR_BASE_ADDRESS + C_REG_SCR_S21_S3_SYS_POWER_STATE )); */
	lui a5,0x4f001
	slli a5,a5,0x4
	addi a5,a5,436
	lw s1,0(a5)

	/** if( power & C_REG_PCSS_SCR_S21_S3_SYS_POWER_STATE_MASK ) */
	mv a5,s1
	andi a5,a5,1
	sext.w a5,a5
	beqz a5,1f

	/** *((uint32_t*)( C_REG_SCR_BASE_ADDRESS + C_REG_SCR_S21_S3_SYS_POWER_STATE )) &= ~C_REG_PCSS_SCR_S21_S3_SYS_POWER_STATE_MASK; */
	lui a5,0x4f001
	slli a5,a5,0x4
	addi a5,a5,436
	lw a4,0(a5)
	lui a5,0x4f001
	slli a5,a5,0x4
	addi a5,a5,436
	andi a4,a4,-2
	sext.w	a4,a4
	sw	a4,0(a5)

	/** s21fw_jump = (uint64_t)( *((uint32_t*)( C_REG_SCR_BASE_ADDRESS + C_REG_DDR_JUMP_REG0 )) & 0x00000000ffffffff ); */
	lui a5,0x4f001
	slli a5,a5,0x4
	addi a5,a5,996
	lw a5,0(a5)
	slli a5,a5,0x20
	srli a5,a5,0x20
	sd a5,-24(s0)

	/** s21fw_jump |= (uint64_t)( ( *((uint64_t*)( C_REG_SCR_BASE_ADDRESS + C_REG_DDR_JUMP_REG1 )) << 32 ) & 0xffffffff0000000 ); */
	lui a5,0x4f001
	slli a5,a5,0x4
	addi a5,a5,1000
	ld a5,0(a5)
	slli a4,a5,0x20
	li a5,-1
	slli a5,a5,0x20
	srli a5,a5,0x4
	and a5,a5,a4
	ld a4,-24(s0)
	or a5,a5,a4
	sd a5,-24(s0)

	/** Jump */
	ld a5,-24(s0)
	jr a5
	nop
	nop
1:
	nop
	nop
	ret

    .cfi_endproc
