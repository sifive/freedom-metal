/* Copyright 2018 SiFive, Inc */
/* SPDX-License-Identifier: Apache-2.0 */

#include <metal/machine/platform.h>

#ifdef METAL_SIFIVE_LPDMA0
#include <metal/drivers/sifive_lpdma0.h>
#include <metal/io.h>
#include <metal/machine.h>
#include <time.h>
#include <stdlib.h>

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
	unsigned int jobq_num = 4;
	struct job *temp_job = NULL;
	long control_base = __metal_driver_sifive_lpdma0_control_base((struct metal_dma *)gdma);

	/**
		STEP1: Setup the Job Queues
		STEP2: Enable the Transfer of the ch
	*/
	
	temp_job = config->jobq;

	while (jobq_num--)
	{
		METAL_DMA_JOB_CTRL_REG(chan) = temp_job->control.bits;
		METAL_DMA_JOB_SRCEND_REG(chan) = temp_job->src_end_ptr;
		METAL_DMA_JOB_DSTEND_REG(chan) = temp_job->dst_end_ptr;
		if (config->queue_type != METAL_DMA_LINKED_LIST)
		{
			METAL_DMA_JOB_NEXTJOBPTR_REG(chan) &= METAL_DMA_NEXT_JOBPTR_INVALID;
		}
	}
	
	METAL_DMA_CHAN_CONTROL_REG(chan) |= 0x1;
	//*((uint32_t *) ((uintptr_t)0x10012000 + 0x0020 + (chan * 0x30))) = 0x1233;

	return 0;
}

int __metal_driver_sifive_lpdma0_disable_dma(struct metal_dma *gdma,
        unsigned int chan)
{

	return 0;
}

int __metal_driver_sifive_lpdma0_channel_active(struct metal_dma *gdma,
        unsigned int chan)
{
	return 0;
}


typedef struct job JOB;
JOB *start = NULL;
JOB *ptr;

static JOB *create_jobqueue (unsigned int queue_depth) {

	if (queue_depth == 0) return start;

	JOB *jobptr;
	if (!(jobptr = malloc (sizeof(struct job)))) {
		exit (1);
	}

	jobptr->next_job = NULL;
	if (start == NULL)
	{
		ptr = start = jobptr;
	}
	else
	{
		ptr->next_job = jobptr;
		ptr = ptr->next_job;
	}
	create_jobqueue (queue_depth - 1);
}




void __metal_driver_sifive_lpdma0_setup_jobques (struct metal_dma *gdma,
                           struct metal_dma_chan_config *config)
{
	unsigned int integ_sram_used;
	struct __metal_driver_sifive_lpdma0 *dma = (void *)gdma;
	unsigned long control_base = __metal_driver_sifive_lpdma0_control_base((struct metal_dma *)gdma);
	//unsigned int jobq_depth = METAL_DMA_REGW(METAL_SIFIVE_LPDMA0_CONFIG);
	//unsigned int jobq_depth = *((uint32_t *) 0x10012000);
	//unsigned int jobq_depth = *((uint32_t *) ((uintptr_t)0x10012000));
	unsigned int jobq_depth = __METAL_GET_FIELD(METAL_DMA_REGW(METAL_SIFIVE_LPDMA0_CONFIG), METAL_DMA_QUEUE_DEPTH_MASK);
	jobq_depth ^= 2;

	//config->jobq = create_jobqueue(jobq_depth);

	integ_sram_used = __METAL_GET_FIELD(METAL_DMA_REGW(METAL_SIFIVE_LPDMA0_CONFIG), METAL_DMA_INTEG_SRAM_MASK);

	if(!integ_sram_used) {
		//METAL_DMA_REGW(METAL_SIFIVE_LPDMA0_JOB_QUEUE) = (uint32_t )config->jobq;
	}

	if (config->queue_type == METAL_DMA_CIRCULAR) {

	}
	if (config->queue_type == METAL_DMA_LINKED_LIST) {

	}

	dma->global_jobq_alloc = 1;
}


void __metal_driver_sifive_lpdma0_init (struct metal_dma *gdma,
                                        unsigned int chan,
                                        struct metal_dma_chan_config *config)
{
	struct __metal_driver_sifive_lpdma0 *dma = (void *)gdma;
	unsigned long control_base = __metal_driver_sifive_lpdma0_control_base((struct  metal_dma *)gdma);

	METAL_DMA_REGW(METAL_SIFIVE_LPDMA0_CONTROL) = METAL_DMA_CTL_EN;

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

	//if(dma->global_jobq_alloc == 0) {
		//setup_jobques(gdma, config);
	//}
}

__METAL_DEFINE_VTABLE(__metal_driver_vtable_sifive_lpdma0) = {
	.dma.init            =  __metal_driver_sifive_lpdma0_init,
	.dma.enable_dma      =  __metal_driver_sifive_lpdma0_enable_dma,
	.dma.disable_dma     =  __metal_driver_sifive_lpdma0_disable_dma,
	.dma.channel_active  =  __metal_driver_sifive_lpdma0_channel_active,
	.dma.setup_jobques   = __metal_driver_sifive_lpdma0_setup_jobques,
};

#endif /* METAL_SIFIVE_LPDMA0 */
