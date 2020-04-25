/* Copyright 2020 SiFive, Inc */
/* SPDX-License-Identifier: Apache-2.0 */

#ifndef METAL__DMA_H
#define METAL__DMA_H

struct metal_dma;

union jobcontrol {
    struct {
        unsigned int src_size: 3;
        unsigned int dst_size: 3;
        unsigned int src_inc: 1;
        unsigned int dst_inc: 1;
        unsigned int num_minus_1: 15;
        unsigned int rcount: 4;
        unsigned int done_ie: 1;
        unsigned int error_ie: 1;
        unsigned int trigger_mode: 1;
    } fields;
    unsigned int bits;
};

struct job {
    union jobcontrol control;
    unsigned int src_end_ptr;
    unsigned int dst_end_ptr;    
};

/*! @brief The configuration for a DMA transfer */
struct metal_dma_chan_config {
#if 0
    unsigned int *src_addr;
    unsigned int *dest_addr;
    unsigned int bufsize;
    unsigned int tx_size;


    /*! @brief The protocol for the DMA transfer */
    enum {
        METAL_DMA_NORMAL,
        METAL_DMA_CIRCULAR,
        METAL_DMA_LINKED_LIST
    } queue_type;
#endif
    enum {
        METAL_DMA_PRIORITY_LOW,
        METAL_DMA_PRIORITY_HIGH,
        METAL_DMA_PRIORITY_MEDIUM,
        METAL_DMA_PRIORITY_VERY_HIGH 
    } priority;

    //unsigned int src_inc : 1;
    //unsigned int dest_inc : 1;
    unsigned int hw_trig_en : 1;
    unsigned int sw_trig : 1;
    unsigned int job_cycle: 1;    
};

enum metal_dma_state {
    METAL_DMA_DISABLE = 0,
    METAL_DMA_ENABLE = !METAL_DMA_DISABLE
};

struct metal_dma_vtable {    
    int (*dma_enable)(struct metal_dma *dma, unsigned int chan, struct metal_dma_chan_config *config);
    int (*dma_disable)(struct metal_dma *dma, unsigned int chan);
    void (*chan_init)(struct metal_dma *dma, unsigned int chan, struct metal_dma_chan_config *config);
    int (*chan_active)(struct metal_dma *dma, unsigned int chan);
    void (*setup_jobques)(struct metal_dma *dma, unsigned int chan, struct metal_dma_chan_config *config);
    struct metal_interrupt *(*get_interrupt)(const struct metal_dma *const dma);
    int (*get_interrupt_id)(const struct metal_dma *const dma);
};


/*! @brief A handle for a DMA device */
struct metal_dma {
    const struct metal_dma_vtable *vtable;
    struct job **jobq;
    unsigned int jobq_depth;
    unsigned int channel_count;
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
inline void metal_dma_channel_init(struct metal_dma *dma, unsigned int metal_dma_channel, struct metal_dma_chan_config *config) {
    return dma->vtable->chan_init(dma, metal_dma_channel, config);

}

/*! @brief Enables the DMA transfter on a pirticular channel
 * @param dma                       The handle for the DMA device to initialize
 * @param metal_dma_channel         This is the channel number a peripheral of DMA want to use 
 * @param metal_dma_chan_config     This is the channel configurations to be used for the channel
 */
inline int metal_dma_enable(struct metal_dma *dma, unsigned int metal_dma_channel, struct metal_dma_chan_config *config) {
    return dma->vtable->dma_enable(dma, metal_dma_channel, config);
}

/*! @brief Disables the DMA transfter on a pirticular channel
 * @param dma                       The handle for the DMA device to initialize
 * @param metal_dma_channel         This is the channel number a peripheral of DMA want to use  
 */
inline int metal_dma_disable(struct metal_dma *dma, unsigned int metal_dma_channel) {
    return dma->vtable->dma_disable(dma, metal_dma_channel);
}

/*! @brief Disables the DMA transfter on a pirticular channel
 * @param dma                       The handle for the DMA device to initialize
 * @param metal_dma_channel         This is the channel number a peripheral of DMA want to use  
 * TODO: Is this API required??? need to see once the confirmed spec is released
 */
inline int metal_dma_channel_active(struct metal_dma *dma, unsigned int metal_dma_channel) {
    return dma->vtable->chan_active(dma, metal_dma_channel);
}

/*! @brief Setup the global jobqueues for all the DMA Channels
 * @param dma                       The handle for the DMA device to initialize  
 * @param metal_dma_chan_config     This is the channel configurations to be used for the channel
 */
inline void metal_dma_setup_jobques(struct metal_dma *dma, unsigned int metal_dma_channel, struct metal_dma_chan_config *config) {
    return dma->vtable->setup_jobques(dma, metal_dma_channel, config);
}

/*!
 * @brief Get the interrupt handle for the dma compare
 * @return The interrupt handle
 */
inline struct metal_interrupt *
metal_dma_get_interrupt(const struct metal_dma *const dma) {
    return dma->vtable->get_interrupt(dma);
}

/*!
 * @brief Get the interrupt ID for the dma compare
 * @return The interrupt ID
 */
inline int metal_dma_get_interrupt_id(const struct metal_dma *const dma) {
    return dma->vtable->get_interrupt_id(dma);
}

#endif
