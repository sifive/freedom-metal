/* Copyright 2018 SiFive, Inc */
/* SPDX-License-Identifier: Apache-2.0 */

#include <metal/machine.h>
#include <metal/dma.h>


extern inline void metal_dma_channel_init(struct metal_dma *dma, unsigned int chanid, struct metal_dma_chan_config *config);
extern inline int metal_dma_channel_active(struct metal_dma *dma, unsigned int chan);
extern inline void metal_dma_channel_txfer_enable(struct metal_dma *dma, unsigned int chanid, struct metal_dma_chan_config *config);
extern inline int metal_dma_channel_txfer_status(struct metal_dma *dma, unsigned int chanid);
extern inline void metal_dma_channel_txfer_enableIT(struct metal_dma *dma, unsigned int chanid, struct metal_dma_chan_config *config);
extern struct metal_interrupt * metal_dma_interrupt_controller(struct metal_dma *dma);
extern int metal_dma_get_interrupt_id(struct metal_dma *dma, unsigned int idx);
extern inline int metal_dma_chan_cfg_interrupt(struct metal_dma *dma, unsigned int chanid, metal_dma_interrupt_t flag);
extern inline int metal_dma_chan_clr_interrupt(struct metal_dma *dma, unsigned int chanid, metal_dma_interrupt_t flag);



struct metal_dma *metal_dma_get_device(int device_num)
{
    if(device_num >= __METAL_DT_MAX_DMAS) {
        return NULL;
    }

    return (struct metal_dma *) __metal_dma_table[device_num];
}