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

#define RET_OK 0
#define RET_NOK 1

int __metal_driver_riscv_dma0_enable_dma(struct metal_dma *gdma,
        									unsigned int chan,
        									struct metal_dma_chan_config *config)
{
#if 0
    unsigned int dma_cfg;
	//TO DO: Need to look this below pointer, in non SRAM based
	struct job **jobqtemp;
	struct job *jobtemp;
	unsigned char integ_sram_used, jobq_depth, ch_cnt_minus_1;
	struct __metal_driver_riscv_dma0 *dma = (void *)gdma;
	unsigned long control_base = __metal_driver_riscv_dma0_control_base((struct metal_dma *)gdma);

	METAL_DMA_REGW(METAL_RISCV_DMA0_CONTROL) = METAL_DMA_CTL_EN;

	dma_cfg = METAL_DMA_REGW(METAL_RISCV_DMA0_CONFIG);
	integ_sram_used = __METAL_GET_FIELD(dma_cfg, METAL_DMA_INTEG_SRAM_MASK);
	

#if 0
    jobq_depth = METAL_DMA_REGW(METAL_RISCV_DMA0_CONFIG);
	jobq_depth = __METAL_GET_FIELD(jobq_depth, METAL_DMA_QUEUE_DEPTH_MASK);
	jobq_depth = jobq_depth ^ 2;
	assert(jobq_depth == 4);
#endif
	jobq_depth = 4;
	
	ch_cnt_minus_1 = __METAL_GET_FIELD(dma_cfg, METAL_DMA_CHANCNT_MINUS1_MASK);
	ch_cnt_minus_1 += 1;

	assert(ch_cnt_minus_1 == 4);
	

	if(!integ_sram_used) {
		METAL_DMA_REGW(METAL_RISCV_DMA0_JOBQ_PTR) = (uintptr_t) jobqtemp;
	}

	gdma->jobq_depth = jobq_depth;
	gdma->channel_count = ch_cnt_minus_1;

#endif

	return 0;
}

int __metal_driver_riscv_dma0_disable_dma(struct metal_dma *gdma,
        unsigned int chan)
{

	return 0;
}

int __metal_driver_riscv_dma0_chan_active(struct metal_dma *gdma,
        unsigned int chan)
{
	return 0;
}


void __metal_driver_riscv_dma0_setup_jobques (struct metal_dma *gdma, unsigned int chan,
                           struct metal_dma_chan_config *config)
{
#if 0
	struct job tempjob;
	struct __metal_driver_riscv_dma0 *dma = (void *)gdma;
	unsigned long control_base = __metal_driver_riscv_dma0_control_base((struct  metal_dma *)gdma);    

	for (int i =0; i < 1; i++) {
		tempjob.control.bits = gdma->jobq[chan][i].control.bits;
		tempjob.src_end_ptr = gdma->jobq[chan][i].src_end_ptr;
		tempjob.dst_end_ptr = gdma->jobq[chan][i].dst_end_ptr;

		METAL_DMA_CHAN_ENQUE_JOBCTRL_REG(chan) = tempjob.control.bits;
		METAL_DMA_CHAN_ENQUE_SRC_ENDPTR_REG(chan) = tempjob.src_end_ptr;
		METAL_DMA_CHAN_ENQUE_DST_ENTPTR_REG(chan) = tempjob.dst_end_ptr;
	}	
	//METAL_DMA_REGW(METAL_RISCV_DMA0_CONTROL) = METAL_DMA_CTL_EN;
#endif
}


void __metal_driver_riscv_dma0_chan_init (struct metal_dma *gdma,
                                             unsigned int chan,
                                             struct metal_dma_chan_config *config)
{
	struct __metal_driver_riscv_dma0 *dma = (void *)gdma;
	unsigned long control_base = __metal_driver_riscv_dma0_control_base((struct  metal_dma *)gdma);	
	
        METAL_DMA_CHAN_CONTROL_REG(chan) |= 0x1;

