#ifndef METAL_I2S_H
#define METAL_I2S_H

struct metal_i2s;

struct metal_i2s_config
{
//setting as a master or slave is not s/w configurable
//Fifo_depth is also not s/w configurable

	enum {
		   DATA_RESOLUTION_IGNORE=0, //programmed data_resolution should be less than or equal to I2S_RX_WORDSIZE_x / I2S_TX_WORDSIZE_x
		   DATA_RESOLUTION_12=1,     //channel must be disabled before making any changes to the data_resolution
		   DATA_RESOLUTION_16=2,
		   DATA_RESOLUTION_20=3,
		   DATA_RESOLUTION_24=4,
		   DATA_RESOLUTION_32=5 
	}data_resolution;

	enum {
		   WS_LENGTH_16=0, //clock enable register CER[0] must be disabled prior making any changes to CCR register
		   WS_LENGTH_24=1,
		   WS_LENGTH_32=2
	}ws_length;

	enum {
		   SCLK_GATING_0=0, //clock enable register CER[0] must be disabled prior making any changes to CCR register
		   SCLK_GATING_12=1,
		   SCLK_GATING_16=2,
		   SCLK_GATING_20=3,
		   SCLK_GATING_24=4,
	}sclk_gating;

    unsigned int master;

    unsigned int tx_fifo_trigger_level; //Trigger level =TXCHET

    unsigned int rx_fifo_trigger_level; //Trigger level=Programmed value+1
    
    unsigned int no_of_channels; 

    unsigned int tx_block_en; //disable= 0

    unsigned int rx_block_en; //disable= 0

    unsigned int intr_unmask_en; //disable=0
    //fifo_depth=8
    //interrupt_polarity=1
    //interrupt_signals=0 (global interrupt output)
    //fifo_triggering is set as 3 only for one channel. Rest 3 are set to 0
};


struct metal_i2s_vtable 
{
	
	void (*init)(struct metal_i2s *i2s,struct metal_i2s_config *cfg);
	int (*set_data_resolution)(struct metal_i2s *i2s,struct metal_i2s_config *cfg);
	int (*set_ws_length)(struct metal_i2s *i2s,struct metal_i2s_config *cfg);
	int (*set_sclk_gating)(struct metal_i2s *i2s,struct metal_i2s_config *cfg);
	int (*set_fifo_trigger_level)(struct metal_i2s *i2s,struct metal_i2s_config *cfg);
	int (*interrupt_enable) (struct metal_i2s *i2s,struct metal_i2s_config *cfg, int interrupt_number);
	int (*interrupt_disable) (struct metal_i2s *i2s,struct metal_i2s_config *cfg, int interrupt_number);
	int (*get_intr_id) (struct metal_i2s *i2s,struct metal_i2s_config *cfg, int num);
	struct metal_interrupt* (*get_interrupt_controller)(struct metal_i2s *i2s, struct metal_i2s_config *cfg);
	int (*get_intr_num) (struct metal_i2s *i2s,struct metal_i2s_config *cfg);
	int (*tx_enable) (struct metal_i2s *i2s,struct metal_i2s_config *cfg);
	int (*tx_disable) (struct metal_i2s *i2s,struct metal_i2s_config *cfg);
	int (*rx_enable) (struct metal_i2s *i2s,struct metal_i2s_config *cfg);
	int (*rx_disable) (struct metal_i2s *i2s,struct metal_i2s_config *cfg);
	int (*clock_enable) (struct metal_i2s *i2s,struct metal_i2s_config *cfg);
	int (*clock_disable) (struct metal_i2s *i2s,struct metal_i2s_config *cfg);
	int (*put_data) (struct metal_i2s *i2s,struct metal_i2s_config *cfg,int len,uint32_t *tx_buf);
	int (*get_data) (struct metal_i2s *i2s,struct metal_i2s_config *cfg,int len,uint32_t *rx_buf);
	int (*global_enable) (struct metal_i2s *i2s,struct metal_i2s_config *cfg);
	int (*global_disable) (struct metal_i2s *i2s,struct metal_i2s_config *cfg);
	void (*load_buffer) (struct metal_i2s *i2s,uint32_t *tx_buf,int data,unsigned char *buffer_len);
	void (*global_interrupt_clear) (struct metal_i2s *i2s,struct metal_i2s_config *cfg,int interrupt_number);
	void (*tx_fifo_flush) (struct metal_i2s *i2s, struct metal_i2s_config *cfg);
	void (*rx_fifo_flush) (struct metal_i2s *i2s, struct metal_i2s_config *cfg);
};


struct metal_i2s
{
	const struct metal_i2s_vtable *vtable;	
};


__inline__ void metal_i2s_init(struct metal_i2s *i2s,struct metal_i2s_config *cfg)
{
	i2s->vtable->init(i2s,cfg);
}

__inline__ int metal_i2s_set_data_resolution(struct metal_i2s *i2s,struct metal_i2s_config *cfg)
{
	return i2s->vtable->set_data_resolution(i2s,cfg);
}

