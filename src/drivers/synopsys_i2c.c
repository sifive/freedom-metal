
#include <metal/machine/platform.h>

#ifdef METAL_SYNOPSYS_I2C_V2_02A_STANDARD

#include <metal/drivers/synopsys_i2c.h>
#include <metal/io.h>
#include <metal/machine.h>


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#define REGISTER_DATATYPE uint32_t
#define INTERFACE_SIZE 8
#define REGISTER_SIZE 32
#define METAL_I2C_STANDARD_MODE 0X1
#define METAL_I2C_FAST_SPEED  0x2
#define METAL_I2C_HIGH_SPEED 0x3



#define I2C_REG(offset) ((unsigned long)((__metal_driver_synopsys_i2c_control_base(i2c)) + offset))
#define I2C_REGB(offset) (__METAL_ACCESS_ONCE((REGISTER_DATATYPE *)I2C_REG(offset)))
#define I2C_REG_RW_BIT(offset,bit,val) (write_register(i2c,offset,set_bit(read_register(i2c,offset),bit,val)))
#define I2C_REG_RW_BIT_RANGE(offset,start_bit,stop_bit,val) (write_register(i2c,offset,set_bit_range(read_register(i2c,offset),start_bit,stop_bit,val)))

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



REGISTER_DATATYPE split_lsb(REGISTER_DATATYPE value, int bit)
{
	return (value & ((1<<bit)-1)); 

	//// get the bit from which the value needs to be splitted 
	//eg : value == 0100 bit = 2 >  .. we need  bit 0 and bit 1 value as output ie. 00
		// (1<<bit)-1  will return 4-1 ie. 3 ie. 0011
		//0100 & 011 will return 00 
					
}

REGISTER_DATATYPE get_new_msb(REGISTER_DATATYPE value,int bit,REGISTER_DATATYPE val)
{

	return (((value >> (bit+1))<<1) | val);
}

REGISTER_DATATYPE set_bit(REGISTER_DATATYPE value, int bit, REGISTER_DATATYPE val)
{
	return (((get_new_msb(value,bit,val))<<bit) | (split_lsb(value,bit)));
}

REGISTER_DATATYPE set_bit_range(REGISTER_DATATYPE value,int start_bit,int end_bit,REGISTER_DATATYPE val)
{
	 return ((value >> (end_bit+1))<<(end_bit+1))|((val<<start_bit) | (split_lsb(value,start_bit)));
}

void write_register(struct metal_i2c *i2c,REGISTER_DATATYPE offset,REGISTER_DATATYPE value)
{
	I2C_REGB(offset) = value;
}	

REGISTER_DATATYPE read_register(struct metal_i2c *i2c,REGISTER_DATATYPE offset)
{
	return I2C_REGB(offset);
}

REGISTER_DATATYPE read_register_bit(struct metal_i2c *i2c,REGISTER_DATATYPE offset,int bit)
{
	return ((read_register(i2c,offset) & (1<<(bit)))>>bit);
}

