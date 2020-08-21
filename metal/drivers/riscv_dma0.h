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

#define METAL_DMA_CHAN_STATUS_MASK    0x00000008UL
#define METAL_DMA_CHAN_ERROR_MASK     0x00000004UL
#define METAL_DMA_CHAN_TXFER_DONE_MASK 0x40000000UL

#define METAL_DMA_CHAN_ENABLE_SHIFT 1
#define METAL_DMA_CHAN_INT_DONE_SHIFT 14
#define METAL_DMA_CHAN_INT_ERR_SHIFT 15


#define METAL_DMA_REG(offset)   (((unsigned long)control_base + offset))
#define METAL_DMA_REGB(offset)  (__METAL_ACCESS_ONCE((__metal_io_u8  *)METAL_DMA_REG(offset)))
#define METAL_DMA_REGW(offset)  (__METAL_ACCESS_ONCE((__metal_io_u32 *)METAL_DMA_REG(offset)))


#define METAL_DMA_CHAN_CONTROL_REG(chan_num) (__METAL_ACCESS_ONCE((__metal_io_u32 *)(METAL_DMA_REG(0) + METAL_RISCV_DMA0_CTRL + (chan_num * 0x1000))))
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
};

#endif