/* Copyright 2020 SiFive, Inc */
/* SPDX-License-Identifier: Apache-2.0 */

#include <metal/machine/platform.h>

#ifndef METAL_SIFIVE_NB2_I2C_DRIVER
#define METAL_SIFIVE_NB2_I2C_DRIVER

#include <metal/drivers/sifive_nb2i2c0.h>
#include <metal/machine.h>
#include <metal/io.h>
#include <metal/clock.h>


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#define REGISTER_DATATYPE uint32_t


#define I2C_REG(offset) ((unsigned long)((__metal_driver_synopsys_i2c_control_base(i2c)) + offset))
#define I2C_REGB(offset) (__METAL_ACCESS_ONCE((REGISTER_DATATYPE *)I2C_REG(offset)))
#define I2C_REG_RW_BIT(offset,bit,val) (write_register(i2c,offset,set_bit(read_register(i2c,offset),bit,val)))
#define I2C_REG_RW_BIT_RANGE(offset,start_bit,stop_bit,val) (write_register(i2c,offset,set_bit_range(read_register(i2c,offset),start_bit,stop_bit,val)))

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//The link between vtables
/*struct __metal_driver_synopsys_i2c __metal_dt_apbi2c_60000 ={
    .i2c.vtable = &__metal_driver_vtable_synopsys_i2c.i2c,
};
*/



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

void __metal_driver_synopsys_i2c_spike_suppression_configuration(struct metal_i2c *i2c,struct metal_i2c_config *cfg)
{
   struct metal_clock *clock_h = __metal_driver_synopsys_i2c_clock(i2c);
   double long max_freq;
   uint32_t val;

   if(cfg->speed_mode == METAL_I2C_HIGH_SPEED)
   {
      max_freq = 100000000; //1/10ns
      val = metal_clock_get_rate_hz(clock_h)/max_freq;
      write_register(i2c,METAL_SYNOPSYS_I2C_V2_02A_STANDARD_IC_HS_SPKLEN, val);

      max_freq = 20000000;
      val = metal_clock_get_rate_hz(clock_h)/max_freq;
      write_register(i2c,METAL_SYNOPSYS_I2C_V2_02A_STANDARD_IC_FS_SPKLEN, val);
   }
   else
   {
      max_freq = 20000000; //1/50ns
      val = metal_clock_get_rate_hz(clock_h)/max_freq;
      write_register(i2c,METAL_SYNOPSYS_I2C_V2_02A_STANDARD_IC_FS_SPKLEN, val);

   }

}
void __metal_driver_synopsys_i2c_hold_time_config(struct metal_i2c *i2c,struct metal_i2c_config *cfg,double long clock_rate)
{
   uint32_t sda_hold;

   if(cfg->speed_mode == METAL_I2C_STANDARD_SPEED || cfg->speed_mode == METAL_I2C_FAST_SPEED || cfg->speed_mode == METAL_I2C_FAST_PLUS_SPEED)
   {
      sda_hold = (uint32_t)(clock_rate/3333333); // 1/300ns
   }
   else if(cfg->speed_mode == METAL_I2C_HIGH_SPEED)
   {
      sda_hold = (uint32_t)(clock_rate/14285714); // 1/70ns
   }

   sda_hold = ((sda_hold & 0xffff) << 16) | sda_hold;
   write_register(i2c,METAL_SYNOPSYS_I2C_V2_02A_STANDARD_IC_SDA_HOLD,sda_hold);

}