REGISTER_DATATYPE read_register_bit_range(struct metal_i2c *i2c,REGISTER_DATATYPE offset,int start_bit,int stop_bit)
{
	return ((read_register(i2c,offset) & (((1<<(stop_bit-start_bit+1))-1)<<start_bit))>>start_bit);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void __metal_driver_synopsys_i2c_enable_i2c(struct metal_i2c *i2c)
{
	I2C_REG_RW_BIT(METAL_SYNOPSYS_I2C_V2_02A_STANDARD_IC_ENABLE,0,1);

}

void __metal_driver_synopsys_i2c_disable_i2c(struct metal_i2c *i2c)
{	
			while(read_register_bit(i2c,METAL_SYNOPSYS_I2C_V2_02A_STANDARD_IC_RAW_INTR_STAT,8))
					
					{
						read_register(i2c,METAL_SYNOPSYS_I2C_V2_02A_STANDARD_IC_CLR_ACTIVITY);	
					}

			I2C_REG_RW_BIT(METAL_SYNOPSYS_I2C_V2_02A_STANDARD_IC_ENABLE,0,0);
			
}

int __metal_driver_synopsys_i2c_set_speed_mode(struct metal_i2c *i2c,struct metal_i2c_config *cfg)
{	
		I2C_REG_RW_BIT_RANGE(METAL_SYNOPSYS_I2C_V2_02A_STANDARD_IC_CON,1,2,cfg->speed_mode);
}		

int __metal_driver_synopsys_i2c_set_address_mode(struct metal_i2c *i2c,struct metal_i2c_config *cfg)
{
			I2C_REG_RW_BIT(METAL_SYNOPSYS_I2C_V2_02A_STANDARD_IC_CON,3,cfg->addressing);
			I2C_REG_RW_BIT(METAL_SYNOPSYS_I2C_V2_02A_STANDARD_IC_CON,4,cfg->addressing);		
}		

int __metal_driver_synopsys_i2c_set_target_address(struct metal_i2c *i2c,struct metal_i2c_config *cfg,unsigned int address)
{
	write_register((struct metal_i2c *)(i2c),METAL_SYNOPSYS_I2C_V2_02A_STANDARD_IC_TAR,set_bit(set_bit((REGISTER_DATATYPE)(address & 0x3ff),10,cfg->transmission_mode),11,cfg->enable_special_bit));
}

void __metal_driver_synopsys_i2c_init(struct metal_i2c *i2c,struct metal_i2c_config *cfg)
{

		I2C_REG_RW_BIT(METAL_SYNOPSYS_I2C_V2_02A_STANDARD_IC_ENABLE,0,0);


		if(cfg->operation_mode == METAL_I2C_MASTER)
			{
				I2C_REG_RW_BIT(METAL_SYNOPSYS_I2C_V2_02A_STANDARD_IC_CON,0,1);
				I2C_REG_RW_BIT(METAL_SYNOPSYS_I2C_V2_02A_STANDARD_IC_CON,6,1);
				write_register((struct metal_i2c *)(i2c),METAL_SYNOPSYS_I2C_V2_02A_STANDARD_IC_TAR,cfg->target_address);
			}	
		else if(cfg->operation_mode == METAL_I2C_SLAVE)
			{
				I2C_REG_RW_BIT(METAL_SYNOPSYS_I2C_V2_02A_STANDARD_IC_CON,0,0);
				I2C_REG_RW_BIT(METAL_SYNOPSYS_I2C_V2_02A_STANDARD_IC_CON,6,0);
				write_register((struct metal_i2c *)(i2c),METAL_SYNOPSYS_I2C_V2_02A_STANDARD_IC_SAR,cfg->slave_address);
			}		

		I2C_REG_RW_BIT(METAL_SYNOPSYS_I2C_V2_02A_STANDARD_IC_CON,5,1);

		if(cfg->speed_mode == METAL_I2C_STANDARD_SPEED){
		
			write_register((struct metal_i2c *)(i2c),METAL_SYNOPSYS_I2C_V2_02A_STANDARD_IC_SS_SCL_LCNT,cfg->low_count);
			write_register((struct metal_i2c *)(i2c),METAL_SYNOPSYS_I2C_V2_02A_STANDARD_IC_SS_SCL_HCNT,cfg->high_count);
		}
		else if(cfg->speed_mode == METAL_I2C_FAST_SPEED)
		{
			write_register((struct metal_i2c *)(i2c),METAL_SYNOPSYS_I2C_V2_02A_STANDARD_IC_FS_SCL_LCNT,cfg->low_count);
			write_register((struct metal_i2c *)(i2c),METAL_SYNOPSYS_I2C_V2_02A_STANDARD_IC_FS_SCL_HCNT,cfg->high_count);
		}
		else if(cfg->speed_mode == METAL_I2C_HIGH_SPEED)
		{
			write_register((struct metal_i2c *)(i2c),METAL_SYNOPSYS_I2C_V2_02A_STANDARD_IC_HS_SCL_LCNT,cfg->low_count);
			write_register((struct metal_i2c *)(i2c),METAL_SYNOPSYS_I2C_V2_02A_STANDARD_IC_HS_SCL_HCNT,cfg->high_count);	
		}
		if(cfg->mask_all_interrupts)
		{
			write_register((struct metal_i2c *)(i2c),METAL_SYNOPSYS_I2C_V2_02A_STANDARD_IC_INTR_MASK,0X0000);
		}
		else
		{
			write_register((struct metal_i2c *)(i2c),METAL_SYNOPSYS_I2C_V2_02A_STANDARD_IC_INTR_MASK,0XFFFF);
		}
	

		write_register((struct metal_i2c *)(i2c),METAL_SYNOPSYS_I2C_V2_02A_STANDARD_IC_TX_TL,cfg->transmitter_threshold);
		write_register((struct metal_i2c *)(i2c),METAL_SYNOPSYS_I2C_V2_02A_STANDARD_IC_RX_TL,cfg->receiver_threshold);


	__metal_driver_synopsys_i2c_set_address_mode((struct metal_i2c *)(i2c),cfg);			
	__metal_driver_synopsys_i2c_set_target_address((struct metal_i2c *)(i2c),cfg,cfg->target_address);	
		
    __metal_driver_synopsys_i2c_set_speed_mode((struct metal_i2c *)(i2c),cfg);

}

int __metal_driver_synopsys_i2c_write_transfer(struct metal_i2c *i2c,unsigned char *tx_buf)
{
		write_register((struct metal_i2c *)(i2c),METAL_SYNOPSYS_I2C_V2_02A_STANDARD_IC_DATA_CMD,set_bit(tx_buf,8,0));
}

int __metal_driver_synopsys_i2c_read_transfer(struct metal_i2c *i2c,struct metal_i2c_config *cfg,int len,unsigned char *rx_buf)
{	

	if(cfg->operation_mode == METAL_I2C_MASTER)
	{
		for(int i=0;i<len;i++)
				{
					write_register((struct metal_i2c *)(i2c),METAL_SYNOPSYS_I2C_V2_02A_STANDARD_IC_DATA_CMD,set_bit(0x0,8,1));
				}
		for(int i =0;i<len; i++)
				{
					while(!(read_register_bit((struct metal_i2c *)(i2c),METAL_SYNOPSYS_I2C_V2_02A_STANDARD_IC_RAW_INTR_STAT,2)))
					{}

					rx_buf[i] = (unsigned char)read_register(i2c,METAL_SYNOPSYS_I2C_V2_02A_STANDARD_IC_DATA_CMD);

				}		
				
	}

	else if(cfg->operation_mode == METAL_I2C_SLAVE)
		{
						
			while(read_register_bit(i2c,METAL_SYNOPSYS_I2C_V2_02A_STANDARD_IC_STATUS,3))
			{
				rx_buf[len++] = (unsigned char)read_register(i2c,METAL_SYNOPSYS_I2C_V2_02A_STANDARD_IC_DATA_CMD);
			}
			return len;

		}	   
}

__METAL_DEFINE_VTABLE(__metal_driver_vtable_synopsys_i2c) = {
	.i2c.enable_i2c         = __metal_driver_synopsys_i2c_enable_i2c,
	.i2c.disable_i2c        = __metal_driver_synopsys_i2c_disable_i2c,
	.i2c.init				= __metal_driver_synopsys_i2c_init,
	.i2c.write_transfer 	= __metal_driver_synopsys_i2c_write_transfer,
	.i2c.read_transfer 		= __metal_driver_synopsys_i2c_read_transfer,
	.i2c.set_address_mode 	= __metal_driver_synopsys_i2c_set_address_mode,
	.i2c.set_speed_mode 	= __metal_driver_synopsys_i2c_set_speed_mode,
	.i2c.set_target_address = __metal_driver_synopsys_i2c_set_target_address,
	.i2c.get_interrupt_controller = __metal_driver_synopsys_i2c_interrupt_parent,
	//.i2c.get_interrupt_id   = __metal_driver_synopsys_i2c_get_interrupt_id
};



#endif