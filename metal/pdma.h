/* Copyright 2021 SiFive, Inc */
/* SPDX-License-Identifier: Apache-2.0 */

#ifndef METAL__PDMA_H
#define METAL__PDMA_H

struct metal_pdma;

/*! @brief Channels data transfer config parameters */
union metal_jobctrl {
    struct {
        unsigned int reserved : 2;
        unsigned int repeat : 1;
        unsigned int order : 1;
        unsigned int reserved1 : 20;
        unsigned int wsize : 4;
        unsigned int rsize : 4;
    } fields;
    unsigned int bits;
};

/*! @brief The configuration for a DMA transfer */
struct metal_pdma_chan_config {
    unsigned long num_bytes;
    unsigned long src_ptr;
    unsigned long dst_ptr;
    union metal_jobctrl jobctrl;
};

/*! @brief Channels data transfer config parameters */
enum metal_pdma_state {
    METAL_PDMA_DISABLE = 0,
    METAL_PDMA_ENABLE = !METAL_PDMA_DISABLE
};

/*! @brief Enums for Channel Interrupts */
typedef enum {
    METAL_PDMA_INT_DONE_ENABLE = 0x01,
    METAL_PDMA_INT_ERR_ENABLE = 0x02,
} metal_pdma_interrupt_t;

/*! @brief vtable for  DMA  */
struct metal_pdma_vtable {
    void (*chan_init)(struct metal_pdma *dma, unsigned int chan,
                      struct metal_pdma_chan_config *config);
    int (*chan_active)(struct metal_pdma *dma, unsigned int chan);
    int (*get_interrupt_id)(struct metal_pdma *dma, unsigned int idx);
    void (*chan_txfer_enable)(struct metal_pdma *dma, unsigned int chanid,
                              struct metal_pdma_chan_config *config);
    int (*chan_txfer_status)(struct metal_pdma *dma, unsigned int chanid);
    void (*chan_txfer_enableIT)(struct metal_pdma *dma, unsigned int chanid,
                                struct metal_pdma_chan_config *config);
    struct metal_interrupt *(*get_interrupt_controller)(struct metal_pdma *dma);
    int (*chan_cfg_interrupt)(struct metal_pdma *dma, unsigned int chanid,
                              metal_pdma_interrupt_t flag);
    int (*chan_clr_interrupt)(struct metal_pdma *dma, unsigned int chanid,
                              metal_pdma_interrupt_t flag);
};

/*! @brief A handle for a DMA device */
struct metal_pdma {
    const struct metal_pdma_vtable *vtable;
};

/*! @brief Get a handle for a DMA device
 * @param device_num The index of the desired DMA device
 * @return A handle to the DMA device, or NULL if the device does not exist*/
struct metal_pdma *metal_pdma_get_device(int device_num);

/*! @brief Initialize a DMA Channel with Channel configurations
 * @param dma                       The handle for the DMA device to initialize
 * @param metal_pdma_channel         This is the channel number a peripheral of
 * DMA want to use
 * @param metal_pdma_chan_config     This is the channel configurations to be
 * used for the channel
 */
inline void metal_pdma_channel_init(struct metal_pdma *dma,
                                   unsigned int metal_pdma_channel,
                                   struct metal_pdma_chan_config *config) {
    return dma->vtable->chan_init(dma, metal_pdma_channel, config);
}

/*! @brief Disables the DMA transfter on a pirticular channel
 * @param dma                       The handle for the DMA device to initialize
 * @param metal_pdma_channel         This is the channel number a peripheral of
 * DMA want to use
 * TODO: Is this API required??? need to see once the confirmed spec is released
 */
inline int metal_pdma_channel_active(struct metal_pdma *dma,
                                    unsigned int metal_pdma_channel) {
    return dma->vtable->chan_active(dma, metal_pdma_channel);
}

/*!
 * @brief Get the interrupt ID of the DMA channel.
 * @param dma DMA device handle
 * @idx   channel id of the DMA device
 * @return The interrupt ID
 */
inline int metal_pdma_get_interrupt_id(struct metal_pdma *dma, unsigned int idx) {
    return dma->vtable->get_interrupt_id(dma, idx);
}

/*!
 * @brief Enables the transfer on a pirticular DMA channel, with a pirticular
 * channel configuration.
 * @param dma DMA device handle.
 * @param config DMA channel configuration.
 */
inline void
metal_pdma_channel_txfer_enable(struct metal_pdma *dma, unsigned int chanid,
                               struct metal_pdma_chan_config *config) {
    return dma->vtable->chan_txfer_enable(dma, chanid, config);
}

/*! @brief Get the data transfer status of DMA channel.
 * @param dma DMA device handle.
 * @param chanid DMA channel id.
 * @return 0 If no error.*/
inline int metal_pdma_channel_txfer_status(struct metal_pdma *dma,
                                          unsigned int chanid) {
    return dma->vtable->chan_txfer_status(dma, chanid);
}

/*! @brief Sets frequency in Hz for a given PWM instance.
 * @param dma DMA device handle.
 * @param chanid DMA channel id.
 * @param freq PWM frequency in Hz.
 * @return 0 If no error.*/
inline void
metal_pdma_channel_txfer_enableIT(struct metal_pdma *dma, unsigned int chanid,
                                 struct metal_pdma_chan_config *config) {
    return dma->vtable->chan_txfer_enableIT(dma, chanid, config);
}

/*! @brief Get the interrupt controller of the DMA peripheral.
 * The interrupt controller must be initialized before any interrupts can be
 * registered or enabled with it.
 * @param dma DMA device handle.
 * @return The handle for the DMA interrupt controller.*/
inline struct metal_interrupt *
metal_pdma_interrupt_controller(struct metal_pdma *dma) {
    return dma->vtable->get_interrupt_controller(dma);
}

/*! @brief Enable or Disable DMA Channel Interrupts.
 * @param dma DMA device handle.
 * @param chanid DMA channel id.
 * @param flag DMA interrupt flags.
 * @return 0 If no error.*/
inline int metal_pdma_chan_cfg_interrupt(struct metal_pdma *dma,
                                        unsigned int chanid,
                                        metal_pdma_interrupt_t flag) {
    return dma->vtable->chan_cfg_interrupt(dma, chanid, flag);
}

/*! @brief Clears pending interrupt flags.
 * @param dma DMA device handle.
 * @param chanid DMA channel id.
 * @param flag DMA interrupt flag.
 * @return 0 If no error.*/
inline int metal_pdma_chan_clr_interrupt(struct metal_pdma *dma,
                                        unsigned int chanid,
                                        metal_pdma_interrupt_t flag) {
    return dma->vtable->chan_clr_interrupt(dma, chanid, flag);
}

#endif
