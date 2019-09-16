#include<metal/machine.h>
#include<metal/i2c.h>

extern __inline__ void metal_i2c_enable(struct metal_i2c *i2c);
extern __inline__ void metal_i2c_disable(struct metal_i2c *i2c);
extern __inline__ void metal_i2c_init(struct metal_i2c *i2c,struct metal_i2c_config *cfg);
extern __inline__ int metal_i2c_write_transfer(struct metal_i2c *i2c,unsigned char *tx_buf);
extern __inline__ int metal_i2c_read_transfer(struct metal_i2c *i2c,struct metal_i2c_config *cfg,int len,unsigned char *rx_buf);
extern __inline__ int metal_i2c_set_address_mode(struct metal_i2c *i2c,struct metal_i2c_config *cfg);
extern __inline__ int metal_i2c_set_speed_mode(struct metal_i2c *i2c,struct metal_i2c_config *cfg);
extern __inline__ int metal_i2c_set_target_address(struct metal_i2c *i2c,struct metal_i2c_config *cfg,unsigned int address);
extern __inline__ struct metal_interrupt* metal_i2c_get_interrupt_controller(struct metal_i2c *i2c);
//extern __inline__ int metal_i2c_get_interrupt_id(struct metal_i2c *i2c);

struct metal_i2c *metal_i2c_get_device(unsigned int device_num)
{
#if __METAL_DT_MAX_I2CS > 0
    if (device_num < __METAL_DT_MAX_I2CS) {
        return (struct metal_i2c *) __metal_i2c_table[device_num];
    }
#endif

    return NULL;
}