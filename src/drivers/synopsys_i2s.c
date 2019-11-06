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
#define SYNOPSYS_I2S_V1_11A_LRBRx 0X020 //0X020+(0X040*X)
//#define SYNOPSYS_I2S_V1_11A_LRBR_1 0X060 
//#define SYNOPSYS_I2S_V1_11A_LRBR_2 0X0A0
//#define SYNOPSYS_I2S_V1_11A_LRBR_3 0X0E0
#define SYNOPSYS_I2S_V1_11A_LTHRx 0X020
//#define SYNOPSYS_I2S_V1_11A_LTHR_1 0X060
//#define SYNOPSYS_I2S_V1_11A_LTHR_2 0X0A0
//#define SYNOPSYS_I2S_V1_11A_LTHR_3 0X0E0
#define SYNOPSYS_I2S_V1_11A_RRBRx 0X024
//#define SYNOPSYS_I2S_V1_11A_RRBR_1 0X064
//#define SYNOPSYS_I2S_V1_11A_RRBR_2 0X0A4
//#define SYNOPSYS_I2S_V1_11A_RRBR_3 0X0E4
#define SYNOPSYS_I2S_V1_11A_RTHRx 0X024
/*#define SYNOPSYS_I2S_V1_11A_RTHR_1 0X064
#define SYNOPSYS_I2S_V1_11A_RTHR_2 0X0A4
#define SYNOPSYS_I2S_V1_11A_RTHR_3 0X0E4*/
#define SYNOPSYS_I2S_V1_11A_RERx 0X028 
/*#define SYNOPSYS_I2S_V1_11A_RER_1 0X068
#define SYNOPSYS_I2S_V1_11A_RER_2 0X0A8
#define SYNOPSYS_I2S_V1_11A_RER_3 0X0E8*/
#define SYNOPSYS_I2S_V1_11A_TERx 0X02C
/*#define SYNOPSYS_I2S_V1_11A_TER_1 0X06C
#define SYNOPSYS_I2S_V1_11A_TER_2 0X0AC
#define SYNOPSYS_I2S_V1_11A_TER_3 0X0EC*/
#define SYNOPSYS_I2S_V1_11A_RCRx 0X030
/*#define SYNOPSYS_I2S_V1_11A_RCR_1 0X070
#define SYNOPSYS_I2S_V1_11A_RCR_2 0X0B0
#define SYNOPSYS_I2S_V1_11A_RCR_3 0X0F0*/
#define SYNOPSYS_I2S_V1_11A_TCRx 0X034
/*#define SYNOPSYS_I2S_V1_11A_TCR_1 0X074
#define SYNOPSYS_I2S_V1_11A_TCR_2 0X0B4
#define SYNOPSYS_I2S_V1_11A_TCR_3 0X0F4*/
#define SYNOPSYS_I2S_V1_11A_ISRx 0X038
/*#define SYNOPSYS_I2S_V1_11A_ISR_1 0X078
#define SYNOPSYS_I2S_V1_11A_ISR_2 0X0B8
#define SYNOPSYS_I2S_V1_11A_ISR_3 0X0F8*/
#define SYNOPSYS_I2S_V1_11A_IMRx 0X03C
/*#define SYNOPSYS_I2S_V1_11A_IMR_1 0X07C
#define SYNOPSYS_I2S_V1_11A_IMR_2 0X0BC
#define SYNOPSYS_I2S_V1_11A_IMR_3 0X0FC*/
#define SYNOPSYS_I2S_V1_11A_RORx 0X040
/*#define SYNOPSYS_I2S_V1_11A_ROR_1 0X080
#define SYNOPSYS_I2S_V1_11A_ROR_2 0X0C0
#define SYNOPSYS_I2S_V1_11A_ROR_3 0X100*/
#define SYNOPSYS_I2S_V1_11A_TORx 0X044
/*#define SYNOPSYS_I2S_V1_11A_TOR_1 0X084
#define SYNOPSYS_I2S_V1_11A_TOR_2 0X0C4
#define SYNOPSYS_I2S_V1_11A_TOR_3 0X104*/
#define SYNOPSYS_I2S_V1_11A_RFCRx 0X048
/*#define SYNOPSYS_I2S_V1_11A_RFCR_1 0X088
#define SYNOPSYS_I2S_V1_11A_RFCR_2 0X0C8
#define SYNOPSYS_I2S_V1_11A_RFCR_3 0X108*/
#define SYNOPSYS_I2S_V1_11A_TFCRx 0X04C
/*#define SYNOPSYS_I2S_V1_11A_TFCR_1 0X08C
#define SYNOPSYS_I2S_V1_11A_TFCR_2 0X0CC
#define SYNOPSYS_I2S_V1_11A_TFCR_3 0X10C*/
#define SYNOPSYS_I2S_V1_11A_RFFx 0X050
/*#define SYNOPSYS_I2S_V1_11A_RFF_1 0X090
#define SYNOPSYS_I2S_V1_11A_RFF_2 0X0D0
#define SYNOPSYS_I2S_V1_11A_RFF_3 0X110*/
#define SYNOPSYS_I2S_V1_11A_TFFx 0X054
/*#define SYNOPSYS_I2S_V1_11A_TFF_1 0X094
#define SYNOPSYS_I2S_V1_11A_TFF_2 0X0D4
#define SYNOPSYS_I2S_V1_11A_TFF_3 0X114*/
#define SYNOPSYS_I2S_V1_11A_RXDMA 0X1C0
#define SYNOPSYS_I2S_V1_11A_RRXDMA 0X1C4
#define SYNOPSYS_I2S_V1_11A_TXDMA 0X1C8
#define SYNOPSYS_I2S_V1_11A_RTXDMA 0X1CC
#define SYNOPSYS_I2S_V1_11A_COMP_PARAM_2 0X1F0
#define SYNOPSYS_I2S_V1_11A_COMP_PARAM_1 0X1F4
#define SYNOPSYS_I2S_V1_11A_COMP_VERSION 0X1F8
#define SYNOPSYS_I2S_V1_11A_COMP_TYPE 0X1FC
#define SYNOPSYS_I2S_V1_11A_DMACR 0X200
#define SYNOPSYS_I2S_V1_11A_RXDMA_CH 0X204
/*#define SYNOPSYS_I2S_V1_11A_RXDMA_CH_1 0X204
#define SYNOPSYS_I2S_V1_11A_RXDMA_CH_2 0X204
#define SYNOPSYS_I2S_V1_11A_RXDMA_CH_3 0X204*/
#define SYNOPSYS_I2S_V1_11A_TXDMA_CH 0X214
/*#define SYNOPSYS_I2S_V1_11A_TXDMA_CH_1 0X214
#define SYNOPSYS_I2S_V1_11A_TXDMA_CH_2 0X214
#define SYNOPSYS_I2S_V1_11A_TXDMA_CH_3 0X214*/
/////////////////////////////////////////
#define REGISTER_DATATYPE uint32_t
#define INTERFACE_SIZE 32
#define REGISTER_SIZE 32

