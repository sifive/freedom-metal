#include <metal/machine/platform.h>

#ifdef METAL_SYNOPSYS_I2S_MASTER_V1_11A
      #define SYNOPSYS_I2S_V1_11A
#else
#ifdef METAL_SYNOPSYS_I2S_SLAVE_V1_11A 
      #define SYNOPSYS_I2S_V1_11A
#endif
#endif

#ifdef SYNOPSYS_I2S_V1_11A    

#include <metal/drivers/synopsys_i2s.h>
#include <metal/io.h>
#include <metal/machine.h>

// no need of below #defines need to be placed in the bare_metal
#define SYNOPSYS_I2S_V1_11A_IER 0x00
#define SYNOPSYS_I2S_V1_11A_IRER 0x04
#define SYNOPSYS_I2S_V1_11A_ITER 0X08
#define SYNOPSYS_I2S_V1_11A_CER 0X0C
#define SYNOPSYS_I2S_V1_11A_CCR 0X10
#define SYNOPSYS_I2S_V1_11A_RXFFR 0X14
#define SYNOPSYS_I2S_V1_11A_TXFFR 0X18
#define SYNOPSYS_I2S_V1_11A_LRBR_0 0X020 //0X020+(0X040*X)
#define SYNOPSYS_I2S_V1_11A_LRBR_1 0X060 
#define SYNOPSYS_I2S_V1_11A_LRBR_2 0X0A0
#define SYNOPSYS_I2S_V1_11A_LRBR_3 0X0E0
#define SYNOPSYS_I2S_V1_11A_LTHR_0 0X020
#define SYNOPSYS_I2S_V1_11A_LTHR_1 0X060
#define SYNOPSYS_I2S_V1_11A_LTHR_2 0X0A0
#define SYNOPSYS_I2S_V1_11A_LTHR_3 0X0E0
#define SYNOPSYS_I2S_V1_11A_RRBR_0 0X024
#define SYNOPSYS_I2S_V1_11A_RRBR_1 0X064
#define SYNOPSYS_I2S_V1_11A_RRBR_2 0X0A4
#define SYNOPSYS_I2S_V1_11A_RRBR_3 0X0E4
#define SYNOPSYS_I2S_V1_11A_RTHR_0 0X024
#define SYNOPSYS_I2S_V1_11A_RTHR_1 0X064
#define SYNOPSYS_I2S_V1_11A_RTHR_2 0X0A4
#define SYNOPSYS_I2S_V1_11A_RTHR_3 0X0E4
#define SYNOPSYS_I2S_V1_11A_RER_0 0X028
#define SYNOPSYS_I2S_V1_11A_RER_1 0X068
#define SYNOPSYS_I2S_V1_11A_RER_2 0X0A8
#define SYNOPSYS_I2S_V1_11A_RER_3 0X0E8
#define SYNOPSYS_I2S_V1_11A_TER_0 0X02C
#define SYNOPSYS_I2S_V1_11A_TER_1 0X06C
#define SYNOPSYS_I2S_V1_11A_TER_2 0X0AC
#define SYNOPSYS_I2S_V1_11A_TER_3 0X0EC
#define SYNOPSYS_I2S_V1_11A_RCR_0 0X030
#define SYNOPSYS_I2S_V1_11A_RCR_1 0X070
#define SYNOPSYS_I2S_V1_11A_RCR_2 0X0B0
#define SYNOPSYS_I2S_V1_11A_RCR_3 0X0F0
#define SYNOPSYS_I2S_V1_11A_TCR_0 0X034
#define SYNOPSYS_I2S_V1_11A_TCR_1 0X074
#define SYNOPSYS_I2S_V1_11A_TCR_2 0X0B4
#define SYNOPSYS_I2S_V1_11A_TCR_3 0X0F4
#define SYNOPSYS_I2S_V1_11A_ISR_0 0X038
#define SYNOPSYS_I2S_V1_11A_ISR_1 0X078
#define SYNOPSYS_I2S_V1_11A_ISR_2 0X0B8
#define SYNOPSYS_I2S_V1_11A_ISR_3 0X0F8
#define SYNOPSYS_I2S_V1_11A_IMR_0 0X03C
#define SYNOPSYS_I2S_V1_11A_IMR_1 0X07C
#define SYNOPSYS_I2S_V1_11A_IMR_2 0X0BC
#define SYNOPSYS_I2S_V1_11A_IMR_3 0X0FC
#define SYNOPSYS_I2S_V1_11A_ROR_0 0X040
#define SYNOPSYS_I2S_V1_11A_ROR_1 0X080
#define SYNOPSYS_I2S_V1_11A_ROR_2 0X0C0
#define SYNOPSYS_I2S_V1_11A_ROR_3 0X100
#define SYNOPSYS_I2S_V1_11A_TOR_0 0X044
#define SYNOPSYS_I2S_V1_11A_TOR_1 0X084
#define SYNOPSYS_I2S_V1_11A_TOR_2 0X0C4
#define SYNOPSYS_I2S_V1_11A_TOR_3 0X104
#define SYNOPSYS_I2S_V1_11A_RFCR_0 0X048
#define SYNOPSYS_I2S_V1_11A_RFCR_1 0X088
#define SYNOPSYS_I2S_V1_11A_RFCR_2 0X0C8
#define SYNOPSYS_I2S_V1_11A_RFCR_3 0X108
#define SYNOPSYS_I2S_V1_11A_TFCR_0 0X04C
#define SYNOPSYS_I2S_V1_11A_TFCR_1 0X08C
#define SYNOPSYS_I2S_V1_11A_TFCR_2 0X0CC
#define SYNOPSYS_I2S_V1_11A_TFCR_3 0X10C
#define SYNOPSYS_I2S_V1_11A_RFF_0 0X050
#define SYNOPSYS_I2S_V1_11A_RFF_1 0X090
#define SYNOPSYS_I2S_V1_11A_RFF_2 0X0D0
#define SYNOPSYS_I2S_V1_11A_RFF_3 0X110
#define SYNOPSYS_I2S_V1_11A_TFF_0 0X054
#define SYNOPSYS_I2S_V1_11A_TFF_1 0X094
#define SYNOPSYS_I2S_V1_11A_TFF_2 0X0D4
#define SYNOPSYS_I2S_V1_11A_TFF_3 0X114
#define SYNOPSYS_I2S_V1_11A_RXDMA 0X1C0
#define SYNOPSYS_I2S_V1_11A_RRXDMA 0X1C4
#define SYNOPSYS_I2S_V1_11A_TXDMA 0X1C8
#define SYNOPSYS_I2S_V1_11A_RTXDMA 0X1CC
#define SYNOPSYS_I2S_V1_11A_COMP_PARAM_2 0X1F0
#define SYNOPSYS_I2S_V1_11A_COMP_PARAM_1 0X1F4
#define SYNOPSYS_I2S_V1_11A_COMP_VERSION 0X1F8
#define SYNOPSYS_I2S_V1_11A_COMP_TYPE 0X1FC
#define SYNOPSYS_I2S_V1_11A_DMACR 0X200
#define SYNOPSYS_I2S_V1_11A_RXDMA_CH_0 0X204
#define SYNOPSYS_I2S_V1_11A_RXDMA_CH_1 0X204
#define SYNOPSYS_I2S_V1_11A_RXDMA_CH_2 0X204
#define SYNOPSYS_I2S_V1_11A_RXDMA_CH_3 0X204
#define SYNOPSYS_I2S_V1_11A_TXDMA_CH_0 0X214
#define SYNOPSYS_I2S_V1_11A_TXDMA_CH_1 0X214
#define SYNOPSYS_I2S_V1_11A_TXDMA_CH_2 0X214
#define SYNOPSYS_I2S_V1_11A_TXDMA_CH_3 0X214
/////////////////////////////////////////
#define REGISTER_DATATYPE uint32_t
#define INTERFACE_SIZE 32
#define REGISTER_SIZE 32

#define I2S_REG(TYPE,offset) ((unsigned long)((__metal_driver_synopsys_i2s_##TYPE##_control_base((struct metal_i2s *)i2s)) + offset))
#define I2S_REGB(TYPE,offset) (__METAL_ACCESS_ONCE((__metal_io_u32 *)I2S_REG(TYPE,offset)))
#define I2S_REG_RW_BIT(offset,bit,val) (write_register(cfg,i2s,offset,set_bit(read_register(cfg,i2s,offset),bit,val)))
#define I2S_REG_RW_BIT_RANGE(offset,start_bit,stop_bit,val) (write_register(cfg,i2s,offset,set_bit_range(read_register(cfg,i2s,offset),start_bit,stop_bit,val)))

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


REGISTER_DATATYPE power(REGISTER_DATATYPE base,REGISTER_DATATYPE power)
{
	REGISTER_DATATYPE val = 1;

	if(power == 0)
	{
		return 1;
	}
	else
	{
		for(int i=0; i<power; ++i)
			{
				val = val*base; 
			}

	}	return val;
}

REGISTER_DATATYPE split_lsb(REGISTER_DATATYPE value, int bit)
{
	REGISTER_DATATYPE lsb;

		lsb = (power(2,bit)-1);
		lsb = (value & lsb);
		return lsb;					
}

REGISTER_DATATYPE get_new_msb(REGISTER_DATATYPE value,int bit,REGISTER_DATATYPE val)
{
	REGISTER_DATATYPE temp;

	temp = (value >> (bit+1))<<1;

	return (temp | val);
}

REGISTER_DATATYPE set_bit(REGISTER_DATATYPE value, int bit, REGISTER_DATATYPE val)
{
	REGISTER_DATATYPE MSB , LSB;

				LSB = split_lsb(value,bit);
				MSB = get_new_msb(value,bit,val);
				return ((MSB<<bit) | (LSB));
}

