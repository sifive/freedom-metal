/* Copyright 2018 SiFive, Inc */
/* SPDX-License-Identifier: Apache-2.0 */

#ifndef METAL__DRIVERS__RISCV_DMA0_H
#define METAL__DRIVERS__RISCV_DMA0_H

#include <metal/drivers/sifive_gpio0.h>
#include <metal/clock.h>
#include <metal/compiler.h>
#include <metal/io.h>
#include <metal/dma.h>

/* Register fields */

#define METAL_DMA_CTL_EN 1
#define METAL_DMA_CTL_TST_EN 1 
#define METAL_DMA_SEC_WRLD_EN 1


#define METAL_DMA_QUEUE_DEPTH_SHIFT   8
#define METAL_DMA_QUEUE_DEPTH_MASK    0x00000700UL
#define METAL_DMA_CHANCNT_MINUS1_MASK 0x0000001FUL
#define METAL_DMA_CHAN_STATUS_MASK    0x00000008UL
#define METAL_DMA_CHAN_ERROR_MASK     0x00000004UL

#define METAL_DMA_CHAN_TXFER_DONE_MASK 0x40000000UL

#define METAL_DMA_CHAN_PRIO_SHIFT 1
#define METAL_DMA_CHAN_HW_TRIG_SHIFT 2
#define METAL_DMA_CHAN_SW_TRIG_SHIFT 3
#define METAL_DMA_CHAN_JOB_CYCLE_SHIFT 4
#define METAL_DMA_CHAN_ENABLE_SHIFT 1

#define METAL_DMA_NEXT_JOBPTR_MASK 3
#define METAL_DMA_NEXT_JOBPTR_VALID 0
#define METAL_DMA_NEXT_JOBPTR_INVALID 1

#define METAL_DMA_INTEG_SRAM_MASK 0x00000080UL

#define METAL_DMA_REG(offset)   (((unsigned long)control_base + offset))
#define METAL_DMA_REGB(offset)  (__METAL_ACCESS_ONCE((__metal_io_u8  *)METAL_DMA_REG(offset)))
#define METAL_DMA_REGW(offset)  (__METAL_ACCESS_ONCE((__metal_io_u32 *)METAL_DMA_REG(offset)))


#define METAL_DMA_CHAN_CONTROL_REG(chan_num) (__METAL_ACCESS_ONCE((__metal_io_u32 *)(METAL_DMA_REG(0) + METAL_RISCV_DMA0_CTRL + (chan_num * 0x1000))))
#define METAL_DMA_CHAN_STATUS_REG(chan_num) (__METAL_ACCESS_ONCE((__metal_io_u32 *)(METAL_DMA_REG(0) + 0x020 + (chan_num * 0x0020) + 0x04)))
#define METAL_DMA_CHAN_ENQUE_JOBCTRL_REG(chan_num) (__METAL_ACCESS_ONCE((__metal_io_u32 *)(METAL_DMA_REG(0) + 0x020 + (chan_num * 0x0020) + 0x08))) 
#define METAL_DMA_CHAN_ENQUE_SRC_ENDPTR_REG(chan_num) (__METAL_ACCESS_ONCE((__metal_io_u32 *)(METAL_DMA_REG(0) + 0x020 + (chan_num * 0x0020) + 0x0C))) 
#define METAL_DMA_CHAN_ENQUE_DST_ENTPTR_REG(chan_num) (__METAL_ACCESS_ONCE((__metal_io_u32 *)(METAL_DMA_REG(0) + 0x020 + (chan_num * 0x0020) + 0x10))) 

#define METAL_DMA_JOB_CTRL_REG(chan_num) (__METAL_ACCESS_ONCE((__metal_io_u32 *)(METAL_DMA_REG(0) + 0x0020 + (chan_num * 0x0030))))
#define METAL_DMA_JOB_SRCEND_REG(chan_num) (__METAL_ACCESS_ONCE((__metal_io_u32 *)(METAL_DMA_REG(0) + 0x0020 + (chan_num * 0x0030) + 0x4)))
#define METAL_DMA_JOB_DSTEND_REG(chan_num) (__METAL_ACCESS_ONCE((__metal_io_u32 *)(METAL_DMA_REG(0) + 0x0020 + (chan_num * 0x0030) + 0x8)))
#define METAL_DMA_JOB_NEXTJOBPTR_REG(chan_num) (__METAL_ACCESS_ONCE((__metal_io_u32 *)(METAL_DMA_REG(0) + 0x0020 + (chan_num * 0x0030) + 0xC)))

#define METAL_DMA_CHAN_CONFIG_REG(chan_num) (__METAL_ACCESS_ONCE((__metal_io_u32 *)(METAL_DMA_REG(0) + METAL_RISCV_DMA0_NEXT_CONFIG + (chan_num * 0x1000))))
#define METAL_DMA_CHAN_BYTES_REG(chan_num) (__METAL_ACCESS_ONCE((unsigned long *)(METAL_DMA_REG(0) + METAL_RISCV_DMA0_NEXT_BYTES + (chan_num * 0x1000))))
#define METAL_DMA_CHAN_DEST_REG(chan_num) (__METAL_ACCESS_ONCE((unsigned long *)(METAL_DMA_REG(0) + METAL_RISCV_DMA0_NEXT_DEST + (chan_num * 0x1000))))
#define METAL_DMA_CHAN_SRC_REG(chan_num) (__METAL_ACCESS_ONCE((unsigned long *)(METAL_DMA_REG(0) + METAL_RISCV_DMA0_NEXT_SRC + (chan_num * 0x1000))))



/*****************************************/

struct __metal_driver_vtable_riscv_dma0 {
	const struct metal_dma_vtable dma;
};

__METAL_DECLARE_VTABLE(__metal_driver_vtable_riscv_dma0)

struct __metal_driver_riscv_dma0 {
	struct metal_dma dma;
	unsigned int global_jobq_alloc: 1;
};

#endif