void __metal_driver_synopsys_i2c_setup_time_config(struct metal_i2c *i2c,struct metal_i2c_config *cfg,double long clock_rate)
{

   if(cfg->speed_mode == METAL_I2C_STANDARD_SPEED)
   {

      write_register(i2c,METAL_SYNOPSYS_I2C_V2_02A_STANDARD_IC_SDA_SETUP,((uint32_t)(clock_rate/4000000))); //1/250ns
   }
   else if(cfg->speed_mode == METAL_I2C_FAST_SPEED)
   {
      write_register(i2c,METAL_SYNOPSYS_I2C_V2_02A_STANDARD_IC_SDA_SETUP,((uint32_t)(clock_rate/10000000))); //1/100ns
   }
   else if(cfg->speed_mode == METAL_I2C_FAST_PLUS_SPEED)
   {
      write_register(i2c,METAL_SYNOPSYS_I2C_V2_02A_STANDARD_IC_SDA_SETUP,((uint32_t)(clock_rate/20000000))); //1/50ns
   }
   else if(cfg->speed_mode == METAL_I2C_HIGH_SPEED)
   {

      write_register(i2c,METAL_SYNOPSYS_I2C_V2_02A_STANDARD_IC_SDA_SETUP,((uint32_t)(clock_rate/100000000))); //1/10ns
   }
}

void __metal_driver_synopsys_i2c_clock_config(struct metal_i2c *i2c,struct metal_i2c_config *cfg)
{
   struct metal_clock *clock_h = __metal_driver_synopsys_i2c_clock(i2c);
   unsigned int count;
   long double min_low_in_hz,min_low_count, high_count;
   double long clock_rate_in_hz = metal_clock_get_rate_hz(clock_h);
   __metal_driver_synopsys_i2c_hold_time_config(i2c,cfg,clock_rate_in_hz);
   __metal_driver_synopsys_i2c_setup_time_config(i2c,cfg,clock_rate_in_hz);


   if(cfg->speed_mode == METAL_I2C_STANDARD_SPEED)
   {
      count = clock_rate_in_hz/100000;  // 100KHz
      min_low_in_hz =208333;            // 4.7us
      min_low_count =  clock_rate_in_hz/min_low_in_hz;

      write_register(i2c,METAL_SYNOPSYS_I2C_V2_02A_STANDARD_IC_SS_SCL_LCNT,min_low_count-1);

      high_count =((count-min_low_count)-(I2C_REGB(METAL_SYNOPSYS_I2C_V2_02A_STANDARD_IC_FS_SPKLEN)+7));

      write_register(i2c,METAL_SYNOPSYS_I2C_V2_02A_STANDARD_IC_SS_SCL_HCNT,high_count);

   }
   else if((cfg->speed_mode == METAL_I2C_FAST_SPEED)  || (cfg->speed_mode == METAL_I2C_FAST_PLUS_SPEED))
   {
      if(cfg->speed_mode == METAL_I2C_FAST_PLUS_SPEED)
      {

         min_low_in_hz =2000000;           // 0.5us
         count = clock_rate_in_hz/1000000;  // 1MHz
      }
      else
      {
        min_low_in_hz =769230; // 1.3us
        count = clock_rate_in_hz/400000;  // 400KHz
      }

      min_low_count =  clock_rate_in_hz/min_low_in_hz;

      write_register(i2c,METAL_SYNOPSYS_I2C_V2_02A_STANDARD_IC_FS_SCL_LCNT,(min_low_count-1));

      high_count =((count-min_low_count)-(I2C_REGB(METAL_SYNOPSYS_I2C_V2_02A_STANDARD_IC_FS_SPKLEN)+7));

      write_register(i2c,METAL_SYNOPSYS_I2C_V2_02A_STANDARD_IC_FS_SCL_HCNT,high_count);

   }
   else if(cfg->speed_mode == METAL_I2C_HIGH_SPEED)
   {
      min_low_in_hz = 770100;  // 1.3us

      count = clock_rate_in_hz/400000;  //400KHz

      min_low_count =  clock_rate_in_hz/min_low_in_hz;

      write_register(i2c,METAL_SYNOPSYS_I2C_V2_02A_STANDARD_IC_FS_SCL_LCNT,min_low_count);

      high_count =((count-min_low_count)-(I2C_REGB(METAL_SYNOPSYS_I2C_V2_02A_STANDARD_IC_FS_SPKLEN)+7));

      write_register(i2c,METAL_SYNOPSYS_I2C_V2_02A_STANDARD_IC_FS_SCL_HCNT,high_count);

      min_low_in_hz = 5000000;               // 200ns
      count = clock_rate_in_hz/3400000;      // 3.3MHz

     min_low_count =  (clock_rate_in_hz)/(min_low_in_hz);

      write_register(i2c,METAL_SYNOPSYS_I2C_V2_02A_STANDARD_IC_HS_SCL_LCNT,(min_low_count-1));

      high_count =(min_low_count/2)-((I2C_REGB(METAL_SYNOPSYS_I2C_V2_02A_STANDARD_IC_HS_SPKLEN)+7));


      write_register(i2c,METAL_SYNOPSYS_I2C_V2_02A_STANDARD_IC_HS_SCL_HCNT,high_count);
   }
}