REGISTER_DATATYPE set_bit_range(REGISTER_DATATYPE value,int start_bit,int end_bit,REGISTER_DATATYPE val)
{
	REGISTER_DATATYPE max_val,bit_val;
	REGISTER_DATATYPE last_val=value;

	for(int i= start_bit; i<= end_bit; ++i)
	{
		max_val = power(2,i-start_bit);
		bit_val = (max_val & val)>>(i-start_bit); 
		last_val = set_bit(last_val,i,bit_val);
	}
	return last_val;
}

void write_register(struct metal_i2s_config *cfg,struct metal_i2s *i2s,REGISTER_DATATYPE offset,REGISTER_DATATYPE value)
{

	for(int i=0; i<(REGISTER_SIZE/INTERFACE_SIZE); ++i)
	{
		if(INTERFACE_SIZE == 16)
		{
			if(cfg->master==1)
			{
			I2S_REGB(master,offset+(2*i)) =  value>>(INTERFACE_SIZE*i);
			}
			else if(cfg->master==0)
			{
			I2S_REGB(slave,offset+(2*i)) =  value>>(INTERFACE_SIZE*i);	
			}
		}
		else
		{
			if(cfg->master==1)
			{
			 I2S_REGB(master,offset+i) = value>>(INTERFACE_SIZE*i);
			}
			else if(cfg->master==0)
			{
			I2S_REGB(slave,offset+i) =  value>>(INTERFACE_SIZE*i);	
			}
		}
	}
}	


REGISTER_DATATYPE read_register(struct metal_i2s_config *cfg,struct metal_i2s *i2s,REGISTER_DATATYPE offset)
{
	
	REGISTER_DATATYPE new_val;
	REGISTER_DATATYPE prev_val = 0;


	for(int i=0; i<(REGISTER_SIZE/INTERFACE_SIZE); ++i)
	{
		if(INTERFACE_SIZE == 16)
		{
			if(cfg->master==1)
			{
			new_val = I2S_REGB(master,offset+(2*i));
		    }
		    else if(cfg->master==0)
		    {
		    new_val = I2S_REGB(slave,offset+(2*i));	
		    }
		}
		else 
		{
			if(cfg->master==1)
			{
			new_val = I2S_REGB(master,offset+i);
		    }
		    else if(cfg->master==0)
		    {
		    new_val = I2S_REGB(slave,offset+i);	
		    }
		}	
			
		new_val = new_val<<(INTERFACE_SIZE * i);
		prev_val = prev_val | new_val;
	}
	
		return prev_val;
}

REGISTER_DATATYPE read_register_bit(struct metal_i2s_config *cfg,struct metal_i2s *i2s,REGISTER_DATATYPE offset,int bit)
{
	return ((read_register(cfg,i2s,offset) & power(2,bit))>>bit);
}

