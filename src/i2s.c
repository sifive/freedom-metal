#include<metal/machine.h>
#include<metal/i2s.h>

extern __inline__ void metal_i2s_init(struct metal_i2s *i2s,struct metal_i2s_config *cfg);
extern __inline__ int metal_i2s_set_data_resolution(struct metal_i2s *i2s,struct metal_i2s_config *cfg);
extern __inline__ int metal_i2s_set_ws_length(struct metal_i2s *i2s,struct metal_i2s_config *cfg);
extern __inline__ int metal_i2s_set_sclk_gating(struct metal_i2s *i2s,struct metal_i2s_config *cfg);
extern __inline__ int metal_i2s_set_fifo_trigger_level(struct metal_i2s *i2s,struct metal_i2s_config *cfg);
extern __inline__ int metal_i2s_interrupt_enable(struct metal_i2s *i2s,struct metal_i2s_config *cfg,int interrupt_number);
extern __inline__ int metal_i2s_interrupt_disable(struct metal_i2s *i2s, struct metal_i2s_config *cfg,int interrupt_number);
extern __inline__ int metal_i2s_get_intr_id(struct metal_i2s *i2s, struct metal_i2s_config *cfg, int num);
extern __inline__ struct metal_interrupt* metal_i2s_get_interrupt_controller(struct metal_i2s *i2s,struct metal_i2s_config *cfg);
extern __inline__ int metal_i2s_get_intr_num(struct metal_i2s *i2s,struct metal_i2s_config *cfg);
extern __inline__ int metal_i2s_tx_enable(struct metal_i2s *i2s,struct metal_i2s_config *cfg);
extern __inline__ int metal_i2s_tx_disable(struct metal_i2s *i2s,struct metal_i2s_config *cfg);
extern __inline__ int metal_i2s_rx_enable(struct metal_i2s *i2s,struct metal_i2s_config *cfg);
extern __inline__ int metal_i2s_rx_disable(struct metal_i2s *i2s,struct metal_i2s_config *cfg);
extern __inline__ int metal_i2s_clock_enable(struct metal_i2s *i2s,struct metal_i2s_config *cfg);
extern __inline__ int metal_i2s_clock_disable(struct metal_i2s *i2s,struct metal_i2s_config *cfg);
extern __inline__ int metal_i2s_put_data(struct metal_i2s *i2s, struct metal_i2s_config *cfg,int len,uint32_t *tx_buf);
extern __inline__ int metal_i2s_get_data(struct metal_i2s *i2s, struct metal_i2s_config *cfg,int len,uint32_t *rx_buf);
extern __inline__ int metal_i2s_global_enable(struct metal_i2s *i2s,struct metal_i2s_config *cfg);
extern __inline__ int metal_i2s_global_disable(struct metal_i2s *i2s,struct metal_i2s_config *cfg);
extern __inline__ void metal_i2s_load_buffer(struct metal_i2s *i2s,uint32_t *tx_buf,int data,unsigned char *buffer_len);
extern __inline__ void metal_i2s_global_interrupt_clear(struct metal_i2s *i2s,struct metal_i2s_config *cfg,int interrupt_number);
extern __inline__ void metal_i2s_tx_fifo_flush (struct metal_i2s *i2s, struct metal_i2s_config *cfg);
extern __inline__ void metal_i2s_rx_fifo_flush(struct metal_i2s *i2s, struct metal_i2s_config *cfg);
//extern __inline__ int metal_i2s_get_interrupt_id(struct metal_i2s *i2s);

struct metal_i2s *metal_i2s_get_master_device(unsigned int device_num)
{
	#if __METAL_DT_MAX_I2S_MASTER > 0
    if (device_num < __METAL_DT_MAX_I2S_MASTER) {
        return (struct metal_i2s *) __metal_i2s_master_table[device_num];
    }
#endif

    return NULL;
}

struct metal_i2s *metal_i2s_get_slave_device(unsigned int device_num)
{
	#if __METAL_DT_MAX_I2S_SLAVE > 0
    if (device_num < __METAL_DT_MAX_I2S_SLAVE) {
        return (struct metal_i2s *) __metal_i2s_slave_table[device_num];
    }
#endif

    return NULL;
}