void __metal_driver_synopsys_i2c_enable_i2c(struct metal_i2c *i2c)
{
   I2C_REG_RW_BIT(METAL_SYNOPSYS_I2C_V2_02A_STANDARD_IC_ENABLE,0,1);

}

void __metal_driver_synopsys_i2c_disable_i2c(struct metal_i2c *i2c)
{
   while(read_register_bit_range(i2c,METAL_SYNOPSYS_I2C_V2_02A_STANDARD_IC_STATUS,0,3) != 0x6 )
   {}

   I2C_REG_RW_BIT(METAL_SYNOPSYS_I2C_V2_02A_STANDARD_IC_ENABLE,0,0);

}

int __metal_driver_synopsys_i2c_set_speed_mode(struct metal_i2c *i2c,struct metal_i2c_config *cfg)
{
   if(cfg->speed_mode == METAL_I2C_FAST_PLUS_SPEED)
   {
      I2C_REG_RW_BIT_RANGE(METAL_SYNOPSYS_I2C_V2_02A_STANDARD_IC_CON,1,2,2);
   }
   else
   {
      I2C_REG_RW_BIT_RANGE(METAL_SYNOPSYS_I2C_V2_02A_STANDARD_IC_CON,1,2,cfg->speed_mode);
   }
}

int __metal_driver_synopsys_i2c_set_address_mode(struct metal_i2c *i2c,struct metal_i2c_config *cfg)
{
   I2C_REG_RW_BIT(METAL_SYNOPSYS_I2C_V2_02A_STANDARD_IC_CON,3,cfg->addressing);
   I2C_REG_RW_BIT(METAL_SYNOPSYS_I2C_V2_02A_STANDARD_IC_CON,4,cfg->addressing);
}

int __metal_driver_synopsys_i2c_set_target_address(struct metal_i2c *i2c,struct metal_i2c_config *cfg,unsigned int address)
{
   write_register(i2c,METAL_SYNOPSYS_I2C_V2_02A_STANDARD_IC_TAR,set_bit(set_bit((REGISTER_DATATYPE)(address & 0x3ff),10,cfg->transmission_mode),11,cfg->enable_special_bit));
   if(cfg->speed_mode == METAL_I2C_HIGH_SPEED)
   {
       write_register(i2c,METAL_SYNOPSYS_I2C_V2_02A_STANDARD_IC_HS_MADDR,cfg->high_speed_master_addr);
   }
}