REGISTER_DATATYPE read_register_bit_range(struct metal_i2s_config *cfg,struct metal_i2s *i2s,REGISTER_DATATYPE offset,int start_bit,int stop_bit)
{
	return ((read_register(cfg,i2s,offset) & ((power(2,(stop_bit-start_bit+1))-1)<<start_bit))>>start_bit);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int __metal_driver_synopsys_i2s_set_data_resolution(struct metal_i2s *i2s,struct metal_i2s_config *cfg)
{  


if(cfg->no_of_channels==1)
	{ 
		if(cfg->data_resolution == DATA_RESOLUTION_IGNORE)
		{
			I2S_REG_RW_BIT(SYNOPSYS_I2S_V1_11A_RER_0,0,0);
			I2S_REG_RW_BIT_RANGE(SYNOPSYS_I2S_V1_11A_RCR_0,0,2,cfg->data_resolution);
			I2S_REG_RW_BIT(SYNOPSYS_I2S_V1_11A_RER_0,0,1);
			I2S_REG_RW_BIT(SYNOPSYS_I2S_V1_11A_TER_0,0,0);
			I2S_REG_RW_BIT_RANGE(SYNOPSYS_I2S_V1_11A_TCR_0,0,2,cfg->data_resolution);
			I2S_REG_RW_BIT(SYNOPSYS_I2S_V1_11A_TER_0,0,1);
		}
		else if(cfg->data_resolution == DATA_RESOLUTION_12)
		{
			I2S_REG_RW_BIT(SYNOPSYS_I2S_V1_11A_RER_0,0,0);
			I2S_REG_RW_BIT_RANGE(SYNOPSYS_I2S_V1_11A_RCR_0,0,2,cfg->data_resolution);
			I2S_REG_RW_BIT(SYNOPSYS_I2S_V1_11A_RER_0,0,1);
			I2S_REG_RW_BIT(SYNOPSYS_I2S_V1_11A_TER_0,0,0);
			I2S_REG_RW_BIT_RANGE(SYNOPSYS_I2S_V1_11A_TCR_0,0,2,cfg->data_resolution);
			I2S_REG_RW_BIT(SYNOPSYS_I2S_V1_11A_TER_0,0,1);
		}
		else if(cfg->data_resolution == DATA_RESOLUTION_16)
		{
			I2S_REG_RW_BIT(SYNOPSYS_I2S_V1_11A_RER_0,0,0);
			I2S_REG_RW_BIT_RANGE(SYNOPSYS_I2S_V1_11A_RCR_0,0,2,cfg->data_resolution);
			I2S_REG_RW_BIT(SYNOPSYS_I2S_V1_11A_RER_0,0,1);
			I2S_REG_RW_BIT(SYNOPSYS_I2S_V1_11A_TER_0,0,0);
			I2S_REG_RW_BIT_RANGE(SYNOPSYS_I2S_V1_11A_TCR_0,0,2,cfg->data_resolution);		
			I2S_REG_RW_BIT(SYNOPSYS_I2S_V1_11A_TER_0,0,1);
		}
		else if(cfg->data_resolution == DATA_RESOLUTION_20)
		{
			I2S_REG_RW_BIT(SYNOPSYS_I2S_V1_11A_RER_0,0,0);
			I2S_REG_RW_BIT_RANGE(SYNOPSYS_I2S_V1_11A_RCR_0,0,2,cfg->data_resolution);
			I2S_REG_RW_BIT(SYNOPSYS_I2S_V1_11A_RER_0,0,1);
			I2S_REG_RW_BIT(SYNOPSYS_I2S_V1_11A_TER_0,0,0);
			I2S_REG_RW_BIT_RANGE(SYNOPSYS_I2S_V1_11A_TCR_0,0,2,cfg->data_resolution);	
			I2S_REG_RW_BIT(SYNOPSYS_I2S_V1_11A_TER_0,0,1);	
		}
		else if(cfg->data_resolution == DATA_RESOLUTION_24)
		{
			I2S_REG_RW_BIT(SYNOPSYS_I2S_V1_11A_RER_0,0,0);
			I2S_REG_RW_BIT_RANGE(SYNOPSYS_I2S_V1_11A_RCR_0,0,2,cfg->data_resolution);
			I2S_REG_RW_BIT(SYNOPSYS_I2S_V1_11A_RER_0,0,1);
			I2S_REG_RW_BIT(SYNOPSYS_I2S_V1_11A_TER_0,0,0);
			I2S_REG_RW_BIT_RANGE(SYNOPSYS_I2S_V1_11A_TCR_0,0,2,cfg->data_resolution);	
			I2S_REG_RW_BIT(SYNOPSYS_I2S_V1_11A_TER_0,0,1);
		}
		else if(cfg->data_resolution == DATA_RESOLUTION_32)
		{
			I2S_REG_RW_BIT(SYNOPSYS_I2S_V1_11A_RER_0,0,0);
			I2S_REG_RW_BIT_RANGE(SYNOPSYS_I2S_V1_11A_RCR_0,0,2,cfg->data_resolution);
			I2S_REG_RW_BIT(SYNOPSYS_I2S_V1_11A_RER_0,0,1);
			I2S_REG_RW_BIT(SYNOPSYS_I2S_V1_11A_TER_0,0,0);
			I2S_REG_RW_BIT_RANGE(SYNOPSYS_I2S_V1_11A_TCR_0,0,2,cfg->data_resolution);
			I2S_REG_RW_BIT(SYNOPSYS_I2S_V1_11A_TER_0,0,1);	
		}
	}
	else if(cfg->no_of_channels==2)
	{
		if(cfg->data_resolution == DATA_RESOLUTION_IGNORE)
		{
			I2S_REG_RW_BIT(SYNOPSYS_I2S_V1_11A_RER_0,0,0);
			I2S_REG_RW_BIT_RANGE(SYNOPSYS_I2S_V1_11A_RCR_0,0,2,cfg->data_resolution);
			I2S_REG_RW_BIT(SYNOPSYS_I2S_V1_11A_RER_0,0,1);
			I2S_REG_RW_BIT(SYNOPSYS_I2S_V1_11A_TER_0,0,0);
			I2S_REG_RW_BIT_RANGE(SYNOPSYS_I2S_V1_11A_TCR_0,0,2,cfg->data_resolution);
			I2S_REG_RW_BIT(SYNOPSYS_I2S_V1_11A_TER_0,0,1);
			I2S_REG_RW_BIT(SYNOPSYS_I2S_V1_11A_RER_1,0,0);
			I2S_REG_RW_BIT_RANGE(SYNOPSYS_I2S_V1_11A_RCR_1,0,2,cfg->data_resolution);
			I2S_REG_RW_BIT(SYNOPSYS_I2S_V1_11A_RER_1,0,1);
			I2S_REG_RW_BIT(SYNOPSYS_I2S_V1_11A_TER_1,0,0);
			I2S_REG_RW_BIT_RANGE(SYNOPSYS_I2S_V1_11A_TCR_1,0,2,cfg->data_resolution);
			I2S_REG_RW_BIT(SYNOPSYS_I2S_V1_11A_TER_1,0,1);
		}
		else if(cfg->data_resolution == DATA_RESOLUTION_12)
		{
			I2S_REG_RW_BIT(SYNOPSYS_I2S_V1_11A_RER_0,0,0);
			I2S_REG_RW_BIT_RANGE(SYNOPSYS_I2S_V1_11A_RCR_0,0,2,cfg->data_resolution);
			I2S_REG_RW_BIT(SYNOPSYS_I2S_V1_11A_RER_0,0,1);
			I2S_REG_RW_BIT(SYNOPSYS_I2S_V1_11A_TER_0,0,0);
			I2S_REG_RW_BIT_RANGE(SYNOPSYS_I2S_V1_11A_TCR_0,0,2,cfg->data_resolution);
			I2S_REG_RW_BIT(SYNOPSYS_I2S_V1_11A_TER_0,0,1);
			I2S_REG_RW_BIT(SYNOPSYS_I2S_V1_11A_RER_1,0,0);
			I2S_REG_RW_BIT_RANGE(SYNOPSYS_I2S_V1_11A_RCR_1,0,2,cfg->data_resolution);
			I2S_REG_RW_BIT(SYNOPSYS_I2S_V1_11A_RER_1,0,1);
			I2S_REG_RW_BIT(SYNOPSYS_I2S_V1_11A_TER_1,0,0);
			I2S_REG_RW_BIT_RANGE(SYNOPSYS_I2S_V1_11A_TCR_1,0,2,cfg->data_resolution);
			I2S_REG_RW_BIT(SYNOPSYS_I2S_V1_11A_TER_1,0,1);	
		}
		else if(cfg->data_resolution == DATA_RESOLUTION_16)
		{
			I2S_REG_RW_BIT(SYNOPSYS_I2S_V1_11A_RER_0,0,0);
			I2S_REG_RW_BIT_RANGE(SYNOPSYS_I2S_V1_11A_RCR_0,0,2,cfg->data_resolution);
			I2S_REG_RW_BIT(SYNOPSYS_I2S_V1_11A_RER_0,0,1);
			I2S_REG_RW_BIT(SYNOPSYS_I2S_V1_11A_TER_0,0,0);
			I2S_REG_RW_BIT_RANGE(SYNOPSYS_I2S_V1_11A_TCR_0,0,2,cfg->data_resolution);
			I2S_REG_RW_BIT(SYNOPSYS_I2S_V1_11A_TER_0,0,1);
			I2S_REG_RW_BIT(SYNOPSYS_I2S_V1_11A_RER_1,0,0);
			I2S_REG_RW_BIT_RANGE(SYNOPSYS_I2S_V1_11A_RCR_1,0,2,cfg->data_resolution);
			I2S_REG_RW_BIT(SYNOPSYS_I2S_V1_11A_RER_1,0,1);
			I2S_REG_RW_BIT(SYNOPSYS_I2S_V1_11A_TER_1,0,0);
			I2S_REG_RW_BIT_RANGE(SYNOPSYS_I2S_V1_11A_TCR_1,0,2,cfg->data_resolution);
			I2S_REG_RW_BIT(SYNOPSYS_I2S_V1_11A_TER_1,0,1);	
		}
		else if(cfg->data_resolution == DATA_RESOLUTION_20)
		{
			I2S_REG_RW_BIT(SYNOPSYS_I2S_V1_11A_RER_0,0,0);
			I2S_REG_RW_BIT_RANGE(SYNOPSYS_I2S_V1_11A_RCR_0,0,2,cfg->data_resolution);
			I2S_REG_RW_BIT(SYNOPSYS_I2S_V1_11A_RER_0,0,1);
			I2S_REG_RW_BIT(SYNOPSYS_I2S_V1_11A_TER_0,0,0);
			I2S_REG_RW_BIT_RANGE(SYNOPSYS_I2S_V1_11A_TCR_0,0,2,cfg->data_resolution);
			I2S_REG_RW_BIT(SYNOPSYS_I2S_V1_11A_TER_0,0,1);
			I2S_REG_RW_BIT(SYNOPSYS_I2S_V1_11A_RER_1,0,0);
			I2S_REG_RW_BIT_RANGE(SYNOPSYS_I2S_V1_11A_RCR_1,0,2,cfg->data_resolution);
			I2S_REG_RW_BIT(SYNOPSYS_I2S_V1_11A_RER_1,0,1);
			I2S_REG_RW_BIT(SYNOPSYS_I2S_V1_11A_TER_1,0,0);
			I2S_REG_RW_BIT_RANGE(SYNOPSYS_I2S_V1_11A_TCR_1,0,2,cfg->data_resolution);
			I2S_REG_RW_BIT(SYNOPSYS_I2S_V1_11A_TER_1,0,1);		
		}
		else if(cfg->data_resolution == DATA_RESOLUTION_24)
		{
			I2S_REG_RW_BIT(SYNOPSYS_I2S_V1_11A_RER_0,0,0);
			I2S_REG_RW_BIT_RANGE(SYNOPSYS_I2S_V1_11A_RCR_0,0,2,cfg->data_resolution);
			I2S_REG_RW_BIT(SYNOPSYS_I2S_V1_11A_RER_0,0,1);
			I2S_REG_RW_BIT(SYNOPSYS_I2S_V1_11A_TER_0,0,0);
			I2S_REG_RW_BIT_RANGE(SYNOPSYS_I2S_V1_11A_TCR_0,0,2,cfg->data_resolution);
			I2S_REG_RW_BIT(SYNOPSYS_I2S_V1_11A_TER_0,0,1);
			I2S_REG_RW_BIT(SYNOPSYS_I2S_V1_11A_RER_1,0,0);
			I2S_REG_RW_BIT_RANGE(SYNOPSYS_I2S_V1_11A_RCR_1,0,2,cfg->data_resolution);
			I2S_REG_RW_BIT(SYNOPSYS_I2S_V1_11A_RER_1,0,1);
			I2S_REG_RW_BIT(SYNOPSYS_I2S_V1_11A_TER_1,0,0);
			I2S_REG_RW_BIT_RANGE(SYNOPSYS_I2S_V1_11A_TCR_1,0,2,cfg->data_resolution);
			I2S_REG_RW_BIT(SYNOPSYS_I2S_V1_11A_TER_1,0,1);	
		}
		else if(cfg->data_resolution == DATA_RESOLUTION_32)
		{
			I2S_REG_RW_BIT(SYNOPSYS_I2S_V1_11A_RER_0,0,0);
			I2S_REG_RW_BIT_RANGE(SYNOPSYS_I2S_V1_11A_RCR_0,0,2,cfg->data_resolution);
			I2S_REG_RW_BIT(SYNOPSYS_I2S_V1_11A_RER_0,0,1);
			I2S_REG_RW_BIT(SYNOPSYS_I2S_V1_11A_TER_0,0,0);
			I2S_REG_RW_BIT_RANGE(SYNOPSYS_I2S_V1_11A_TCR_0,0,2,cfg->data_resolution);
			I2S_REG_RW_BIT(SYNOPSYS_I2S_V1_11A_TER_0,0,1);
			I2S_REG_RW_BIT(SYNOPSYS_I2S_V1_11A_RER_1,0,0);
			I2S_REG_RW_BIT_RANGE(SYNOPSYS_I2S_V1_11A_RCR_1,0,2,cfg->data_resolution);
			I2S_REG_RW_BIT(SYNOPSYS_I2S_V1_11A_RER_1,0,1);
			I2S_REG_RW_BIT(SYNOPSYS_I2S_V1_11A_TER_1,0,0);
			I2S_REG_RW_BIT_RANGE(SYNOPSYS_I2S_V1_11A_TCR_1,0,2,cfg->data_resolution);
			I2S_REG_RW_BIT(SYNOPSYS_I2S_V1_11A_TER_1,0,1);		
		}
	}		
	else if(cfg->no_of_channels==3)
	{
		if(cfg->data_resolution == DATA_RESOLUTION_IGNORE)
		{
			I2S_REG_RW_BIT(SYNOPSYS_I2S_V1_11A_RER_0,0,0);
			I2S_REG_RW_BIT_RANGE(SYNOPSYS_I2S_V1_11A_RCR_0,0,2,cfg->data_resolution);
			I2S_REG_RW_BIT(SYNOPSYS_I2S_V1_11A_RER_0,0,1);
			I2S_REG_RW_BIT(SYNOPSYS_I2S_V1_11A_TER_0,0,0);
			I2S_REG_RW_BIT_RANGE(SYNOPSYS_I2S_V1_11A_TCR_0,0,2,cfg->data_resolution);
			I2S_REG_RW_BIT(SYNOPSYS_I2S_V1_11A_TER_0,0,1);
			I2S_REG_RW_BIT(SYNOPSYS_I2S_V1_11A_RER_1,0,0);
			I2S_REG_RW_BIT_RANGE(SYNOPSYS_I2S_V1_11A_RCR_1,0,2,cfg->data_resolution);
			I2S_REG_RW_BIT(SYNOPSYS_I2S_V1_11A_RER_1,0,1);
			I2S_REG_RW_BIT(SYNOPSYS_I2S_V1_11A_TER_1,0,0);
			I2S_REG_RW_BIT_RANGE(SYNOPSYS_I2S_V1_11A_TCR_1,0,2,cfg->data_resolution);
			I2S_REG_RW_BIT(SYNOPSYS_I2S_V1_11A_TER_1,0,1);
			I2S_REG_RW_BIT(SYNOPSYS_I2S_V1_11A_RER_2,0,0);
			I2S_REG_RW_BIT_RANGE(SYNOPSYS_I2S_V1_11A_RCR_2,0,2,cfg->data_resolution);
			I2S_REG_RW_BIT(SYNOPSYS_I2S_V1_11A_RER_2,0,1);
			I2S_REG_RW_BIT(SYNOPSYS_I2S_V1_11A_TER_2,0,0);
			I2S_REG_RW_BIT_RANGE(SYNOPSYS_I2S_V1_11A_TCR_2,0,2,cfg->data_resolution);
			I2S_REG_RW_BIT(SYNOPSYS_I2S_V1_11A_TER_2,0,1);
		}
		else if(cfg->data_resolution == DATA_RESOLUTION_12)
		{
			I2S_REG_RW_BIT(SYNOPSYS_I2S_V1_11A_RER_0,0,0);
			I2S_REG_RW_BIT_RANGE(SYNOPSYS_I2S_V1_11A_RCR_0,0,2,cfg->data_resolution);
			I2S_REG_RW_BIT(SYNOPSYS_I2S_V1_11A_RER_0,0,1);
			I2S_REG_RW_BIT(SYNOPSYS_I2S_V1_11A_TER_0,0,0);
			I2S_REG_RW_BIT_RANGE(SYNOPSYS_I2S_V1_11A_TCR_0,0,2,cfg->data_resolution);
			I2S_REG_RW_BIT(SYNOPSYS_I2S_V1_11A_TER_0,0,1);
			I2S_REG_RW_BIT(SYNOPSYS_I2S_V1_11A_RER_1,0,0);
			I2S_REG_RW_BIT_RANGE(SYNOPSYS_I2S_V1_11A_RCR_1,0,2,cfg->data_resolution);
			I2S_REG_RW_BIT(SYNOPSYS_I2S_V1_11A_RER_1,0,1);
			I2S_REG_RW_BIT(SYNOPSYS_I2S_V1_11A_TER_1,0,0);
			I2S_REG_RW_BIT_RANGE(SYNOPSYS_I2S_V1_11A_TCR_1,0,2,cfg->data_resolution);
			I2S_REG_RW_BIT(SYNOPSYS_I2S_V1_11A_TER_1,0,1);
			I2S_REG_RW_BIT(SYNOPSYS_I2S_V1_11A_RER_2,0,0);
			I2S_REG_RW_BIT_RANGE(SYNOPSYS_I2S_V1_11A_RCR_2,0,2,cfg->data_resolution);
			I2S_REG_RW_BIT(SYNOPSYS_I2S_V1_11A_RER_2,0,1);
			I2S_REG_RW_BIT(SYNOPSYS_I2S_V1_11A_TER_2,0,0);
			I2S_REG_RW_BIT_RANGE(SYNOPSYS_I2S_V1_11A_TCR_2,0,2,cfg->data_resolution);
			I2S_REG_RW_BIT(SYNOPSYS_I2S_V1_11A_TER_2,0,1);
		}
		else if(cfg->data_resolution == DATA_RESOLUTION_16)
		{
			I2S_REG_RW_BIT(SYNOPSYS_I2S_V1_11A_RER_0,0,0);
			I2S_REG_RW_BIT_RANGE(SYNOPSYS_I2S_V1_11A_RCR_0,0,2,cfg->data_resolution);
			I2S_REG_RW_BIT(SYNOPSYS_I2S_V1_11A_RER_0,0,1);
			I2S_REG_RW_BIT(SYNOPSYS_I2S_V1_11A_TER_0,0,0);
			I2S_REG_RW_BIT_RANGE(SYNOPSYS_I2S_V1_11A_TCR_0,0,2,cfg->data_resolution);
			I2S_REG_RW_BIT(SYNOPSYS_I2S_V1_11A_TER_0,0,1);
			I2S_REG_RW_BIT(SYNOPSYS_I2S_V1_11A_RER_1,0,0);
			I2S_REG_RW_BIT_RANGE(SYNOPSYS_I2S_V1_11A_RCR_1,0,2,cfg->data_resolution);
			I2S_REG_RW_BIT(SYNOPSYS_I2S_V1_11A_RER_1,0,1);
			I2S_REG_RW_BIT(SYNOPSYS_I2S_V1_11A_TER_1,0,0);
			I2S_REG_RW_BIT_RANGE(SYNOPSYS_I2S_V1_11A_TCR_1,0,2,cfg->data_resolution);
			I2S_REG_RW_BIT(SYNOPSYS_I2S_V1_11A_TER_1,0,1);
			I2S_REG_RW_BIT(SYNOPSYS_I2S_V1_11A_RER_2,0,0);
			I2S_REG_RW_BIT_RANGE(SYNOPSYS_I2S_V1_11A_RCR_2,0,2,cfg->data_resolution);
			I2S_REG_RW_BIT(SYNOPSYS_I2S_V1_11A_RER_2,0,1);
			I2S_REG_RW_BIT(SYNOPSYS_I2S_V1_11A_TER_2,0,0);
			I2S_REG_RW_BIT_RANGE(SYNOPSYS_I2S_V1_11A_TCR_2,0,2,cfg->data_resolution);
			I2S_REG_RW_BIT(SYNOPSYS_I2S_V1_11A_TER_2,0,1);
		}
		else if(cfg->data_resolution == DATA_RESOLUTION_20)
		{
			I2S_REG_RW_BIT(SYNOPSYS_I2S_V1_11A_RER_0,0,0);
			I2S_REG_RW_BIT_RANGE(SYNOPSYS_I2S_V1_11A_RCR_0,0,2,cfg->data_resolution);
			I2S_REG_RW_BIT(SYNOPSYS_I2S_V1_11A_RER_0,0,1);
			I2S_REG_RW_BIT(SYNOPSYS_I2S_V1_11A_TER_0,0,0);
			I2S_REG_RW_BIT_RANGE(SYNOPSYS_I2S_V1_11A_TCR_0,0,2,cfg->data_resolution);
			I2S_REG_RW_BIT(SYNOPSYS_I2S_V1_11A_TER_0,0,1);
			I2S_REG_RW_BIT(SYNOPSYS_I2S_V1_11A_RER_1,0,0);
			I2S_REG_RW_BIT_RANGE(SYNOPSYS_I2S_V1_11A_RCR_1,0,2,cfg->data_resolution);
			I2S_REG_RW_BIT(SYNOPSYS_I2S_V1_11A_RER_1,0,1);
			I2S_REG_RW_BIT(SYNOPSYS_I2S_V1_11A_TER_1,0,0);
			I2S_REG_RW_BIT_RANGE(SYNOPSYS_I2S_V1_11A_TCR_1,0,2,cfg->data_resolution);
			I2S_REG_RW_BIT(SYNOPSYS_I2S_V1_11A_TER_1,0,1);
			I2S_REG_RW_BIT(SYNOPSYS_I2S_V1_11A_RER_2,0,0);
			I2S_REG_RW_BIT_RANGE(SYNOPSYS_I2S_V1_11A_RCR_2,0,2,cfg->data_resolution);
			I2S_REG_RW_BIT(SYNOPSYS_I2S_V1_11A_RER_2,0,1);
			I2S_REG_RW_BIT(SYNOPSYS_I2S_V1_11A_TER_2,0,0);
			I2S_REG_RW_BIT_RANGE(SYNOPSYS_I2S_V1_11A_TCR_2,0,2,cfg->data_resolution);
			I2S_REG_RW_BIT(SYNOPSYS_I2S_V1_11A_TER_2,0,1);
		}
		else if(cfg->data_resolution == DATA_RESOLUTION_24)
		{
			I2S_REG_RW_BIT(SYNOPSYS_I2S_V1_11A_RER_0,0,0);
			I2S_REG_RW_BIT_RANGE(SYNOPSYS_I2S_V1_11A_RCR_0,0,2,cfg->data_resolution);
			I2S_REG_RW_BIT(SYNOPSYS_I2S_V1_11A_RER_0,0,1);
			I2S_REG_RW_BIT(SYNOPSYS_I2S_V1_11A_TER_0,0,0);
			I2S_REG_RW_BIT_RANGE(SYNOPSYS_I2S_V1_11A_TCR_0,0,2,cfg->data_resolution);
			I2S_REG_RW_BIT(SYNOPSYS_I2S_V1_11A_TER_0,0,1);
			I2S_REG_RW_BIT(SYNOPSYS_I2S_V1_11A_RER_1,0,0);
			I2S_REG_RW_BIT_RANGE(SYNOPSYS_I2S_V1_11A_RCR_1,0,2,cfg->data_resolution);
			I2S_REG_RW_BIT(SYNOPSYS_I2S_V1_11A_RER_1,0,1);
			I2S_REG_RW_BIT(SYNOPSYS_I2S_V1_11A_TER_1,0,0);
			I2S_REG_RW_BIT_RANGE(SYNOPSYS_I2S_V1_11A_TCR_1,0,2,cfg->data_resolution);
			I2S_REG_RW_BIT(SYNOPSYS_I2S_V1_11A_TER_1,0,1);
			I2S_REG_RW_BIT(SYNOPSYS_I2S_V1_11A_RER_2,0,0);
			I2S_REG_RW_BIT_RANGE(SYNOPSYS_I2S_V1_11A_RCR_2,0,2,cfg->data_resolution);
			I2S_REG_RW_BIT(SYNOPSYS_I2S_V1_11A_RER_2,0,1);
			I2S_REG_RW_BIT(SYNOPSYS_I2S_V1_11A_TER_2,0,0);
			I2S_REG_RW_BIT_RANGE(SYNOPSYS_I2S_V1_11A_TCR_2,0,2,cfg->data_resolution);
			I2S_REG_RW_BIT(SYNOPSYS_I2S_V1_11A_TER_2,0,1);	
		}
		else if(cfg->data_resolution == DATA_RESOLUTION_32)
		{
			I2S_REG_RW_BIT(SYNOPSYS_I2S_V1_11A_RER_0,0,0);
			I2S_REG_RW_BIT_RANGE(SYNOPSYS_I2S_V1_11A_RCR_0,0,2,cfg->data_resolution);
			I2S_REG_RW_BIT(SYNOPSYS_I2S_V1_11A_RER_0,0,1);
			I2S_REG_RW_BIT(SYNOPSYS_I2S_V1_11A_TER_0,0,0);
			I2S_REG_RW_BIT_RANGE(SYNOPSYS_I2S_V1_11A_TCR_0,0,2,cfg->data_resolution);
			I2S_REG_RW_BIT(SYNOPSYS_I2S_V1_11A_TER_0,0,1);
			I2S_REG_RW_BIT(SYNOPSYS_I2S_V1_11A_RER_1,0,0);
			I2S_REG_RW_BIT_RANGE(SYNOPSYS_I2S_V1_11A_RCR_1,0,2,cfg->data_resolution);
			I2S_REG_RW_BIT(SYNOPSYS_I2S_V1_11A_RER_1,0,1);
			I2S_REG_RW_BIT(SYNOPSYS_I2S_V1_11A_TER_1,0,0);
			I2S_REG_RW_BIT_RANGE(SYNOPSYS_I2S_V1_11A_TCR_1,0,2,cfg->data_resolution);
			I2S_REG_RW_BIT(SYNOPSYS_I2S_V1_11A_TER_1,0,1);
			I2S_REG_RW_BIT(SYNOPSYS_I2S_V1_11A_RER_2,0,0);
			I2S_REG_RW_BIT_RANGE(SYNOPSYS_I2S_V1_11A_RCR_2,0,2,cfg->data_resolution);
			I2S_REG_RW_BIT(SYNOPSYS_I2S_V1_11A_RER_2,0,1);
			I2S_REG_RW_BIT(SYNOPSYS_I2S_V1_11A_TER_2,0,0);
			I2S_REG_RW_BIT_RANGE(SYNOPSYS_I2S_V1_11A_TCR_2,0,2,cfg->data_resolution);
			I2S_REG_RW_BIT(SYNOPSYS_I2S_V1_11A_TER_2,0,1);	
		}
	}		
	else if(cfg->no_of_channels==4)
	{
		if(cfg->data_resolution == DATA_RESOLUTION_IGNORE)
		{
			I2S_REG_RW_BIT(SYNOPSYS_I2S_V1_11A_RER_0,0,0);
			I2S_REG_RW_BIT_RANGE(SYNOPSYS_I2S_V1_11A_RCR_0,0,2,cfg->data_resolution);
			I2S_REG_RW_BIT(SYNOPSYS_I2S_V1_11A_RER_0,0,1);
			I2S_REG_RW_BIT(SYNOPSYS_I2S_V1_11A_TER_0,0,0);
			I2S_REG_RW_BIT_RANGE(SYNOPSYS_I2S_V1_11A_TCR_0,0,2,cfg->data_resolution);
			I2S_REG_RW_BIT(SYNOPSYS_I2S_V1_11A_TER_0,0,1);
			I2S_REG_RW_BIT(SYNOPSYS_I2S_V1_11A_RER_1,0,0);
			I2S_REG_RW_BIT_RANGE(SYNOPSYS_I2S_V1_11A_RCR_1,0,2,cfg->data_resolution);
			I2S_REG_RW_BIT(SYNOPSYS_I2S_V1_11A_RER_1,0,1);
			I2S_REG_RW_BIT(SYNOPSYS_I2S_V1_11A_TER_1,0,0);
			I2S_REG_RW_BIT_RANGE(SYNOPSYS_I2S_V1_11A_TCR_1,0,2,cfg->data_resolution);
			I2S_REG_RW_BIT(SYNOPSYS_I2S_V1_11A_TER_1,0,1);
			I2S_REG_RW_BIT(SYNOPSYS_I2S_V1_11A_RER_2,0,0);
			I2S_REG_RW_BIT_RANGE(SYNOPSYS_I2S_V1_11A_RCR_2,0,2,cfg->data_resolution);
			I2S_REG_RW_BIT(SYNOPSYS_I2S_V1_11A_RER_2,0,1);
			I2S_REG_RW_BIT(SYNOPSYS_I2S_V1_11A_TER_2,0,0);
			I2S_REG_RW_BIT_RANGE(SYNOPSYS_I2S_V1_11A_TCR_2,0,2,cfg->data_resolution);
			I2S_REG_RW_BIT(SYNOPSYS_I2S_V1_11A_TER_2,0,1);
			I2S_REG_RW_BIT(SYNOPSYS_I2S_V1_11A_RER_3,0,0);	
			I2S_REG_RW_BIT_RANGE(SYNOPSYS_I2S_V1_11A_RCR_3,0,2,cfg->data_resolution);
			I2S_REG_RW_BIT(SYNOPSYS_I2S_V1_11A_RER_3,0,1);
			I2S_REG_RW_BIT(SYNOPSYS_I2S_V1_11A_TER_3,0,0);
			I2S_REG_RW_BIT_RANGE(SYNOPSYS_I2S_V1_11A_TCR_3,0,2,cfg->data_resolution);
			I2S_REG_RW_BIT(SYNOPSYS_I2S_V1_11A_TER_3,0,1);
		}
		else if(cfg->data_resolution == DATA_RESOLUTION_12)
		{
			I2S_REG_RW_BIT(SYNOPSYS_I2S_V1_11A_RER_0,0,0);
			I2S_REG_RW_BIT_RANGE(SYNOPSYS_I2S_V1_11A_RCR_0,0,2,cfg->data_resolution);
			I2S_REG_RW_BIT(SYNOPSYS_I2S_V1_11A_RER_0,0,1);
			I2S_REG_RW_BIT(SYNOPSYS_I2S_V1_11A_TER_0,0,0);
			I2S_REG_RW_BIT_RANGE(SYNOPSYS_I2S_V1_11A_TCR_0,0,2,cfg->data_resolution);
			I2S_REG_RW_BIT(SYNOPSYS_I2S_V1_11A_TER_0,0,1);
			I2S_REG_RW_BIT(SYNOPSYS_I2S_V1_11A_RER_1,0,0);
			I2S_REG_RW_BIT_RANGE(SYNOPSYS_I2S_V1_11A_RCR_1,0,2,cfg->data_resolution);
			I2S_REG_RW_BIT(SYNOPSYS_I2S_V1_11A_RER_1,0,1);
			I2S_REG_RW_BIT(SYNOPSYS_I2S_V1_11A_TER_1,0,0);
			I2S_REG_RW_BIT_RANGE(SYNOPSYS_I2S_V1_11A_TCR_1,0,2,cfg->data_resolution);
			I2S_REG_RW_BIT(SYNOPSYS_I2S_V1_11A_TER_1,0,1);
			I2S_REG_RW_BIT(SYNOPSYS_I2S_V1_11A_RER_2,0,0);
			I2S_REG_RW_BIT_RANGE(SYNOPSYS_I2S_V1_11A_RCR_2,0,2,cfg->data_resolution);
			I2S_REG_RW_BIT(SYNOPSYS_I2S_V1_11A_RER_2,0,1);
			I2S_REG_RW_BIT(SYNOPSYS_I2S_V1_11A_TER_2,0,0);
			I2S_REG_RW_BIT_RANGE(SYNOPSYS_I2S_V1_11A_TCR_2,0,2,cfg->data_resolution);
			I2S_REG_RW_BIT(SYNOPSYS_I2S_V1_11A_TER_2,0,1);
			I2S_REG_RW_BIT(SYNOPSYS_I2S_V1_11A_RER_3,0,0);	
			I2S_REG_RW_BIT_RANGE(SYNOPSYS_I2S_V1_11A_RCR_3,0,2,cfg->data_resolution);
			I2S_REG_RW_BIT(SYNOPSYS_I2S_V1_11A_RER_3,0,1);
			I2S_REG_RW_BIT(SYNOPSYS_I2S_V1_11A_TER_3,0,0);
			I2S_REG_RW_BIT_RANGE(SYNOPSYS_I2S_V1_11A_TCR_3,0,2,cfg->data_resolution);
			I2S_REG_RW_BIT(SYNOPSYS_I2S_V1_11A_TER_3,0,1);
		}
		else if(cfg->data_resolution == DATA_RESOLUTION_16)
		{
			I2S_REG_RW_BIT(SYNOPSYS_I2S_V1_11A_RER_0,0,0);
			I2S_REG_RW_BIT_RANGE(SYNOPSYS_I2S_V1_11A_RCR_0,0,2,cfg->data_resolution);
			I2S_REG_RW_BIT(SYNOPSYS_I2S_V1_11A_RER_0,0,1);
			I2S_REG_RW_BIT(SYNOPSYS_I2S_V1_11A_TER_0,0,0);
			I2S_REG_RW_BIT_RANGE(SYNOPSYS_I2S_V1_11A_TCR_0,0,2,cfg->data_resolution);
			I2S_REG_RW_BIT(SYNOPSYS_I2S_V1_11A_TER_0,0,1);
			I2S_REG_RW_BIT(SYNOPSYS_I2S_V1_11A_RER_1,0,0);
			I2S_REG_RW_BIT_RANGE(SYNOPSYS_I2S_V1_11A_RCR_1,0,2,cfg->data_resolution);
			I2S_REG_RW_BIT(SYNOPSYS_I2S_V1_11A_RER_1,0,1);
			I2S_REG_RW_BIT(SYNOPSYS_I2S_V1_11A_TER_1,0,0);
			I2S_REG_RW_BIT_RANGE(SYNOPSYS_I2S_V1_11A_TCR_1,0,2,cfg->data_resolution);
			I2S_REG_RW_BIT(SYNOPSYS_I2S_V1_11A_TER_1,0,1);
			I2S_REG_RW_BIT(SYNOPSYS_I2S_V1_11A_RER_2,0,0);
			I2S_REG_RW_BIT_RANGE(SYNOPSYS_I2S_V1_11A_RCR_2,0,2,cfg->data_resolution);
			I2S_REG_RW_BIT(SYNOPSYS_I2S_V1_11A_RER_2,0,1);
			I2S_REG_RW_BIT(SYNOPSYS_I2S_V1_11A_TER_2,0,0);
			I2S_REG_RW_BIT_RANGE(SYNOPSYS_I2S_V1_11A_TCR_2,0,2,cfg->data_resolution);
			I2S_REG_RW_BIT(SYNOPSYS_I2S_V1_11A_TER_2,0,1);
			I2S_REG_RW_BIT(SYNOPSYS_I2S_V1_11A_RER_3,0,0);	
			I2S_REG_RW_BIT_RANGE(SYNOPSYS_I2S_V1_11A_RCR_3,0,2,cfg->data_resolution);
			I2S_REG_RW_BIT(SYNOPSYS_I2S_V1_11A_RER_3,0,1);
			I2S_REG_RW_BIT(SYNOPSYS_I2S_V1_11A_TER_3,0,0);
			I2S_REG_RW_BIT_RANGE(SYNOPSYS_I2S_V1_11A_TCR_3,0,2,cfg->data_resolution);
			I2S_REG_RW_BIT(SYNOPSYS_I2S_V1_11A_TER_3,0,1);	
		}
		else if(cfg->data_resolution == DATA_RESOLUTION_20)
		{
			I2S_REG_RW_BIT(SYNOPSYS_I2S_V1_11A_RER_0,0,0);
			I2S_REG_RW_BIT_RANGE(SYNOPSYS_I2S_V1_11A_RCR_0,0,2,cfg->data_resolution);
			I2S_REG_RW_BIT(SYNOPSYS_I2S_V1_11A_RER_0,0,1);
			I2S_REG_RW_BIT(SYNOPSYS_I2S_V1_11A_TER_0,0,0);
			I2S_REG_RW_BIT_RANGE(SYNOPSYS_I2S_V1_11A_TCR_0,0,2,cfg->data_resolution);
			I2S_REG_RW_BIT(SYNOPSYS_I2S_V1_11A_TER_0,0,1);
			I2S_REG_RW_BIT(SYNOPSYS_I2S_V1_11A_RER_1,0,0);
			I2S_REG_RW_BIT_RANGE(SYNOPSYS_I2S_V1_11A_RCR_1,0,2,cfg->data_resolution);
			I2S_REG_RW_BIT(SYNOPSYS_I2S_V1_11A_RER_1,0,1);
			I2S_REG_RW_BIT(SYNOPSYS_I2S_V1_11A_TER_1,0,0);
			I2S_REG_RW_BIT_RANGE(SYNOPSYS_I2S_V1_11A_TCR_1,0,2,cfg->data_resolution);
			I2S_REG_RW_BIT(SYNOPSYS_I2S_V1_11A_TER_1,0,1);
			I2S_REG_RW_BIT(SYNOPSYS_I2S_V1_11A_RER_2,0,0);
			I2S_REG_RW_BIT_RANGE(SYNOPSYS_I2S_V1_11A_RCR_2,0,2,cfg->data_resolution);
			I2S_REG_RW_BIT(SYNOPSYS_I2S_V1_11A_RER_2,0,1);
			I2S_REG_RW_BIT(SYNOPSYS_I2S_V1_11A_TER_2,0,0);
			I2S_REG_RW_BIT_RANGE(SYNOPSYS_I2S_V1_11A_TCR_2,0,2,cfg->data_resolution);
			I2S_REG_RW_BIT(SYNOPSYS_I2S_V1_11A_TER_2,0,1);
			I2S_REG_RW_BIT(SYNOPSYS_I2S_V1_11A_RER_3,0,0);	
			I2S_REG_RW_BIT_RANGE(SYNOPSYS_I2S_V1_11A_RCR_3,0,2,cfg->data_resolution);
			I2S_REG_RW_BIT(SYNOPSYS_I2S_V1_11A_RER_3,0,1);
			I2S_REG_RW_BIT(SYNOPSYS_I2S_V1_11A_TER_3,0,0);
			I2S_REG_RW_BIT_RANGE(SYNOPSYS_I2S_V1_11A_TCR_3,0,2,cfg->data_resolution);
			I2S_REG_RW_BIT(SYNOPSYS_I2S_V1_11A_TER_3,0,1);
		}
		else if(cfg->data_resolution == DATA_RESOLUTION_24)
		{
			I2S_REG_RW_BIT(SYNOPSYS_I2S_V1_11A_RER_0,0,0);
			I2S_REG_RW_BIT_RANGE(SYNOPSYS_I2S_V1_11A_RCR_0,0,2,cfg->data_resolution);
			I2S_REG_RW_BIT(SYNOPSYS_I2S_V1_11A_RER_0,0,1);
			I2S_REG_RW_BIT(SYNOPSYS_I2S_V1_11A_TER_0,0,0);
			I2S_REG_RW_BIT_RANGE(SYNOPSYS_I2S_V1_11A_TCR_0,0,2,cfg->data_resolution);
			I2S_REG_RW_BIT(SYNOPSYS_I2S_V1_11A_TER_0,0,1);
			I2S_REG_RW_BIT(SYNOPSYS_I2S_V1_11A_RER_1,0,0);
			I2S_REG_RW_BIT_RANGE(SYNOPSYS_I2S_V1_11A_RCR_1,0,2,cfg->data_resolution);
			I2S_REG_RW_BIT(SYNOPSYS_I2S_V1_11A_RER_1,0,1);
			I2S_REG_RW_BIT(SYNOPSYS_I2S_V1_11A_TER_1,0,0);
			I2S_REG_RW_BIT_RANGE(SYNOPSYS_I2S_V1_11A_TCR_1,0,2,cfg->data_resolution);
			I2S_REG_RW_BIT(SYNOPSYS_I2S_V1_11A_TER_1,0,1);
			I2S_REG_RW_BIT(SYNOPSYS_I2S_V1_11A_RER_2,0,0);
			I2S_REG_RW_BIT_RANGE(SYNOPSYS_I2S_V1_11A_RCR_2,0,2,cfg->data_resolution);
			I2S_REG_RW_BIT(SYNOPSYS_I2S_V1_11A_RER_2,0,1);
			I2S_REG_RW_BIT(SYNOPSYS_I2S_V1_11A_TER_2,0,0);
			I2S_REG_RW_BIT_RANGE(SYNOPSYS_I2S_V1_11A_TCR_2,0,2,cfg->data_resolution);
			I2S_REG_RW_BIT(SYNOPSYS_I2S_V1_11A_TER_2,0,1);
			I2S_REG_RW_BIT(SYNOPSYS_I2S_V1_11A_RER_3,0,0);	
			I2S_REG_RW_BIT_RANGE(SYNOPSYS_I2S_V1_11A_RCR_3,0,2,cfg->data_resolution);
			I2S_REG_RW_BIT(SYNOPSYS_I2S_V1_11A_RER_3,0,1);
			I2S_REG_RW_BIT(SYNOPSYS_I2S_V1_11A_TER_3,0,0);
			I2S_REG_RW_BIT_RANGE(SYNOPSYS_I2S_V1_11A_TCR_3,0,2,cfg->data_resolution);
			I2S_REG_RW_BIT(SYNOPSYS_I2S_V1_11A_TER_3,0,1);
		}
		else if(cfg->data_resolution == DATA_RESOLUTION_32)
		{
			I2S_REG_RW_BIT(SYNOPSYS_I2S_V1_11A_RER_0,0,0);
			I2S_REG_RW_BIT_RANGE(SYNOPSYS_I2S_V1_11A_RCR_0,0,2,cfg->data_resolution);
			I2S_REG_RW_BIT(SYNOPSYS_I2S_V1_11A_RER_0,0,1);
			I2S_REG_RW_BIT(SYNOPSYS_I2S_V1_11A_TER_0,0,0);
			I2S_REG_RW_BIT_RANGE(SYNOPSYS_I2S_V1_11A_TCR_0,0,2,cfg->data_resolution);
			I2S_REG_RW_BIT(SYNOPSYS_I2S_V1_11A_TER_0,0,1);
			I2S_REG_RW_BIT(SYNOPSYS_I2S_V1_11A_RER_1,0,0);
			I2S_REG_RW_BIT_RANGE(SYNOPSYS_I2S_V1_11A_RCR_1,0,2,cfg->data_resolution);
			I2S_REG_RW_BIT(SYNOPSYS_I2S_V1_11A_RER_1,0,1);
			I2S_REG_RW_BIT(SYNOPSYS_I2S_V1_11A_TER_1,0,0);
			I2S_REG_RW_BIT_RANGE(SYNOPSYS_I2S_V1_11A_TCR_1,0,2,cfg->data_resolution);
			I2S_REG_RW_BIT(SYNOPSYS_I2S_V1_11A_TER_1,0,1);
			I2S_REG_RW_BIT(SYNOPSYS_I2S_V1_11A_RER_2,0,0);
			I2S_REG_RW_BIT_RANGE(SYNOPSYS_I2S_V1_11A_RCR_2,0,2,cfg->data_resolution);
			I2S_REG_RW_BIT(SYNOPSYS_I2S_V1_11A_RER_2,0,1);
			I2S_REG_RW_BIT(SYNOPSYS_I2S_V1_11A_TER_2,0,0);
			I2S_REG_RW_BIT_RANGE(SYNOPSYS_I2S_V1_11A_TCR_2,0,2,cfg->data_resolution);
			I2S_REG_RW_BIT(SYNOPSYS_I2S_V1_11A_TER_2,0,1);
			I2S_REG_RW_BIT(SYNOPSYS_I2S_V1_11A_RER_3,0,0);	
			I2S_REG_RW_BIT_RANGE(SYNOPSYS_I2S_V1_11A_RCR_3,0,2,cfg->data_resolution);
			I2S_REG_RW_BIT(SYNOPSYS_I2S_V1_11A_RER_3,0,1);
			I2S_REG_RW_BIT(SYNOPSYS_I2S_V1_11A_TER_3,0,0);
			I2S_REG_RW_BIT_RANGE(SYNOPSYS_I2S_V1_11A_TCR_3,0,2,cfg->data_resolution);
			I2S_REG_RW_BIT(SYNOPSYS_I2S_V1_11A_TER_3,0,1);
		}
	}		
}		

int __metal_driver_synopsys_i2s_set_ws_length(struct metal_i2s *i2s,struct metal_i2s_config *cfg)
{
        //only for master mode 
		if(cfg->ws_length == WS_LENGTH_16)
		{
			I2S_REG_RW_BIT(SYNOPSYS_I2S_V1_11A_CER,0,0);
			I2S_REG_RW_BIT_RANGE(SYNOPSYS_I2S_V1_11A_CCR,3,4,cfg->ws_length);
			//I2S_REG_RW_BIT(SYNOPSYS_I2S_V1_11A_CER,0,1);
		}
		else if(cfg->ws_length == WS_LENGTH_24)
		{
	        I2S_REG_RW_BIT(SYNOPSYS_I2S_V1_11A_CER,0,0);
			I2S_REG_RW_BIT_RANGE(SYNOPSYS_I2S_V1_11A_CCR,3,4,cfg->ws_length);
			//I2S_REG_RW_BIT(SYNOPSYS_I2S_V1_11A_CER,0,1);
		}
		else if(cfg->ws_length == WS_LENGTH_32)
		{
			I2S_REG_RW_BIT(SYNOPSYS_I2S_V1_11A_CER,0,0);
            I2S_REG_RW_BIT_RANGE(SYNOPSYS_I2S_V1_11A_CCR,3,4,cfg->ws_length); 
           // I2S_REG_RW_BIT(SYNOPSYS_I2S_V1_11A_CER,0,1);
		}		
		
}		


int __metal_driver_synopsys_i2s_set_sclk_gating(struct metal_i2s *i2s,struct metal_i2s_config *cfg)
{   
	//only for master mode
	if(cfg->sclk_gating == SCLK_GATING_0)
	{
		I2S_REG_RW_BIT(SYNOPSYS_I2S_V1_11A_CER,0,0);
		I2S_REG_RW_BIT_RANGE(SYNOPSYS_I2S_V1_11A_CCR,0,2,cfg->sclk_gating);
		//I2S_REG_RW_BIT(SYNOPSYS_I2S_V1_11A_CER,0,1);
	}	
	else if(cfg->sclk_gating == SCLK_GATING_12)
	{
		I2S_REG_RW_BIT(SYNOPSYS_I2S_V1_11A_CER,0,0);
		I2S_REG_RW_BIT_RANGE(SYNOPSYS_I2S_V1_11A_CCR,0,2,cfg->sclk_gating);
		//I2S_REG_RW_BIT(SYNOPSYS_I2S_V1_11A_CER,0,1);
	}	
	else if(cfg->sclk_gating == SCLK_GATING_16)
	{
		I2S_REG_RW_BIT(SYNOPSYS_I2S_V1_11A_CER,0,0);
		I2S_REG_RW_BIT_RANGE(SYNOPSYS_I2S_V1_11A_CCR,0,2,cfg->sclk_gating);
		//I2S_REG_RW_BIT(SYNOPSYS_I2S_V1_11A_CER,0,1);
	}	
	else if(cfg->sclk_gating == SCLK_GATING_20)
	{
		I2S_REG_RW_BIT(SYNOPSYS_I2S_V1_11A_CER,0,0);
		I2S_REG_RW_BIT_RANGE(SYNOPSYS_I2S_V1_11A_CCR,0,2,cfg->sclk_gating);
		//I2S_REG_RW_BIT(SYNOPSYS_I2S_V1_11A_CER,0,1);
	}	
	else if(cfg->sclk_gating == SCLK_GATING_24)
	{
		I2S_REG_RW_BIT(SYNOPSYS_I2S_V1_11A_CER,0,0);
		I2S_REG_RW_BIT_RANGE(SYNOPSYS_I2S_V1_11A_CCR,0,2,cfg->sclk_gating);
		//I2S_REG_RW_BIT(SYNOPSYS_I2S_V1_11A_CER,0,1);
	}	
}

int __metal_driver_synopsys_i2s_intr_mask(struct metal_i2s *i2s,struct metal_i2s_config *cfg)
{
	if(cfg->intr_mask_en==1)
	{
		if(cfg->no_of_channels==1)
		{
          I2S_REG_RW_BIT_RANGE(SYNOPSYS_I2S_V1_11A_IMR_0,0,5,63);
		}
		else if(cfg->no_of_channels==2)
		{
          I2S_REG_RW_BIT_RANGE(SYNOPSYS_I2S_V1_11A_IMR_0,0,5,63);
          I2S_REG_RW_BIT_RANGE(SYNOPSYS_I2S_V1_11A_IMR_1,0,5,63);
		}
		else if(cfg->no_of_channels==3)
		{
          I2S_REG_RW_BIT_RANGE(SYNOPSYS_I2S_V1_11A_IMR_0,0,5,63);
          I2S_REG_RW_BIT_RANGE(SYNOPSYS_I2S_V1_11A_IMR_1,0,5,63);
          I2S_REG_RW_BIT_RANGE(SYNOPSYS_I2S_V1_11A_IMR_2,0,5,63);
		}
		else if(cfg->no_of_channels==4)
		{
          I2S_REG_RW_BIT_RANGE(SYNOPSYS_I2S_V1_11A_IMR_0,0,5,63);
          I2S_REG_RW_BIT_RANGE(SYNOPSYS_I2S_V1_11A_IMR_1,0,5,63);
          I2S_REG_RW_BIT_RANGE(SYNOPSYS_I2S_V1_11A_IMR_2,0,5,63);
          I2S_REG_RW_BIT_RANGE(SYNOPSYS_I2S_V1_11A_IMR_3,0,5,63);
		}
	}
}	

void __metal_driver_synopsys_i2s_init(struct metal_i2s *i2s,struct metal_i2s_config *cfg)
{      if(cfg->master==1)
	   {
		I2S_REG_RW_BIT(SYNOPSYS_I2S_V1_11A_IER,0,1);
        if(cfg->tx_block_en==1)
        {	
         	 __metal_driver_synopsys_i2s_set_data_resolution((struct metal_i2s *)(i2s),cfg);	
             __metal_driver_synopsys_i2s_set_ws_length((struct metal_i2s *)(i2s),cfg);
             __metal_driver_synopsys_i2s_set_sclk_gating((struct metal_i2s *)(i2s),cfg);
             __metal_driver_synopsys_i2s_intr_mask((struct metal_i2s *)(i2s),cfg);
        }
        else if(cfg->rx_block_en==1)
        {
        	  
             __metal_driver_synopsys_i2s_set_data_resolution((struct metal_i2s *)(i2s),cfg);	
             __metal_driver_synopsys_i2s_set_ws_length((struct metal_i2s *)(i2s),cfg);
             __metal_driver_synopsys_i2s_set_sclk_gating((struct metal_i2s *)(i2s),cfg);
             __metal_driver_synopsys_i2s_intr_mask((struct metal_i2s *)(i2s),cfg);
              
        } 
       }
	   else if (cfg->master==0)
	   {
	   	I2S_REG_RW_BIT(SYNOPSYS_I2S_V1_11A_IER,0,1);
	   	if(cfg->tx_block_en==1)
	   	{
	   		 __metal_driver_synopsys_i2s_set_data_resolution((struct metal_i2s *)(i2s),cfg);
	   		 __metal_driver_synopsys_i2s_intr_mask((struct metal_i2s *)(i2s),cfg);	
	   	}
	   	else if(cfg->rx_block_en==1)
	   	{
	   		   I2S_REG_RW_BIT(SYNOPSYS_I2S_V1_11A_IRER,0,1);
	   		 __metal_driver_synopsys_i2s_set_data_resolution((struct metal_i2s *)(i2s),cfg);
	   		 __metal_driver_synopsys_i2s_intr_mask((struct metal_i2s *)(i2s),cfg);	
	   	}	
	   }	
					
}

int __metal_driver_synopsys_i2s_master_tx(struct metal_i2s *i2s,struct metal_i2s_config *cfg,int len,uint16_t *tx_buf)
{
	
	if(cfg->no_of_channels==1)
	{
		for (int i = 0;i<len;i++ )
		{
			write_register((struct metal_i2s_config *)(cfg),(struct metal_i2s *)(i2s),SYNOPSYS_I2S_V1_11A_LTHR_0,tx_buf[i]);
			i++;
			write_register((struct metal_i2s_config *)(cfg),(struct metal_i2s *)(i2s),SYNOPSYS_I2S_V1_11A_RTHR_0,tx_buf[i]);
		}
		    read_register(cfg,i2s,SYNOPSYS_I2S_V1_11A_ISR_0);
	}
	else if(cfg->no_of_channels==2)
	{
		for (int i = 0;i<len;i++)
		{
			write_register((struct metal_i2s_config *)(cfg),(struct metal_i2s *)(i2s),SYNOPSYS_I2S_V1_11A_LTHR_0,tx_buf[i]);
			i++;
			write_register((struct metal_i2s_config *)(cfg),(struct metal_i2s *)(i2s),SYNOPSYS_I2S_V1_11A_RTHR_0,tx_buf[i]);
			i++;
			write_register((struct metal_i2s_config *)(cfg),(struct metal_i2s *)(i2s),SYNOPSYS_I2S_V1_11A_LTHR_1,tx_buf[i]);
			i++;
			write_register((struct metal_i2s_config *)(cfg),(struct metal_i2s *)(i2s),SYNOPSYS_I2S_V1_11A_RTHR_1,tx_buf[i]);
		}	
	}	
	else if(cfg->no_of_channels==3)
	{
		for (int i = 0;i<len;i++)
		{
			write_register((struct metal_i2s_config *)(cfg),(struct metal_i2s *)(i2s),SYNOPSYS_I2S_V1_11A_LTHR_0,tx_buf[i]);
			i++;
			write_register((struct metal_i2s_config *)(cfg),(struct metal_i2s *)(i2s),SYNOPSYS_I2S_V1_11A_RTHR_0,tx_buf[i]);
			i++;
			write_register((struct metal_i2s_config *)(cfg),(struct metal_i2s *)(i2s),SYNOPSYS_I2S_V1_11A_LTHR_1,tx_buf[i]);
			i++;
			write_register((struct metal_i2s_config *)(cfg),(struct metal_i2s *)(i2s),SYNOPSYS_I2S_V1_11A_RTHR_1,tx_buf[i]);
			i++;
			write_register((struct metal_i2s_config *)(cfg),(struct metal_i2s *)(i2s),SYNOPSYS_I2S_V1_11A_LTHR_2,tx_buf[i]);
			i++;
			write_register((struct metal_i2s_config *)(cfg),(struct metal_i2s *)(i2s),SYNOPSYS_I2S_V1_11A_RTHR_2,tx_buf[i]);
		}	
	}	
	else if(cfg->no_of_channels==4)
	{
		for (int i = 0;i<len;i++)
		{
			write_register((struct metal_i2s_config *)(cfg),(struct metal_i2s *)(i2s),SYNOPSYS_I2S_V1_11A_LTHR_0,tx_buf[i]);
			i++;
			write_register((struct metal_i2s_config *)(cfg),(struct metal_i2s *)(i2s),SYNOPSYS_I2S_V1_11A_RTHR_0,tx_buf[i]);
			i++;
			write_register((struct metal_i2s_config *)(cfg),(struct metal_i2s *)(i2s),SYNOPSYS_I2S_V1_11A_LTHR_1,tx_buf[i]);
			i++;
			write_register((struct metal_i2s_config *)(cfg),(struct metal_i2s *)(i2s),SYNOPSYS_I2S_V1_11A_RTHR_1,tx_buf[i]);
			i++;
			write_register((struct metal_i2s_config *)(cfg),(struct metal_i2s *)(i2s),SYNOPSYS_I2S_V1_11A_LTHR_2,tx_buf[i]);
			i++;
			write_register((struct metal_i2s_config *)(cfg),(struct metal_i2s *)(i2s),SYNOPSYS_I2S_V1_11A_RTHR_2,tx_buf[i]);
			i++;
			write_register((struct metal_i2s_config *)(cfg),(struct metal_i2s *)(i2s),SYNOPSYS_I2S_V1_11A_RTHR_3,tx_buf[i]);
			i++;
			write_register((struct metal_i2s_config *)(cfg),(struct metal_i2s *)(i2s),SYNOPSYS_I2S_V1_11A_RTHR_3,tx_buf[i]);

		}	
	}
	I2S_REG_RW_BIT(SYNOPSYS_I2S_V1_11A_ITER,0,1);
	I2S_REG_RW_BIT(SYNOPSYS_I2S_V1_11A_CER,0,1);

}			

int __metal_driver_synopsys_i2s_slave_rx(struct metal_i2s *i2s,struct metal_i2s_config *cfg,int len,uint16_t *rx_buf)
{


	if(cfg->no_of_channels==1)
	{	
	if((read_register_bit(cfg,i2s,SYNOPSYS_I2S_V1_11A_ISR_0,0))==1)
	{
		for(int i=0;i<len;i++)
		{
			rx_buf[i] = (uint16_t)read_register(cfg,i2s,SYNOPSYS_I2S_V1_11A_LRBR_0);
			i++;
			rx_buf[i] = (uint16_t)read_register(cfg,i2s,SYNOPSYS_I2S_V1_11A_RRBR_0);
		}	
	}
	}
	else if(cfg->no_of_channels==2)
	{	
	if(((read_register_bit(cfg,i2s,SYNOPSYS_I2S_V1_11A_ISR_0,0))==1)&&((read_register_bit(cfg,i2s,SYNOPSYS_I2S_V1_11A_ISR_1,0))==1))
    {
        for(int i=0;i<=len;i++)
        {
        	rx_buf[i] = (uint16_t)read_register(cfg,i2s,SYNOPSYS_I2S_V1_11A_LRBR_0);
			i++;
			rx_buf[i] = (uint16_t)read_register(cfg,i2s,SYNOPSYS_I2S_V1_11A_RRBR_0);
			i++;
			rx_buf[i] = (uint16_t)read_register(cfg,i2s,SYNOPSYS_I2S_V1_11A_LRBR_1);
			i++;
			rx_buf[i] = (uint16_t)read_register(cfg,i2s,SYNOPSYS_I2S_V1_11A_RRBR_1);
        }	
    }
    }
    else if(cfg->no_of_channels==3)
    {
    if(((read_register_bit(cfg,i2s,SYNOPSYS_I2S_V1_11A_ISR_0,0))==1)&&((read_register_bit(cfg,i2s,SYNOPSYS_I2S_V1_11A_ISR_1,0))==1)&&((read_register_bit(cfg,i2s,SYNOPSYS_I2S_V1_11A_ISR_2,0))==1))
    {
        for(int i=0;i<=len;i++)
        {
        	rx_buf[i] = (uint16_t)read_register(cfg,i2s,SYNOPSYS_I2S_V1_11A_LRBR_0);
			i++;
			rx_buf[i] = (uint16_t)read_register(cfg,i2s,SYNOPSYS_I2S_V1_11A_RRBR_0);
			i++;
			rx_buf[i] = (uint16_t)read_register(cfg,i2s,SYNOPSYS_I2S_V1_11A_LRBR_1);
			i++;
			rx_buf[i] = (uint16_t)read_register(cfg,i2s,SYNOPSYS_I2S_V1_11A_RRBR_1);
			i++;
			rx_buf[i] = (uint16_t)read_register(cfg,i2s,SYNOPSYS_I2S_V1_11A_LRBR_2);
			i++;
			rx_buf[i] = (uint16_t)read_register(cfg,i2s,SYNOPSYS_I2S_V1_11A_RRBR_2);
        }	
    }	
    }
    else if(cfg->no_of_channels==4)
    {
    if(((read_register_bit(cfg,i2s,SYNOPSYS_I2S_V1_11A_ISR_0,0))==1)&&((read_register_bit(cfg,i2s,SYNOPSYS_I2S_V1_11A_ISR_1,0))==1)&&((read_register_bit(cfg,i2s,SYNOPSYS_I2S_V1_11A_ISR_2,0))==1)&&((read_register_bit(cfg,i2s,SYNOPSYS_I2S_V1_11A_ISR_3,0))==1))
    {
        for(int i=0;i<=len;i++)
        {
        	rx_buf[i] = (uint16_t)read_register(cfg,i2s,SYNOPSYS_I2S_V1_11A_LRBR_0);
			i++;
			rx_buf[i] = (uint16_t)read_register(cfg,i2s,SYNOPSYS_I2S_V1_11A_RRBR_0);
			i++;
			rx_buf[i] = (uint16_t)read_register(cfg,i2s,SYNOPSYS_I2S_V1_11A_LRBR_1);
			i++;
			rx_buf[i] = (uint16_t)read_register(cfg,i2s,SYNOPSYS_I2S_V1_11A_RRBR_1);
			i++;
			rx_buf[i] = (uint16_t)read_register(cfg,i2s,SYNOPSYS_I2S_V1_11A_LRBR_2);
			i++;
			rx_buf[i] = (uint16_t)read_register(cfg,i2s,SYNOPSYS_I2S_V1_11A_RRBR_2);
			i++;
			rx_buf[i] = (uint16_t)read_register(cfg,i2s,SYNOPSYS_I2S_V1_11A_LRBR_3);
			i++;
			rx_buf[i] = (uint16_t)read_register(cfg,i2s,SYNOPSYS_I2S_V1_11A_RRBR_3);
        }	
    }	
    }
	
}


__METAL_DEFINE_VTABLE(__metal_driver_vtable_synopsys_i2s) =
{

	.i2s.init				= __metal_driver_synopsys_i2s_init,
	.i2s.master_tx 	        = __metal_driver_synopsys_i2s_master_tx,
	.i2s.slave_rx 		    = __metal_driver_synopsys_i2s_slave_rx,
	.i2s.set_data_resolution= __metal_driver_synopsys_i2s_set_data_resolution,
	.i2s.set_ws_length 	    = __metal_driver_synopsys_i2s_set_ws_length,
	.i2s.set_sclk_gating    = __metal_driver_synopsys_i2s_set_sclk_gating,
	.i2s.intr_mask          = __metal_driver_synopsys_i2s_intr_mask,
	//.i2c.get_interrupt_controller = __metal_driver_synopsis_i2c_get_interrupt_controller,
	//.i2c.get_interrupt_id   = __metal_driver_synopsis_i2c_get_interrupt_id
};

#endif