#define I2S_REG(TYPE,offset) ((unsigned long)((__metal_driver_synopsys_i2s_##TYPE##_control_base((struct metal_i2s *)i2s)) + offset))
#define I2S_REGW(TYPE,offset) (__METAL_ACCESS_ONCE((__metal_io_u32 *)I2S_REG(TYPE,offset)))
#define I2S_REG_RW_BIT(offset,bit,val) (write_register(i2s,cfg,offset,set_bit(read_register(i2s,cfg,offset),bit,val)))
#define I2S_REG_RW_BIT_RANGE(offset,start_bit,stop_bit,val) (write_register(i2s,cfg,offset,set_bit_range(read_register(i2s,cfg,offset),start_bit,stop_bit,val)))

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

REGISTER_DATATYPE split_lsb(REGISTER_DATATYPE value, int bit)
{
		return (value & ((1<<bit)-1));				
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

void write_register(struct metal_i2s *i2s,struct metal_i2s_config *cfg,REGISTER_DATATYPE offset,REGISTER_DATATYPE value)
{
	
			if(cfg->master==1)
			{
			I2S_REGW(master,offset) =  value;
			}
			else
			{
			I2S_REGW(slave,offset) =  value;	
			}
	
}	


REGISTER_DATATYPE read_register(struct metal_i2s *i2s,struct metal_i2s_config *cfg,REGISTER_DATATYPE offset)
{
	        if(cfg->master==1)
			{
			I2S_REGW(master,offset);
		    }
		    else
		    {
		    I2S_REGW(slave,offset);	
		    }
}

REGISTER_DATATYPE read_register_bit(struct metal_i2s *i2s,struct metal_i2s_config *cfg,REGISTER_DATATYPE offset,int bit)
{
	return ((read_register(i2s,cfg,offset) & (1<<bit))>>bit);
}

REGISTER_DATATYPE read_register_bit_range(struct metal_i2s *i2s,struct metal_i2s_config *cfg,REGISTER_DATATYPE offset,int start_bit,int stop_bit)
{
	return ((read_register(i2s,cfg,offset) & ((1<<(stop_bit-start_bit+1)-1)<<start_bit))>>start_bit);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int __metal_driver_synopsys_i2s_set_data_resolution(struct metal_i2s *i2s,struct metal_i2s_config *cfg)
{  
if(cfg->tx_block_en)
	{
		write_register((struct metal_i2s *)(i2s),(struct metal_i2s_config *)(cfg),SYNOPSYS_I2S_V1_11A_TERx,0);
	    write_register((struct metal_i2s *)(i2s),(struct metal_i2s_config *)(cfg),(SYNOPSYS_I2S_V1_11A_TCRx),cfg->data_resolution);
    	write_register((struct metal_i2s *)(i2s),(struct metal_i2s_config *)(cfg),SYNOPSYS_I2S_V1_11A_TERx,1);
    }

if(cfg->rx_block_en)
	{
			write_register((struct metal_i2s *)(i2s),(struct metal_i2s_config *)(cfg),SYNOPSYS_I2S_V1_11A_RERx,0);
			write_register((struct metal_i2s *)(i2s),(struct metal_i2s_config *)(cfg),(SYNOPSYS_I2S_V1_11A_RCRx),cfg->data_resolution);
			write_register((struct metal_i2s *)(i2s),(struct metal_i2s_config *)(cfg),SYNOPSYS_I2S_V1_11A_RERx,1);
	}
}		

int __metal_driver_synopsys_i2s_set_ws_length(struct metal_i2s *i2s,struct metal_i2s_config *cfg)
{
        //only for master mode 
		    I2S_REG_RW_BIT(SYNOPSYS_I2S_V1_11A_CER,0,0);
			I2S_REG_RW_BIT_RANGE(SYNOPSYS_I2S_V1_11A_CCR,3,4,cfg->ws_length);
}		


int __metal_driver_synopsys_i2s_set_sclk_gating(struct metal_i2s *i2s,struct metal_i2s_config *cfg)
{   
	//only for master mode
		I2S_REG_RW_BIT(SYNOPSYS_I2S_V1_11A_CER,0,0);
		I2S_REG_RW_BIT_RANGE(SYNOPSYS_I2S_V1_11A_CCR,0,2,cfg->sclk_gating);		
}

int __metal_driver_synopsys_i2s_interrupt_enable(struct metal_i2s *i2s,struct metal_i2s_config *cfg,int interrupt_number)
{
	unsigned int x=cfg->no_of_channels-1;

	
		for(int i=0;i<=x;i++)
		{
          if(interrupt_number == 4){
          I2S_REG_RW_BIT((SYNOPSYS_I2S_V1_11A_IMRx+0x40*x),4,0);}
          else if (interrupt_number == 2) {
          I2S_REG_RW_BIT((SYNOPSYS_I2S_V1_11A_IMRx+0x40*x),0,0);}
          else if (interrupt_number ==3 ){
          I2S_REG_RW_BIT((SYNOPSYS_I2S_V1_11A_IMRx+0x40*x),5,0);}
          else if (interrupt_number == 1){
          I2S_REG_RW_BIT((SYNOPSYS_I2S_V1_11A_IMRx+0x40*x),1,0);}
		}
	
		
}	

int __metal_driver_synopsys_i2s_interrupt_disable(struct metal_i2s *i2s,struct metal_i2s_config *cfg, int interrupt_number)
{
	unsigned int x=cfg->no_of_channels-1;

		for(int i=0;i<=x;i++)
		{
			if(interrupt_number == 4){
          I2S_REG_RW_BIT((SYNOPSYS_I2S_V1_11A_IMRx+0x40*x),4,1);}
          else if (interrupt_number == 2) {
          I2S_REG_RW_BIT((SYNOPSYS_I2S_V1_11A_IMRx+0x40*x),0,1);}
          else if (interrupt_number ==3 ){
          I2S_REG_RW_BIT((SYNOPSYS_I2S_V1_11A_IMRx+0x40*x),5,1);}
          else if (interrupt_number == 1){
          I2S_REG_RW_BIT((SYNOPSYS_I2S_V1_11A_IMRx+0x40*x),1,1);}
		}
	}
		
int __metal_driver_synopsys_i2s_get_interrupt_id(struct metal_i2s *i2s,struct metal_i2s_config *cfg,int num)
{
	if (num>0)
	{
		if(cfg->master==1)
			{
			return (__metal_driver_synopsys_i2s_master_interrupt_line(i2s));
		    }
		    else
		    {
		    return(__metal_driver_synopsys_i2s_slave_interrupt_line(i2s));	
		    }
	}
}	

void __metal_driver_synopsys_i2s_global_enable(struct metal_i2s *i2s,struct metal_i2s_config *cfg)
{
	write_register((struct metal_i2s *)(i2s),(struct metal_i2s_config *)(cfg),(SYNOPSYS_I2S_V1_11A_IER),1);
}

void __metal_driver_synopsys_i2s_global_disable(struct metal_i2s *i2s,struct metal_i2s_config *cfg)
{
	write_register((struct metal_i2s *)(i2s),(struct metal_i2s_config *)(cfg),(SYNOPSYS_I2S_V1_11A_IER),0);
}


int __metal_driver_synopsys_i2s_set_fifo_trigger_level(struct metal_i2s *i2s,struct metal_i2s_config *cfg)
{
	unsigned int x=cfg->no_of_channels-1;

	if(cfg->tx_block_en==1)
	{
		for(int i=0;i<=x;i++)
		{
		  write_register((struct metal_i2s *)(i2s),(struct metal_i2s_config *)(cfg),(SYNOPSYS_I2S_V1_11A_TERx+0x40*x),0);	
		  
		  write_register((struct metal_i2s *)(i2s),(struct metal_i2s_config *)(cfg),(SYNOPSYS_I2S_V1_11A_TFCRx+0x40*x),cfg->tx_fifo_trigger_level); 	
         
          write_register((struct metal_i2s *)(i2s),(struct metal_i2s_config *)(cfg),(SYNOPSYS_I2S_V1_11A_TERx+0x40*x),1);
          
		}
	}
	
	if(cfg->rx_block_en==1)
	{
		for(int i=0;i<=x;i++)
		{
	      write_register((struct metal_i2s *)(i2s),(struct metal_i2s_config *)(cfg),(SYNOPSYS_I2S_V1_11A_RERx+0x40*x),0);
		  
		  write_register((struct metal_i2s *)(i2s),(struct metal_i2s_config *)(cfg),(SYNOPSYS_I2S_V1_11A_RFCRx+0x40*x),cfg->rx_fifo_trigger_level);
         
          write_register((struct metal_i2s *)(i2s),(struct metal_i2s_config *)(cfg),(SYNOPSYS_I2S_V1_11A_RERx+0x40*x),1);
          
		}
	}	
}	

void __metal_driver_synopsys_i2s_init(struct metal_i2s *i2s,struct metal_i2s_config *cfg)
{     
	    
		
      __metal_driver_synopsys_i2s_set_data_resolution((struct metal_i2s *)(i2s),cfg);

      write_register((struct metal_i2s *)(i2s),(struct metal_i2s_config *)(cfg),(SYNOPSYS_I2S_V1_11A_IMRx),0xFFFF); 
      __metal_driver_synopsys_i2s_set_fifo_trigger_level((struct metal_i2s *)(i2s),cfg);

   if(cfg->master==1)
	   {	
             __metal_driver_synopsys_i2s_set_ws_length((struct metal_i2s *)(i2s),cfg);
             __metal_driver_synopsys_i2s_set_sclk_gating((struct metal_i2s *)(i2s),cfg);
            
       }  
     if(cfg->rx_block_en)
     {
     	    write_register((struct metal_i2s *)(i2s),(struct metal_i2s_config *)(cfg),(SYNOPSYS_I2S_V1_11A_IRER),1);		
     }
					
}


struct metal_interrupt* __metal_driver_synopsys_i2s_get_interrupt_controller(struct metal_i2s *i2s,struct metal_i2s_config *cfg)
{
		if(cfg->master==1)
			{
			return (__metal_driver_synopsys_i2s_master_interrupt_parent(i2s));
		    }
		    else
		    {
		    return(__metal_driver_synopsys_i2s_slave_interrupt_parent(i2s));	
		    }
	}

int __metal_driver_synopsys_i2s_get_intr_num(struct metal_i2s *i2s, struct metal_i2s_config *cfg)
{
	int temp0 = read_register(i2s,cfg,SYNOPSYS_I2S_V1_11A_ISRx);

	if (((temp0>>1)&1) == 1) {return 1;}
	else if  (((temp0)&1) == 1) {return 2;}
	else if (((temp0>>5)&1) == 1) {return 3;}
	else if (((temp0>>4)&1) == 1) {return 4;}
}

int __metal_driver_synopsys_i2s_tx_enable(struct metal_i2s *i2s,struct metal_i2s_config *cfg)
{
	write_register((struct metal_i2s *)(i2s),(struct metal_i2s_config *)(cfg),(SYNOPSYS_I2S_V1_11A_ITER),1);
}

int __metal_driver_synopsys_i2s_tx_disable(struct metal_i2s *i2s,struct metal_i2s_config *cfg)
{
	write_register((struct metal_i2s *)(i2s),(struct metal_i2s_config *)(cfg),(SYNOPSYS_I2S_V1_11A_ITER),0);
}

int __metal_driver_synopsys_i2s_rx_enable(struct metal_i2s *i2s,struct metal_i2s_config *cfg)
{
	write_register((struct metal_i2s *)(i2s),(struct metal_i2s_config *)(cfg),(SYNOPSYS_I2S_V1_11A_IRER),1);
}

int __metal_driver_synopsys_i2s_rx_disable(struct metal_i2s *i2s,struct metal_i2s_config *cfg)
{
	write_register((struct metal_i2s *)(i2s),(struct metal_i2s_config *)(cfg),(SYNOPSYS_I2S_V1_11A_IRER),0);
}

int __metal_driver_synopsys_i2s_clock_enable(struct metal_i2s *i2s,struct metal_i2s_config *cfg)
{
	write_register((struct metal_i2s *)(i2s),(struct metal_i2s_config *)(cfg),(SYNOPSYS_I2S_V1_11A_CER),1);
}

int __metal_driver_synopsys_i2s_clock_disable(struct metal_i2s *i2s,struct metal_i2s_config *cfg)
{
	write_register((struct metal_i2s *)(i2s),(struct metal_i2s_config *)(cfg),(SYNOPSYS_I2S_V1_11A_CER),0);
}
	


//put data
int __metal_driver_synopsys_i2s_put_data(struct metal_i2s *i2s, struct metal_i2s_config *cfg,int len,uint32_t *tx_buf)
{
	unsigned int x=cfg->no_of_channels-1;

for(int i=0;i<len;i++)
	{
		for(int j=0;j<=x;j++)
		{
			if(i<len){
			write_register((struct metal_i2s *)(i2s),(struct metal_i2s_config *)(cfg),(SYNOPSYS_I2S_V1_11A_LTHRx+0x40*x),tx_buf[i]);
			i++;}
			if(i<len){
			write_register((struct metal_i2s *)(i2s),(struct metal_i2s_config *)(cfg),(SYNOPSYS_I2S_V1_11A_RTHRx+0x40*x),tx_buf[i]);
            i++;}
           // read_register(i2s,cfg,(SYNOPSYS_I2S_V1_11A_ISRx+0x40*x));
		}
		i--;
	}
	//return len;
}

//get data
int __metal_driver_synopsys_i2s_get_data(struct metal_i2s *i2s, struct metal_i2s_config *cfg,int len,uint32_t *rx_buf)
{
	
	while(read_register_bit(i2s,cfg,SYNOPSYS_I2S_V1_11A_ISRx,0))
	{
		rx_buf[len] = read_register(i2s,cfg,(SYNOPSYS_I2S_V1_11A_LRBRx));
		++len;
		rx_buf[len] = read_register(i2s,cfg,(SYNOPSYS_I2S_V1_11A_RRBRx));
		++len;
	}

	for(int i=0;i<cfg->rx_fifo_trigger_level;i++)
	{
		rx_buf[len] = read_register(i2s,cfg,(SYNOPSYS_I2S_V1_11A_LRBRx));
		len++;
		rx_buf[len] = read_register(i2s,cfg,(SYNOPSYS_I2S_V1_11A_RRBRx));
		len++;
	}

	return len;
}

void __metal_driver_synopsys_i2s_load_buffer(struct metal_i2s *i2s,uint32_t *tx_buf,int data,unsigned char *buffer_len)
{	
	unsigned char len = *buffer_len;
	tx_buf[len]=data;
	*buffer_len  = ++len;	
}

void __metal_driver_synopsys_i2s_global_interrupt_clear(struct metal_i2s *i2s, struct metal_i2s_config *cfg,int interrupt_number)
{
	if(interrupt_number == 3)
	{
			read_register_bit(i2s,cfg,(SYNOPSYS_I2S_V1_11A_TORx),0);
	}

	if(interrupt_number == 1)
	{
			read_register_bit(i2s,cfg,(SYNOPSYS_I2S_V1_11A_RORx),0);
	}
}

void __metal_driver_synopsys_i2s_tx_fifo_flush(struct metal_i2s *i2s, struct metal_i2s_config *cfg)
{
	write_register((struct metal_i2s *)(i2s),(struct metal_i2s_config *)(cfg),SYNOPSYS_I2S_V1_11A_TFFx,1);
}

void __metal_driver_synopsys_i2s_rx_fifo_flush(struct metal_i2s *i2s, struct metal_i2s_config *cfg)
{
	write_register((struct metal_i2s *)(i2s),(struct metal_i2s_config *)(cfg),SYNOPSYS_I2S_V1_11A_RFFx,1);
}


__METAL_DEFINE_VTABLE(__metal_driver_vtable_synopsys_i2s) =
{

	.i2s.init				= __metal_driver_synopsys_i2s_init,
	.i2s.set_data_resolution= __metal_driver_synopsys_i2s_set_data_resolution,
	.i2s.set_ws_length 	    = __metal_driver_synopsys_i2s_set_ws_length,
	.i2s.set_sclk_gating    = __metal_driver_synopsys_i2s_set_sclk_gating,
	.i2s.set_fifo_trigger_level= __metal_driver_synopsys_i2s_set_fifo_trigger_level,
	.i2s.interrupt_enable   = __metal_driver_synopsys_i2s_interrupt_enable,
	.i2s.interrupt_disable          = __metal_driver_synopsys_i2s_interrupt_disable,
	.i2s.get_intr_id        = __metal_driver_synopsys_i2s_get_interrupt_id,
	.i2s.get_interrupt_controller = __metal_driver_synopsys_i2s_get_interrupt_controller,
	.i2s.get_intr_num       = __metal_driver_synopsys_i2s_get_intr_num,
	.i2s.tx_enable          = __metal_driver_synopsys_i2s_tx_enable,
	.i2s.tx_disable         = __metal_driver_synopsys_i2s_tx_disable,
	.i2s.rx_enable          = __metal_driver_synopsys_i2s_rx_enable,
	.i2s.rx_disable         = __metal_driver_synopsys_i2s_rx_disable,
	.i2s.clock_enable       = __metal_driver_synopsys_i2s_clock_enable,
	.i2s.clock_disable      = __metal_driver_synopsys_i2s_clock_disable,
	.i2s.put_data           = __metal_driver_synopsys_i2s_put_data,
	.i2s.get_data           = __metal_driver_synopsys_i2s_get_data,
	.i2s.global_enable      = __metal_driver_synopsys_i2s_global_enable,
	.i2s.global_disable     = __metal_driver_synopsys_i2s_global_disable,
	.i2s.load_buffer        = __metal_driver_synopsys_i2s_load_buffer,
	.i2s.global_interrupt_clear = __metal_driver_synopsys_i2s_global_interrupt_clear,
	.i2s.tx_fifo_flush      = __metal_driver_synopsys_i2s_tx_fifo_flush,
	.i2s.rx_fifo_flush      = __metal_driver_synopsys_i2s_rx_fifo_flush,
	//.i2s.get_interrupt_id   = __metal_driver_synopsis_i2s_get_interrupt_id,
};

#endif