void __metal_driver_synopsys_i2c_init(struct metal_i2c *i2c,struct metal_i2c_config *cfg)
{

   I2C_REG_RW_BIT(METAL_SYNOPSYS_I2C_V2_02A_STANDARD_IC_ENABLE,0,0);


   if(cfg->operation_mode == METAL_I2C_MASTER)
   {
      I2C_REG_RW_BIT(METAL_SYNOPSYS_I2C_V2_02A_STANDARD_IC_CON,0,1);
      I2C_REG_RW_BIT(METAL_SYNOPSYS_I2C_V2_02A_STANDARD_IC_CON,6,1);
      __metal_driver_synopsys_i2c_set_target_address(i2c,cfg,cfg->target_address);
   }
   else if(cfg->operation_mode == METAL_I2C_SLAVE)
   {
      I2C_REG_RW_BIT(METAL_SYNOPSYS_I2C_V2_02A_STANDARD_IC_CON,0,0);
      I2C_REG_RW_BIT(METAL_SYNOPSYS_I2C_V2_02A_STANDARD_IC_CON,6,0);
      write_register(i2c,METAL_SYNOPSYS_I2C_V2_02A_STANDARD_IC_SAR,cfg->slave_address);
   }

   I2C_REG_RW_BIT(METAL_SYNOPSYS_I2C_V2_02A_STANDARD_IC_CON,5,1);

   write_register(i2c,METAL_SYNOPSYS_I2C_V2_02A_STANDARD_IC_INTR_MASK,0X00);

   write_register(i2c,METAL_SYNOPSYS_I2C_V2_02A_STANDARD_IC_TX_TL,cfg->transmitter_threshold);
   write_register(i2c,METAL_SYNOPSYS_I2C_V2_02A_STANDARD_IC_RX_TL,cfg->receiver_threshold);

   __metal_driver_synopsys_i2c_spike_suppression_configuration(i2c,cfg);

   __metal_driver_synopsys_i2c_clock_config(i2c,cfg);

   __metal_driver_synopsys_i2c_set_address_mode(i2c,cfg);

   __metal_driver_synopsys_i2c_set_speed_mode(i2c,cfg);

}

void __metal_driver_synopsys_i2c_put_data(struct metal_i2c *i2c,uint32_t *tx_buf)
{

   write_register(i2c,METAL_SYNOPSYS_I2C_V2_02A_STANDARD_IC_DATA_CMD,*tx_buf);


}

int __metal_driver_synopsys_i2c_get_data(struct metal_i2c *i2c,struct metal_i2c_config *cfg,int len,REGISTER_DATATYPE *rx_buf)
{

   while(read_register_bit(i2c,METAL_SYNOPSYS_I2C_V2_02A_STANDARD_IC_STATUS,3))
   {
      rx_buf[len] = read_register(i2c,METAL_SYNOPSYS_I2C_V2_02A_STANDARD_IC_DATA_CMD);
      ++len;
   }
   return len;

}

void __metal_driver_synopsys_i2c_load_buffer(struct metal_i2c *i2c,uint32_t *tx_buf,unsigned char data,unsigned char cmd,char *buffer_len)
{
   unsigned char len = *buffer_len;
   tx_buf[len]=set_bit(data,8,cmd);
   *buffer_len = ++len;
}

void __metal_driver_synopsys_i2c_disable_interrupt(struct metal_i2c *i2c,int id)
{
   int interrupt_num = (__metal_driver_synopsys_i2c_interrupt_line(i2c)- METAL_MAX_I2C_INTERRUPTS) + 1;

   if(id == interrupt_num ) { I2C_REG_RW_BIT(METAL_SYNOPSYS_I2C_V2_02A_STANDARD_IC_INTR_MASK,1,0);
   }
   else if(id == (interrupt_num + 1 )) { I2C_REG_RW_BIT(METAL_SYNOPSYS_I2C_V2_02A_STANDARD_IC_INTR_MASK,0,0);
   }
   else if(id == (interrupt_num + 2 )) { I2C_REG_RW_BIT(METAL_SYNOPSYS_I2C_V2_02A_STANDARD_IC_INTR_MASK,3,0);
   }
   else if(id == (interrupt_num + 3 )) { I2C_REG_RW_BIT(METAL_SYNOPSYS_I2C_V2_02A_STANDARD_IC_INTR_MASK,6,0);
   }
   else if(id == (interrupt_num + 4 )) { I2C_REG_RW_BIT(METAL_SYNOPSYS_I2C_V2_02A_STANDARD_IC_INTR_MASK,7,0);
   }
   else if(id == (interrupt_num + 5 )) { I2C_REG_RW_BIT(METAL_SYNOPSYS_I2C_V2_02A_STANDARD_IC_INTR_MASK,4,0);
   }
   else if(id == (interrupt_num + 6 )) { I2C_REG_RW_BIT(METAL_SYNOPSYS_I2C_V2_02A_STANDARD_IC_INTR_MASK,8,0);
   }
   else if(id == (interrupt_num + 7 )) { I2C_REG_RW_BIT(METAL_SYNOPSYS_I2C_V2_02A_STANDARD_IC_INTR_MASK,9,0);
   }
   else if(id == (interrupt_num + 8 )) { I2C_REG_RW_BIT(METAL_SYNOPSYS_I2C_V2_02A_STANDARD_IC_INTR_MASK,10,0);
   }
   else if(id == (interrupt_num + 9 )) { I2C_REG_RW_BIT(METAL_SYNOPSYS_I2C_V2_02A_STANDARD_IC_INTR_MASK,5,0);
   }
   else if(id == (interrupt_num + 10 )) { I2C_REG_RW_BIT(METAL_SYNOPSYS_I2C_V2_02A_STANDARD_IC_INTR_MASK,2,0);
   }
   else if(id == (interrupt_num + 11 )) { I2C_REG_RW_BIT(METAL_SYNOPSYS_I2C_V2_02A_STANDARD_IC_INTR_MASK,11,0);
   }

}

