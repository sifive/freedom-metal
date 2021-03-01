/* Copyright 2021 SiFive, Inc */
/* SPDX-License-Identifier: Apache-2.0 */

#include <metal/pdma.h>
#include <metal/machine.h>

extern inline void metal_pdma_channel_init(struct metal_pdma *dma,
                                          unsigned int chanid,
                                          struct metal_pdma_chan_config *config);
extern inline int metal_pdma_channel_active(struct metal_pdma *dma,
                                           unsigned int chan);
extern inline void
metal_pdma_channel_txfer_enable(struct metal_pdma *dma, unsigned int chanid,
                               struct metal_pdma_chan_config *config);
extern inline int metal_pdma_channel_txfer_status(struct metal_pdma *dma,
                                                 unsigned int chanid);
extern inline void
metal_pdma_channel_txfer_enableIT(struct metal_pdma *dma, unsigned int chanid,
                                 struct metal_pdma_chan_config *config);
extern struct metal_interrupt *
metal_pdma_interrupt_controller(struct metal_pdma *dma);
extern int metal_pdma_get_interrupt_id(struct metal_pdma *dma, unsigned int idx);
extern inline int metal_pdma_chan_cfg_interrupt(struct metal_pdma *dma,
                                               unsigned int chanid,
                                               metal_pdma_interrupt_t flag);
extern inline int metal_pdma_chan_clr_interrupt(struct metal_pdma *dma,
                                               unsigned int chanid,
                                               metal_pdma_interrupt_t flag);

struct metal_pdma *metal_pdma_get_device(int device_num) {
    if (device_num >= __METAL_DT_MAX_DMAS) {
        return NULL;
    }

    return (struct metal_pdma *)__metal_pdma_table[device_num];
}
