/* Copyright 2020 SiFive, Inc */
/* SPDX-License-Identifier: Apache-2.0 */

#ifndef METAL__DMA_H
#define METAL__DMA_H

struct metal_dma;

union metal_jobctrl {
    struct {
        unsigned int reserved: 2;
        unsigned int repeat: 1;
        unsigned int order: 1;
        unsigned int reserved1: 20;
        unsigned int wsize: 4;
        unsigned int rsize: 4;
    } fields;
    unsigned int bits;
};

struct metal_dma_job {
    unsigned long num_bytes;
    unsigned long src_ptr;
    unsigned long dst_ptr;
    union metal_jobctrl jobctrl;
};

/*! @brief The configuration for a DMA transfer */
struct metal_dma_chan_config {
    unsigned long num_bytes;
    unsigned long src_ptr;
    unsigned long dst_ptr;
    union metal_jobctrl jobctrl;   
};

enum metal_dma_state {
    METAL_DMA_DISABLE = 0,
    METAL_DMA_ENABLE = !METAL_DMA_DISABLE
};

typedef enum {
    METAL_DMA_INT_DONE_ENABLE = 0x01,
    METAL_DMA_INT_ERR_ENABLE = 0x02,
} metal_dma_interrupt_t;

struct metal_dma_vtable {    
    int (*dma_enable)(struct metal_dma *dma, unsigned int chan, struct metal_dma_chan_config *config);
    int (*dma_disable)(struct metal_dma *dma, unsigned int chan);
    void (*chan_init)(struct metal_dma *dma, unsigned int chan, struct metal_dma_chan_config *config);
    int (*chan_active)(struct metal_dma *dma, unsigned int chan);
    void (*setup_jobques)(struct metal_dma *dma, unsigned int chan, struct metal_dma_chan_config *config);
    struct metal_interrupt *(*get_interrupt)(const struct metal_dma *const dma);
    int (*get_interrupt_id)(struct metal_dma *dma, unsigned int idx);
    void (*chan_txfer_enable)(struct metal_dma *dma, unsigned int chanid, struct metal_dma_chan_config *config);
    int  (*chan_txfer_status)(struct metal_dma *dma, unsigned int chanid);
    void (*chan_txfer_enableIT)(struct metal_dma *dma, unsigned int chanid, struct metal_dma_chan_config *config);
    struct metal_interrupt *(*get_interrupt_controller)(struct metal_dma *dma);
    int (*chan_cfg_interrupt)(struct metal_dma *dma, unsigned int chanid, metal_dma_interrupt_t flag);
    int (*chan_clr_interrupt)(struct metal_dma *dma, unsigned int chanid, metal_dma_interrupt_t flag);
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
inline int metal_dma_get_interrupt_id(struct metal_dma *dma, unsigned int idx) {
    return dma->vtable->get_interrupt_id(dma, idx);
}

inline void metal_dma_channel_txfer_enable(struct metal_dma *dma, unsigned int chanid, struct metal_dma_chan_config *config) {
    return dma->vtable->chan_txfer_enable(dma, chanid, config);
}

inline int metal_dma_channel_txfer_status(struct metal_dma *dma, unsigned int chanid) {
    return dma->vtable->chan_txfer_status(dma, chanid);
}

inline void metal_dma_channel_txfer_enableIT(struct metal_dma *dma, unsigned int chanid, struct metal_dma_chan_config *config) {
    return dma->vtable->chan_txfer_enableIT(dma, chanid, config);

}

inline struct metal_interrupt * metal_dma_interrupt_controller(struct metal_dma *dma) {
    return dma->vtable->get_interrupt_controller(dma);
}

inline int metal_dma_chan_cfg_interrupt(struct metal_dma *dma, unsigned int chanid, metal_dma_interrupt_t flag) {
return dma->vtable->chan_cfg_interrupt(dma, chanid, flag);
}

inline int metal_dma_chan_clr_interrupt(struct metal_dma *dma, unsigned int chanid, metal_dma_interrupt_t flag) {
return dma->vtable->chan_clr_interrupt(dma, chanid, flag);
}
#endif