void __metal_driver_synopsys_i2c_enable_interrupt(struct metal_i2c *i2c,int id)
{
   int interrupt_num = (__metal_driver_synopsys_i2c_interrupt_line(i2c)- METAL_MAX_I2C_INTERRUPTS) + 1;


   if (id == interrupt_num ) { I2C_REG_RW_BIT(METAL_SYNOPSYS_I2C_V2_02A_STANDARD_IC_INTR_MASK,1,1);
   }
   else if(id == (interrupt_num + 1 )) { I2C_REG_RW_BIT(METAL_SYNOPSYS_I2C_V2_02A_STANDARD_IC_INTR_MASK,0,1);
   }
   else if(id == (interrupt_num + 2 )) { I2C_REG_RW_BIT(METAL_SYNOPSYS_I2C_V2_02A_STANDARD_IC_INTR_MASK,3,1);
   }
   else if(id == (interrupt_num + 3 )) { I2C_REG_RW_BIT(METAL_SYNOPSYS_I2C_V2_02A_STANDARD_IC_INTR_MASK,6,1);
   }
   else if(id == (interrupt_num + 4 )) { I2C_REG_RW_BIT(METAL_SYNOPSYS_I2C_V2_02A_STANDARD_IC_INTR_MASK,7,1);
   }
   else if(id == (interrupt_num + 5 )) { I2C_REG_RW_BIT(METAL_SYNOPSYS_I2C_V2_02A_STANDARD_IC_INTR_MASK,4,1);
   }
   else if(id == (interrupt_num + 6 )) { I2C_REG_RW_BIT(METAL_SYNOPSYS_I2C_V2_02A_STANDARD_IC_INTR_MASK,8,1);
   }
   else if(id == (interrupt_num + 7 )) { I2C_REG_RW_BIT(METAL_SYNOPSYS_I2C_V2_02A_STANDARD_IC_INTR_MASK,9,1);
   }
   else if(id == (interrupt_num + 8 )) { I2C_REG_RW_BIT(METAL_SYNOPSYS_I2C_V2_02A_STANDARD_IC_INTR_MASK,10,1);
   }
   else if(id == (interrupt_num + 9 )) { I2C_REG_RW_BIT(METAL_SYNOPSYS_I2C_V2_02A_STANDARD_IC_INTR_MASK,5,1);
   }
   else if(id == (interrupt_num + 10 )) { I2C_REG_RW_BIT(METAL_SYNOPSYS_I2C_V2_02A_STANDARD_IC_INTR_MASK,2,1);
   }
   else if(id == (interrupt_num + 11 )) { I2C_REG_RW_BIT(METAL_SYNOPSYS_I2C_V2_02A_STANDARD_IC_INTR_MASK,11,1);
   }

}

