/* Copyright 2018 SiFive, Inc */
/* SPDX-License-Identifier: Apache-2.0 */

#include <metal/machine/platform.h>

#ifdef METAL_RISCV_DMA0
#include <metal/drivers/riscv_dma0.h>
#include <metal/io.h>
#include <metal/machine.h>
#include <time.h>
#include <stdlib.h>
#include <assert.h>

#define METAL_DMA_RET_OK 0
#define METAL_DMA_RET_ERR -1


int __metal_driver_riscv_dma0_chan_active(struct metal_dma *gdma,
        unsigned int chan)
{
	return 0;
}

void __metal_driver_riscv_dma0_chan_init (struct metal_dma *gdma,
                                             unsigned int chan,
                                             struct metal_dma_chan_config *config)
{
	struct __metal_driver_riscv_dma0 *dma = (void *)gdma;
	unsigned long control_base = __metal_driver_riscv_dma0_control_base((struct  metal_dma *)gdma);	
	
        METAL_DMA_CHAN_CONTROL_REG(chan) &= (~0xFFFFFFFF);
        METAL_DMA_CHAN_CONTROL_REG(chan) |= 0x1;

	METAL_DMA_CHAN_BYTES_REG(chan) = config->num_bytes;
	METAL_DMA_CHAN_SRC_REG(chan) = config->src_ptr;
	METAL_DMA_CHAN_DEST_REG(chan) = config->dst_ptr;
	METAL_DMA_CHAN_CONFIG_REG(chan) = config->jobctrl.bits;
}

void __metal_driver_riscv_dma0_txfer_enable(struct metal_dma *gdma, unsigned int chanid, struct metal_dma_chan_config *config) {
	struct __metal_driver_riscv_dma0 *dma = (void *)gdma;
	unsigned long control_base = __metal_driver_riscv_dma0_control_base((struct  metal_dma *)gdma);	

	METAL_DMA_CHAN_CONTROL_REG(chanid) |= (1 << METAL_DMA_CHAN_ENABLE_SHIFT);
}

int __metal_driver_riscv_dma0_txfer_status(struct metal_dma *gdma, unsigned int chanid) {
	struct __metal_driver_riscv_dma0 *dma = (void *)gdma;
	unsigned long control_base = __metal_driver_riscv_dma0_control_base((struct  metal_dma *)gdma);	

	unsigned int val = 0;
	unsigned int txfer_status = 0;
	val = METAL_DMA_CHAN_CONTROL_REG(chanid);

	txfer_status =  __METAL_GET_FIELD(val, METAL_DMA_CHAN_TXFER_DONE_MASK);
	return txfer_status;
}

void __metal_driver_riscv_dma0_enableIT(struct metal_dma *gdma, unsigned int chanid, struct metal_dma_chan_config *config) {
   
}


static struct metal_interrupt *
__metal_driver_riscv_dma0_interrupt_controller(struct metal_dma *gdma) {
    return __metal_driver_riscv_dma0_interrupt_parent(gdma);
}

int __metal_driver_riscv_dma0_get_interrupt_id(
    struct metal_dma *gdma, unsigned int idx) {
    return __metal_driver_riscv_dma0_interrupt_lines(gdma, idx);
}

int __metal_driver_riscv_dma0_chan_cfg_interrupt(struct metal_dma *gdma, unsigned int chanid, metal_dma_interrupt_t flag) {
    struct __metal_driver_riscv_dma0 *dma = (void *)gdma;
    unsigned long control_base = __metal_driver_riscv_dma0_control_base((struct  metal_dma *)gdma);
    int ret = METAL_DMA_RET_ERR;

    if(control_base != 0) {
        if (flag == METAL_DMA_INT_DONE_ENABLE) {
            METAL_DMA_CHAN_CONTROL_REG(chanid) |= (1 << METAL_DMA_CHAN_INT_DONE_SHIFT);
        }
        if (flag == METAL_DMA_INT_ERR_ENABLE) {
            METAL_DMA_CHAN_CONTROL_REG(chanid) |= (1 << METAL_DMA_CHAN_INT_ERR_SHIFT);
        }
        if (flag == METAL_DMA_INT_DONE_ENABLE | METAL_DMA_INT_ERR_ENABLE) {
            METAL_DMA_CHAN_CONTROL_REG(chanid) |= (1 << METAL_DMA_CHAN_INT_DONE_SHIFT);
            METAL_DMA_CHAN_CONTROL_REG(chanid) |= (1 << METAL_DMA_CHAN_INT_ERR_SHIFT);
        }
        ret = METAL_DMA_RET_OK;
    }
    return ret;
}

int __metal_driver_riscv_dma0_chan_clr_interrupt(struct metal_dma *gdma, unsigned int chanid, metal_dma_interrupt_t flag) {
    struct __metal_driver_riscv_dma0 *dma = (void *)gdma;
    unsigned long control_base = __metal_driver_riscv_dma0_control_base((struct  metal_dma *)gdma);
    int ret = METAL_DMA_RET_ERR;

    if(control_base != 0) {
        if (flag == METAL_DMA_INT_DONE_ENABLE) {
            METAL_DMA_CHAN_CONTROL_REG(chanid) &= ~(1 << METAL_DMA_CHAN_INT_DONE_SHIFT);
        }
        if (flag == METAL_DMA_INT_ERR_ENABLE) {
            METAL_DMA_CHAN_CONTROL_REG(chanid) &= ~(1 << METAL_DMA_CHAN_INT_ERR_SHIFT);
        }
        if (flag == METAL_DMA_INT_DONE_ENABLE | METAL_DMA_INT_ERR_ENABLE) {
            METAL_DMA_CHAN_CONTROL_REG(chanid) &= ~(1 << METAL_DMA_CHAN_INT_DONE_SHIFT);
            METAL_DMA_CHAN_CONTROL_REG(chanid) &= ~(1 << METAL_DMA_CHAN_INT_ERR_SHIFT);
        }
        ret = METAL_DMA_RET_OK;
    }
    return ret;
}

__METAL_DEFINE_VTABLE(__metal_driver_vtable_riscv_dma0) = {
    .dma.chan_init        =  __metal_driver_riscv_dma0_chan_init,
    .dma.chan_active      =  __metal_driver_riscv_dma0_chan_active,
    .dma.get_interrupt_id = __metal_driver_riscv_dma0_get_interrupt_id,
    .dma.chan_txfer_enable = __metal_driver_riscv_dma0_txfer_enable,
    .dma.chan_txfer_status = __metal_driver_riscv_dma0_txfer_status,
    .dma.chan_txfer_enableIT = __metal_driver_riscv_dma0_enableIT,
    .dma.get_interrupt_controller = __metal_driver_riscv_dma0_interrupt_controller,
    .dma.chan_cfg_interrupt = __metal_driver_riscv_dma0_chan_cfg_interrupt,
    .dma.chan_clr_interrupt = __metal_driver_riscv_dma0_chan_clr_interrupt,
};

#endif /* METAL_RISCV_DMA0 */
