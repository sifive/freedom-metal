/* Copyright 2018 SiFive, Inc */
/* SPDX-License-Identifier: Apache-2.0 */

#include <metal/machine/platform.h>

#ifdef METAL_SIFIVE_LPDMA0
#include <metal/drivers/sifive_lpdma0.h>
#include <metal/io.h>
#include <metal/machine.h>
#include <time.h>
#include <stdlib.h>
#include <assert.h>

static int configure_dma(struct __metal_driver_sifive_lpdma0 *dma,
                         struct metal_dma_chan_config *config)
{
	long control_base = __metal_driver_sifive_lpdma0_control_base((struct metal_dma *)dma);
	return 0;
}

int __metal_driver_sifive_lpdma0_enable_dma(struct metal_dma *gdma,
        									unsigned int chan,
        									struct metal_dma_chan_config *config)
{
    unsigned int dma_cfg;
	//TO DO: Need to look this below pointer, in non SRAM based
	struct job **jobqtemp;
	struct job *jobtemp;
	unsigned char integ_sram_used, jobq_depth, ch_cnt_minus_1;
	struct __metal_driver_sifive_lpdma0 *dma = (void *)gdma;
	unsigned long control_base = __metal_driver_sifive_lpdma0_control_base((struct metal_dma *)gdma);

	METAL_DMA_REGW(METAL_SIFIVE_LPDMA0_CONTROL) = METAL_DMA_CTL_EN;

	dma_cfg = METAL_DMA_REGW(METAL_SIFIVE_LPDMA0_CONFIG);
	integ_sram_used = __METAL_GET_FIELD(dma_cfg, METAL_DMA_INTEG_SRAM_MASK);
	

#if 0
    jobq_depth = METAL_DMA_REGW(METAL_SIFIVE_LPDMA0_CONFIG);
	jobq_depth = __METAL_GET_FIELD(jobq_depth, METAL_DMA_QUEUE_DEPTH_MASK);
	jobq_depth = jobq_depth ^ 2;
	assert(jobq_depth == 4);
#endif
	jobq_depth = 4;
	
	ch_cnt_minus_1 = __METAL_GET_FIELD(dma_cfg, METAL_DMA_CHANCNT_MINUS1_MASK);
	ch_cnt_minus_1 += 1;

	assert(ch_cnt_minus_1 == 4);
	

	if(!integ_sram_used) {
		METAL_DMA_REGW(METAL_SIFIVE_LPDMA0_JOBQ_PTR) = (uintptr_t) jobqtemp;
	}

	gdma->jobq_depth = jobq_depth;
	gdma->channel_count = ch_cnt_minus_1;



	return 0;
}

int __metal_driver_sifive_lpdma0_disable_dma(struct metal_dma *gdma,
        unsigned int chan)
{

	return 0;
}

int __metal_driver_sifive_lpdma0_chan_active(struct metal_dma *gdma,
        unsigned int chan)
{
	return 0;
}


void __metal_driver_sifive_lpdma0_setup_jobques (struct metal_dma *gdma, unsigned int chan,
                           struct metal_dma_chan_config *config)
{
	struct job tempjob;
	struct __metal_driver_sifive_lpdma0 *dma = (void *)gdma;
	unsigned long control_base = __metal_driver_sifive_lpdma0_control_base((struct  metal_dma *)gdma);    

	for (int i =0; i < 1; i++) {
		tempjob.control.bits = gdma->jobq[chan][i].control.bits;
		tempjob.src_end_ptr = gdma->jobq[chan][i].src_end_ptr;
		tempjob.dst_end_ptr = gdma->jobq[chan][i].dst_end_ptr;

		METAL_DMA_CHAN_ENQUE_JOBCTRL_REG(chan) = tempjob.control.bits;
		METAL_DMA_CHAN_ENQUE_SRC_ENDPTR_REG(chan) = tempjob.src_end_ptr;
		METAL_DMA_CHAN_ENQUE_DST_ENTPTR_REG(chan) = tempjob.dst_end_ptr;
	}	
	//METAL_DMA_REGW(METAL_SIFIVE_LPDMA0_CONTROL) = METAL_DMA_CTL_EN;
}


void __metal_driver_sifive_lpdma0_chan_init (struct metal_dma *gdma,
                                             unsigned int chan,
                                             struct metal_dma_chan_config *config)
{
	struct __metal_driver_sifive_lpdma0 *dma = (void *)gdma;
	unsigned long control_base = __metal_driver_sifive_lpdma0_control_base((struct  metal_dma *)gdma);	

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
}

struct metal_interrupt *
__metal_driver_sifive_lpdma0_get_interrupt(const struct metal_dma *const dma) {
    return __metal_driver_sifive_lpdma0_interrupt_parent(dma);
}

int __metal_driver_sifive_lpdma0_get_interrupt_id(
    const struct metal_dma *const dma) {
    return __metal_driver_sifive_lpdma0_interrupt_line(dma);
}

__METAL_DEFINE_VTABLE(__metal_driver_vtable_sifive_lpdma0) = {
	.dma.dma_enable       =  __metal_driver_sifive_lpdma0_enable_dma,
	.dma.dma_disable      =  __metal_driver_sifive_lpdma0_disable_dma,
	.dma.chan_init        =  __metal_driver_sifive_lpdma0_chan_init,
	.dma.chan_active      =  __metal_driver_sifive_lpdma0_chan_active,
	.dma.setup_jobques    = __metal_driver_sifive_lpdma0_setup_jobques,
	.dma.get_interrupt    = __metal_driver_sifive_lpdma0_get_interrupt,
    .dma.get_interrupt_id = __metal_driver_sifive_lpdma0_get_interrupt_id,
};

#endif /* METAL_SIFIVE_LPDMA0 */
