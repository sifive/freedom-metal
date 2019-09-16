#ifndef METAL__I2C_H
#define METAL__I2C_H

struct metal_i2c;

struct metal_i2c_config
{
	
	enum {
			METAL_I2C_SLAVE=0,
			METAL_I2C_MASTER
		 } operation_mode;

	enum {
			METAL_I2C_STANDARD_SPEED = 1,
			METAL_I2C_FAST_SPEED = 2,
			METAL_I2C_HIGH_SPEED = 3
		 } speed_mode;

	enum {
			METAL_I2C_7_BIT = 0,
			METAL_I2C_10_BIT
		 } addressing;

	enum {	METAL_I2C_GENERAL_CALL,
			METAL_I2C_START_BYTE
		 }transmission_mode;

    unsigned int enable_special_bit;
    
	unsigned int target_address;	 

	unsigned int slave_address;

	unsigned int receiver_threshold;

	unsigned int transmitter_threshold;

	unsigned int issue_stop;

	unsigned int issue_restart;

	unsigned int low_count; // need to be automated using "clock" field in the device tree

	unsigned int high_count; // need to be automated using "clock" field in the device tree

	unsigned int mask_all_interrupts;
};



struct metal_i2c_vtable 
{
	void (*enable_i2c)(struct metal_i2c *i2c);
	void (*disable_i2c)(struct metal_i2c *i2c);
	void (*init)(struct metal_i2c *i2c,struct metal_i2c_config *cfg);
	int (*write_transfer)(struct metal_i2c *i2c,unsigned char *tx_buf);
	int (*read_transfer)(struct metal_i2c *i2c,struct metal_i2c_config *cfg,int len,unsigned char *rx_buf);
	int (*set_target_address)(struct metal_i2c *i2c,struct metal_i2c_config *cfg,unsigned int address);
	int (*set_speed_mode)(struct metal_i2c *i2c,struct metal_i2c_config *cfg);
	int (*set_address_mode)(struct metal_i2c *i2c,struct metal_i2c_config *cfg);
	struct metal_interrupt* (*get_interrupt_controller)(struct metal_i2c *i2c);
//	int (*get_interrupt_id)(struct metal_i2c *i2c);

};


struct metal_i2c
{
	const struct metal_i2c_vtable *vtable;	
};

__inline__ void metal_i2c_enable(struct metal_i2c *i2c)
{
	i2c->vtable->enable_i2c(i2c);
}

__inline__ void metal_i2c_disable(struct metal_i2c *i2c)
{
	i2c->vtable->disable_i2c(i2c);
}

__inline__ void metal_i2c_init(struct metal_i2c *i2c,struct metal_i2c_config *cfg)
{
	i2c->vtable->init(i2c,cfg);
}

__inline__ int metal_i2c_write_transfer(struct metal_i2c *i2c,unsigned char *tx_buf)
{
	return i2c->vtable->write_transfer(i2c,tx_buf);
}

__inline__ int metal_i2c_read_transfer(struct metal_i2c *i2c,struct metal_i2c_config *cfg,int len,unsigned char *rx_buf)
{
	return i2c->vtable->read_transfer(i2c,cfg,len,rx_buf);
}

__inline__ int metal_i2c_set_address_mode(struct metal_i2c *i2c,struct metal_i2c_config *cfg)
{
	return i2c->vtable->set_address_mode(i2c,cfg);
}

__inline__ int metal_i2c_set_speed_mode(struct metal_i2c *i2c,struct metal_i2c_config *cfg)
{
	return i2c->vtable->set_speed_mode(i2c,cfg);
} 

__inline__ int metal_i2c_set_target_address(struct metal_i2c *i2c,struct metal_i2c_config *cfg,unsigned int address)
{
	return i2c->vtable->set_target_address(i2c,cfg,address);
}

__inline__ struct metal_interrupt* metal_i2c_get_interrupt_controller(struct metal_i2c *i2c)
{
	return i2c->vtable->get_interrupt_controller(i2c);
}

//__inline__ int metal_i2c_get_interrupt_id(struct metal_i2c *i2c)
//{
//	return i2c->vtable-> get_interrupt_id(i2c);
//}



#endif