void __metal_driver_synopsys_i2c_global_interrupt_clear_handler(struct metal_i2c *i2c,int id)
{
   int interrupt_num = (__metal_driver_synopsys_i2c_interrupt_line(i2c) - METAL_MAX_I2C_INTERRUPTS) + 1;


   if(id == interrupt_num ) {read_register(i2c,METAL_SYNOPSYS_I2C_V2_02A_STANDARD_IC_CLR_RX_OVER); // rx_over_run- interrupt
   }
   else if(id == (interrupt_num + 1 )) { read_register(i2c,METAL_SYNOPSYS_I2C_V2_02A_STANDARD_IC_CLR_RX_UNDER); //rx_under
   }
   else if(id == (interrupt_num + 2 )) { read_register(i2c,METAL_SYNOPSYS_I2C_V2_02A_STANDARD_IC_CLR_TX_OVER); //tx_over
   }
   else if(id == (interrupt_num + 3 )) {
      read_register(i2c,0x80);
      read_register(i2c,METAL_SYNOPSYS_I2C_V2_02A_STANDARD_IC_CLR_TX_ABRT); //tx_abort

   }
   else if(id == (interrupt_num + 4 )) { read_register(i2c,METAL_SYNOPSYS_I2C_V2_02A_STANDARD_IC_CLR_RX_DONE); //rx_done
   }
   else if(id == (interrupt_num + 7 )) { read_register(i2c,METAL_SYNOPSYS_I2C_V2_02A_STANDARD_IC_CLR_STOP_DET); //stop_det
   }
   else if(id == (interrupt_num + 8 )) { read_register(i2c,METAL_SYNOPSYS_I2C_V2_02A_STANDARD_IC_CLR_START_DET); //start_det
   }
   else if(id == (interrupt_num + 9 )) { read_register(i2c,METAL_SYNOPSYS_I2C_V2_02A_STANDARD_IC_CLR_RD_REQ); // ic_general_call
   }
   else if(id == (interrupt_num + 11 )) { read_register(i2c,METAL_SYNOPSYS_I2C_V2_02A_STANDARD_IC_CLR_GEN_CALL); // ic_general_call
   }


}
int __metal_driver_synopsys_i2c_get_interrupt_id(struct metal_i2c *i2c,int num)
{
   if((num>0) && (num <= METAL_MAX_I2C_INTERRUPTS))
   {
      return ((__metal_driver_synopsys_i2c_interrupt_line(i2c)- METAL_MAX_I2C_INTERRUPTS) + num);
   }
   else
   {
      return -1;
   }
}

__METAL_DEFINE_VTABLE(__metal_driver_vtable_synopsys_i2c) = {
   .i2c.enable_i2c               = __metal_driver_synopsys_i2c_enable_i2c,
   .i2c.disable_i2c              = __metal_driver_synopsys_i2c_disable_i2c,
   .i2c.init                     = __metal_driver_synopsys_i2c_init,
   .i2c.put_data                 = __metal_driver_synopsys_i2c_put_data,
   .i2c.get_data                 = __metal_driver_synopsys_i2c_get_data,
   .i2c.set_address_mode         = __metal_driver_synopsys_i2c_set_address_mode,
   .i2c.set_speed_mode           = __metal_driver_synopsys_i2c_set_speed_mode,
   .i2c.set_target_address       = __metal_driver_synopsys_i2c_set_target_address,
   .i2c.get_interrupt_controller = __metal_driver_synopsys_i2c_interrupt_parent,
   .i2c.load_buffer              = __metal_driver_synopsys_i2c_load_buffer,
   .i2c.global_interrupt_clear   = __metal_driver_synopsys_i2c_global_interrupt_clear_handler,
   .i2c.disable_interrupt        = __metal_driver_synopsys_i2c_disable_interrupt,
   .i2c.get_interrupt_id         = __metal_driver_synopsys_i2c_get_interrupt_id,
   .i2c.enable_interrupt         = __metal_driver_synopsys_i2c_enable_interrupt
};



#endif
