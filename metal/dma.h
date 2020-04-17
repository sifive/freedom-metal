/* Copyright 2020 SiFive, Inc */
/* SPDX-License-Identifier: Apache-2.0 */

#ifndef METAL__DMA_H
#define METAL__DMA_H

struct metal_dma;

union jobcontrol {
    struct {
        int src_size: 3;
        int dst_size: 3;
        int src_inc: 1;
        int dst_inc: 1;
        int num_minus_1: 15;
        int rcount: 4;
        int done_ie: 1;
        int error_ie: 1;
        int trigger_mode: 1;
    } fields;
    unsigned int bits;
};

struct job {
    union jobcontrol control;
    unsigned int *src_end_ptr;
    unsigned int *dst_end_ptr;
    struct job *next_job;
};

/*! @brief The configuration for a DMA transfer */
struct metal_dma_chan_config {
    unsigned int *src_addr;
    unsigned int *dest_addr;
    struct job *jobq;
    unsigned int bufsize;
    unsigned int tx_size;

    /*! @brief The protocol for the DMA transfer */
    enum {
        METAL_DMA_NORMAL,
        METAL_DMA_CIRCULAR,
        METAL_DMA_LINKED_LIST
    } queue_type;

    enum {
        METAL_DMA_PRIORITY_VERY_HIGH,
        METAL_DMA_PRIORITY_HIGH,
        METAL_DMA_PRIORITY_MEDIUM,
        METAL_DMA_PRIORITY_LOW
    } priority;

    unsigned int src_inc : 1;
    unsigned int dest_inc : 1;
    unsigned int hw_trig_en : 1;
    unsigned int sw_trig : 1;
    unsigned int job_cycle: 1;    
};

enum metal_dma_state {
    METAL_DMA_DISABLE = 0,
    METAL_DMA_ENABLE = !METAL_DMA_DISABLE
};

struct metal_dma_vtable {
    void (*init)(struct metal_dma *dma, unsigned int chan, struct metal_dma_chan_config *config);
    int (*enable_dma)(struct metal_dma *dma, unsigned int chan, struct metal_dma_chan_config *config);
    int (*disable_dma)(struct metal_dma *dma, unsigned int chan);
    int (*channel_active)(struct metal_dma *dma, unsigned int chan);
    void (*setup_jobques)(struct metal_dma *dma, struct metal_dma_chan_config *config);
};


/*! @brief A handle for a DMA device */
struct metal_dma {
    const struct metal_dma_vtable *vtable;
};

/*! @brief Get a handle for a DMA device
 * @param device_num The index of the desired DMA device
 * @return A handle to the DMA device, or NULL if the device does not exist*/
struct metal_dma *metal_dma_get_device(int device_num);

/*! @brief Initialize a DMA Channel with Channel configurations 
 * @param dma                       The handle for the DMA device to initialize
 * @param metal_dma_channel         This is the channel number a peripheral of DMA want to use 
 * @param metal_dma_chan_config     This is the channel configurations to be used for the channel
 */
inline void metal_dma_init(struct metal_dma *dma, unsigned int metal_dma_channel, struct metal_dma_chan_config *config) {
    return dma->vtable->init(dma, metal_dma_channel, config);

}

/*! @brief Enables the DMA transfter on a pirticular channel
 * @param dma                       The handle for the DMA device to initialize
 * @param metal_dma_channel         This is the channel number a peripheral of DMA want to use 
 * @param metal_dma_chan_config     This is the channel configurations to be used for the channel
 */
inline int metal_enable_dma(struct metal_dma *dma, unsigned int metal_dma_channel, struct metal_dma_chan_config *config) {
    return dma->vtable->enable_dma(dma, metal_dma_channel, config);
}

/*! @brief Disables the DMA transfter on a pirticular channel
 * @param dma                       The handle for the DMA device to initialize
 * @param metal_dma_channel         This is the channel number a peripheral of DMA want to use  
 */
inline int metal_disable_dma(struct metal_dma *dma, unsigned int metal_dma_channel) {
    return dma->vtable->disable_dma(dma, metal_dma_channel);
}

/*! @brief Disables the DMA transfter on a pirticular channel
 * @param dma                       The handle for the DMA device to initialize
 * @param metal_dma_channel         This is the channel number a peripheral of DMA want to use  
 * TODO: Is this API required??? need to see once the confirmed spec is released
 */
inline int metal_channel_active(struct metal_dma *dma, unsigned int metal_dma_channel) {
    return dma->vtable->channel_active(dma, metal_dma_channel);
}

/*! @brief Setup the global jobqueues for all the DMA Channels
 * @param dma                       The handle for the DMA device to initialize  
 * @param metal_dma_chan_config     This is the channel configurations to be used for the channel
 */
inline void metal_dma_setup_jobques(struct metal_dma *dma, struct metal_dma_chan_config *config) {
    return dma->vtable->setup_jobques(dma, config);
}

#endif