__inline__ int metal_i2s_set_ws_length(struct metal_i2s *i2s,struct metal_i2s_config *cfg)
{
	return i2s->vtable->set_ws_length(i2s,cfg);
} 

__inline__ int metal_i2s_set_sclk_gating(struct metal_i2s *i2s,struct metal_i2s_config *cfg)
{
	return i2s->vtable->set_sclk_gating(i2s,cfg);
}

__inline__ int metal_i2s_interrupt_enable(struct metal_i2s *i2s,struct metal_i2s_config *cfg,int interrupt_number)
{
	return i2s->vtable->interrupt_enable(i2s,cfg,interrupt_number);
}

__inline__ int metal_i2s_interrupt_disable(struct metal_i2s *i2s,struct metal_i2s_config *cfg,int interrupt_number)
{
	return i2s->vtable->interrupt_disable(i2s,cfg,interrupt_number);
}

__inline__ int metal_i2s_get_intr_id(struct metal_i2s *i2s,struct metal_i2s_config *cfg, int num)
{
	return i2s->vtable->get_intr_id(i2s,cfg,num);
}

__inline__ int metal_i2s_set_fifo_trigger_level(struct metal_i2s *i2s,struct metal_i2s_config *cfg)
{
	return i2s->vtable->set_fifo_trigger_level(i2s,cfg);
}

__inline__ struct metal_interrupt* metal_i2s_get_interrupt_controller(struct metal_i2s *i2s,struct metal_i2s_config *cfg)
{
	return i2s->vtable->get_interrupt_controller(i2s,cfg);
}

__inline__ int metal_i2s_get_intr_num(struct metal_i2s *i2s,struct metal_i2s_config *cfg)
{
	return i2s->vtable->get_intr_num(i2s,cfg);
}

//
__inline__ int metal_i2s_tx_enable(struct metal_i2s *i2s,struct metal_i2s_config *cfg)
{
	return i2s->vtable->tx_enable(i2s,cfg);
}

__inline__ int metal_i2s_tx_disable(struct metal_i2s *i2s,struct metal_i2s_config *cfg)
{
	return i2s->vtable->tx_disable(i2s,cfg);
}

__inline__ int metal_i2s_rx_enable(struct metal_i2s *i2s,struct metal_i2s_config *cfg)
{
	return i2s->vtable->rx_enable(i2s,cfg);
}

__inline__ int metal_i2s_rx_disable(struct metal_i2s *i2s,struct metal_i2s_config *cfg)
{
	return i2s->vtable->rx_disable(i2s,cfg);
}

__inline__ int metal_i2s_clock_enable(struct metal_i2s *i2s,struct metal_i2s_config *cfg)
{
	return i2s->vtable->clock_enable(i2s,cfg);
}

__inline__ int metal_i2s_clock_disable(struct metal_i2s *i2s,struct metal_i2s_config *cfg)
{
	return i2s->vtable->clock_disable(i2s,cfg);
}

__inline__ int metal_i2s_put_data(struct metal_i2s *i2s,struct metal_i2s_config *cfg,int len,uint32_t *tx_buf)
{
	return i2s->vtable->put_data(i2s,cfg,len,tx_buf);
}

__inline__ int metal_i2s_get_data(struct metal_i2s *i2s,struct metal_i2s_config *cfg,int len,uint32_t *rx_buf)
{
	return i2s->vtable->get_data(i2s,cfg,len,rx_buf);
}

__inline__ int metal_i2s_global_enable(struct metal_i2s *i2s,struct metal_i2s_config *cfg)
{
	return i2s->vtable->global_enable(i2s,cfg);
}

__inline__ int metal_i2s_global_disable(struct metal_i2s *i2s,struct metal_i2s_config *cfg)
{
	return i2s->vtable->global_disable(i2s,cfg);
}

__inline__ void metal_i2s_load_buffer(struct metal_i2s *i2s,uint32_t *tx_buf,int data,unsigned char *buffer_len)
{
	return i2s->vtable->load_buffer(i2s,tx_buf,data,buffer_len);
}

__inline__ void metal_i2s_global_interrupt_clear(struct metal_i2s *i2s,struct metal_i2s_config *cfg,int interrupt_number)
{
	return i2s->vtable->global_interrupt_clear(i2s,cfg,interrupt_number);
}

__inline__ void metal_i2s_tx_fifo_flush(struct metal_i2s *i2s, struct metal_i2s_config *cfg)
{
	return i2s->vtable->tx_fifo_flush(i2s,cfg);
}

__inline__ void metal_i2s_rx_fifo_flush(struct metal_i2s *i2s, struct metal_i2s_config *cfg)
{
	return i2s->vtable->rx_fifo_flush(i2s,cfg);
}

struct metal_i2s *metal_i2s_get_master_device(unsigned int device_num);

struct metal_i2s *metal_i2s_get_slave_device(unsigned int device_num);

#endif