	METAL_DMA_CHAN_BYTES_REG(chan) = config->num_bytes;
	METAL_DMA_CHAN_SRC_REG(chan) = config->src_ptr;
	METAL_DMA_CHAN_DEST_REG(chan) = config->dst_ptr;
	METAL_DMA_CHAN_CONFIG_REG(chan) = config->jobctrl.bits;


#if 0
	struct __metal_driver_riscv_dma0 *dma = (void *)gdma;
	unsigned long control_base = __metal_driver_riscv_dma0_control_base((struct  metal_dma *)gdma);	

	unsigned int val;
	unsigned char channel_status = 1;

	switch (config->priority) {
	case METAL_DMA_PRIORITY_VERY_HIGH:
		break;
	case METAL_DMA_PRIORITY_HIGH:
		METAL_DMA_CHAN_CONTROL_REG(chan) |= (1 << METAL_DMA_CHAN_PRIO_SHIFT);
break;
	case METAL_DMA_PRIORITY_MEDIUM:
		break;
	case METAL_DMA_PRIORITY_LOW:
		METAL_DMA_CHAN_CONTROL_REG(chan) &= ~(1 << METAL_DMA_CHAN_PRIO_SHIFT);
		break;
	default:
		break;
	}

	if (config->hw_trig_en) {
		METAL_DMA_CHAN_CONTROL_REG(chan) |= (1 << METAL_DMA_CHAN_HW_TRIG_SHIFT);
	}
	else {
		METAL_DMA_CHAN_CONTROL_REG(chan) &= ~(1 << METAL_DMA_CHAN_HW_TRIG_SHIFT);
	}

	if (config->sw_trig) {
		METAL_DMA_CHAN_CONTROL_REG(chan) |= (1 << METAL_DMA_CHAN_SW_TRIG_SHIFT);
	}
	else {
		METAL_DMA_CHAN_CONTROL_REG(chan) &= ~(1 << METAL_DMA_CHAN_SW_TRIG_SHIFT);
	}

	if (config->job_cycle) {
		METAL_DMA_CHAN_CONTROL_REG(chan) |= (1 << METAL_DMA_CHAN_JOB_CYCLE_SHIFT);
	}
	else {
		METAL_DMA_CHAN_CONTROL_REG(chan) &= ~(1 << METAL_DMA_CHAN_JOB_CYCLE_SHIFT);
	}

	//METAL_DMA_CHAN_CONTROL_REG(chan) |= (1 << METAL_DMA_CHAN_ENABLE_SHIFT);

	val = METAL_DMA_CHAN_CONTROL_REG(chan);
	METAL_DMA_CHAN_CONTROL_REG(chan) = __METAL_SET_FIELD(val, 1, 1);

	while(channel_status) {
		val = METAL_DMA_CHAN_STATUS_REG(chan);
		channel_status = __METAL_GET_FIELD(val, METAL_DMA_CHAN_STATUS_MASK);
	}
#endif
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

struct metal_interrupt *
__metal_driver_riscv_dma0_get_interrupt(const struct metal_dma *const dma) {
    return __metal_driver_riscv_dma0_interrupt_parent(dma);
}

int __metal_driver_riscv_dma0_get_interrupt_id(
    const struct metal_dma *const dma) {
    return __metal_driver_riscv_dma0_interrupt_line(dma);
}

__METAL_DEFINE_VTABLE(__metal_driver_vtable_riscv_dma0) = {
	.dma.dma_enable       =  __metal_driver_riscv_dma0_enable_dma,
	.dma.dma_disable      =  __metal_driver_riscv_dma0_disable_dma,
	.dma.chan_init        =  __metal_driver_riscv_dma0_chan_init,
	.dma.chan_active      =  __metal_driver_riscv_dma0_chan_active,
	.dma.setup_jobques    = __metal_driver_riscv_dma0_setup_jobques,
	.dma.get_interrupt    = __metal_driver_riscv_dma0_get_interrupt,
    .dma.get_interrupt_id = __metal_driver_riscv_dma0_get_interrupt_id,
    .dma.chan_txfer_enable = __metal_driver_riscv_dma0_txfer_enable,
    .dma.chan_txfer_status = __metal_driver_riscv_dma0_txfer_status,
    .dma.chan_txfer_enableIT = __metal_driver_riscv_dma0_enableIT,

};

#endif /* METAL_RISCV_DMA0 */
