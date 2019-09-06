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

    unsigned int intr_mask_en; //disable=0
    //fifo_depth=8
    //interrupt_polarity=1
    //interrupt_signals=0 (global interrupt output)
    //fifo_triggering is set as 3 only for one channel. Rest 3 are set to 0
};


struct metal_i2s_vtable 
{
	
	void (*init)(struct metal_i2s *i2s,struct metal_i2s_config *cfg);
	int (*master_tx)(struct metal_i2s *i2s,struct metal_i2s_config *cfg,int len,uint16_t *tx_buf);
	int (*slave_rx)(struct metal_i2s *i2s,struct metal_i2s_config *cfg,int len,uint16_t *rx_buf);
	int (*set_data_resolution)(struct metal_i2s *i2s,struct metal_i2s_config *cfg);
	int (*set_ws_length)(struct metal_i2s *i2s,struct metal_i2s_config *cfg);
	int (*set_sclk_gating)(struct metal_i2s *i2s,struct metal_i2s_config *cfg);
	int (*intr_mask)(struct metal_i2s *i2s,struct metal_i2s_config *cfg);
//	struct metal_interrupt* (*get_interrupt_controller)(struct metal_i2c *i2c);
//	int (*get_interrupt_id)(struct metal_i2c *i2c);

};


struct metal_i2s
{
	const struct metal_i2s_vtable *vtable;	
};


__inline__ void metal_i2s_init(struct metal_i2s *i2s,struct metal_i2s_config *cfg)
{
	i2s->vtable->init(i2s,cfg);
}

__inline__ int metal_i2s_master_tx(struct metal_i2s *i2s,struct metal_i2s_config *cfg,int len,uint16_t *tx_buf)
{
	return i2s->vtable->master_tx(i2s,cfg,len,tx_buf);
}

__inline__ int metal_i2s_slave_rx(struct metal_i2s *i2s,struct metal_i2s_config *cfg,int len,uint16_t *rx_buf)
{
	return i2s->vtable->slave_rx(i2s,cfg,len,rx_buf);
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

__inline__ int metal_i2s_intr_mask(struct metal_i2s *i2s,struct metal_i2s_config *cfg)
{
	return i2s->vtable->intr_mask(i2s,cfg);
}

//__inline__ struct metal_interrupt* metal_i2c_get_interrupt_controller(struct metal_i2c *i2c)
//{
//	return i2c->vtable->get_interrupt_controller(i2c);
//}

//__inline__ int metal_i2c_get_interrupt_id(struct metal_i2c *i2c)
//{
//	return i2c->vtable-> get_interrupt_id(i2c);
//}
struct metal_i2s *metal_i2s_get_master_device(unsigned int device_num);

struct metal_i2s *metal_i2s_get_slave_device(unsigned int device_num);

#endif