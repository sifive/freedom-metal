/* Copyright 2018 SiFive, Inc */
/* SPDX-License-Identifier: Apache-2.0 */

#include <stdint.h>
#include <metal/io.h>
#include <metal/shutdown.h>
#include <metal/drivers/sifive,clic0.h>

typedef enum metal_priv_mode_ {
    METAL_PRIV_M_MODE = 0,
    METAL_PRIV_MU_MODE = 1,
    METAL_PRIV_MSU_MODE = 2
} metal_priv_mode;

typedef enum metal_clic_vector_{
    METAL_CLIC_NONVECTOR = 0,
    METAL_CLIC_VECTORED  = 1
} metal_clic_vector;

struct __metal_clic_cfg {
    unsigned char : 1,
             nmbits : 2,
             nlbits : 4,
              nvbit : 1;
};

const struct __metal_clic_cfg __metal_clic_defaultcfg = {
                .nmbits = METAL_PRIV_M_MODE,
                .nlbits = 0,
                .nvbit = METAL_CLIC_NONVECTOR
            };

struct __metal_clic_cfg __metal_clic0_configuration (struct __metal_driver_sifive_clic0 *clic,
                                                 struct __metal_clic_cfg *cfg)
{
    volatile unsigned char val;
    struct __metal_clic_cfg cliccfg;
    uintptr_t hartid = __metal_myhart_id();

    if ( cfg ) {
        val = cfg->nmbits << 5 | cfg->nlbits << 1 | cfg->nvbit;
        __METAL_ACCESS_ONCE((__metal_io_u8 *)(clic->control_base +
                                       METAL_CLIC_MMODE_OFFSET +
                                       METAL_CLIC_CFG)) = val;
    }
    val = __METAL_ACCESS_ONCE((__metal_io_u8 *)(clic->control_base +
                                       METAL_CLIC_MMODE_OFFSET +
                                       METAL_CLIC_CFG));
    cliccfg.nmbits = (val & METAL_CLIC_CFG_NMBITS_MASK) >> 5;
    cliccfg.nlbits = (val & METAL_CLIC_CFG_NLBITS_MASK) >> 1;
    cliccfg.nvbit = val & METAL_CLIC_CFG_NVBIT_MASK;
    return cliccfg;
}

int __metal_clic0_interrupt_set_mode (struct __metal_driver_sifive_clic0 *clic, int id, int mode)
{
    uint8_t mask, val;
    struct __metal_clic_cfg cfg = __metal_clic0_configuration(clic, NULL);
 
    if (mode >= (cfg.nmbits << 1)) {
        /* Do nothing, mode request same or exceed what configured in CLIC */
        return 0;
    }
 
    /* Mask out nmbits and retain other values */
    mask = ((uint8_t)(-1)) >> cfg.nmbits;
    val = __METAL_ACCESS_ONCE((__metal_io_u8 *)(clic->control_base +
                                       METAL_CLIC_MMODE_OFFSET +
                                       METAL_CLIC_INTR_CTRL + id)) & mask;
    __METAL_ACCESS_ONCE((__metal_io_u8 *)(clic->control_base +
                                 METAL_CLIC_MMODE_OFFSET +
                                 METAL_CLIC_INTR_CTRL + id)) = val | (mode << (8 - cfg.nmbits));
    return 0;
}

int __metal_clic0_interrupt_set_level (struct __metal_driver_sifive_clic0 *clic, int id, int level)
{
    uint8_t mask, nmmask, nlmask, val;
    struct __metal_clic_cfg cfg = __metal_clic0_configuration(clic, NULL);

    /* Drop the LSBs that don't fit in nlbits */
    level = level >> (METAL_CLIC_MAX_NLBITS - cfg.nlbits);

    nmmask = ~( ((uint8_t)(-1)) >> (cfg.nmbits) );
    nlmask = ((uint8_t)(-1)) >> (cfg.nmbits + cfg.nlbits);
    mask = ~(nlmask | nmmask);
  
    val = __METAL_ACCESS_ONCE((__metal_io_u8 *)(clic->control_base +
                                       METAL_CLIC_MMODE_OFFSET +
                                       METAL_CLIC_INTR_CTRL + id));
    __METAL_ACCESS_ONCE((__metal_io_u8 *)(clic->control_base +
                                 METAL_CLIC_MMODE_OFFSET +
                                 METAL_CLIC_INTR_CTRL + id)) = __METAL_SET_FIELD(val, mask, level);
    return 0;
}

int __metal_clic0_interrupt_get_level (struct __metal_driver_sifive_clic0 *clic, int id)
{
    int level;
    uint8_t mask, val, freebits, nlbits;
    struct __metal_clic_cfg cfg = __metal_clic0_configuration(clic, NULL);

    if ((cfg.nmbits + cfg.nlbits) >= clic->num_intbits) {
        nlbits = clic->num_intbits - cfg.nmbits;
    } else {
        nlbits = cfg.nlbits;
    }

    mask = ((1 << nlbits) - 1) << (8 - (cfg.nmbits + nlbits));
    freebits = ((1 << METAL_CLIC_MAX_NLBITS) - 1) >> nlbits;
  
    if (mask == 0) {
        level = (1 << METAL_CLIC_MAX_NLBITS) - 1;
    } else {
        val = __METAL_ACCESS_ONCE((__metal_io_u8 *)(clic->control_base +
                                       METAL_CLIC_MMODE_OFFSET +
                                       METAL_CLIC_INTR_CTRL + id));
        val = __METAL_GET_FIELD(val, mask);
        level = (val << (METAL_CLIC_MAX_NLBITS - nlbits)) | freebits;
    }

    return level;
}

int __metal_clic0_interrupt_set_priority (struct __metal_driver_sifive_clic0 *clic, int id, int priority)
{
    uint8_t mask, npmask, val, npbits;
    struct __metal_clic_cfg cfg = __metal_clic0_configuration(clic, NULL);

    if ((cfg.nmbits + cfg.nlbits) < clic->num_intbits) {
        npbits = clic->num_intbits - (cfg.nmbits + cfg.nlbits);
        priority = priority >> (8 - npbits);

        mask = ((uint8_t)(-1)) >> (cfg.nmbits + cfg.nlbits + npbits);
        npmask = ~(((uint8_t)(-1)) >> (cfg.nmbits + cfg.nlbits));
        mask = ~(mask | npmask);

        val = __METAL_ACCESS_ONCE((__metal_io_u8 *)(clic->control_base +
                                       METAL_CLIC_MMODE_OFFSET +
                                       METAL_CLIC_INTR_CTRL + id));
        __METAL_ACCESS_ONCE((__metal_io_u8 *)(clic->control_base +
                                     METAL_CLIC_MMODE_OFFSET +
                                     METAL_CLIC_INTR_CTRL + id)) = __METAL_SET_FIELD(val, mask, priority);
    }
    return 0;
}

int __metal_clic0_interrupt_get_priority (struct __metal_driver_sifive_clic0 *clic, int id)
{
    int priority;
    uint8_t mask, val, freebits, nlbits;
    struct __metal_clic_cfg cfg = __metal_clic0_configuration(clic, NULL);

    if ((cfg.nmbits + cfg.nlbits) >= clic->num_intbits) {
        nlbits = clic->num_intbits - cfg.nmbits;
    } else {
        nlbits = cfg.nlbits;
    }

    mask = ((1 << nlbits) - 1) << (8 - (cfg.nmbits + nlbits));
    freebits = ((1 << METAL_CLIC_MAX_NLBITS) - 1) >> nlbits;
    
    if (mask == 0) {
        priority = (1 << METAL_CLIC_MAX_NLBITS) - 1;
    } else {                           
        val = __METAL_ACCESS_ONCE((__metal_io_u8 *)(clic->control_base +
                                       METAL_CLIC_MMODE_OFFSET +
                                       METAL_CLIC_INTR_CTRL + id));
        priority = __METAL_GET_FIELD(val, freebits);
    }
    return priority;
}

int __metal_clic0_interrupt_set_vector (struct __metal_driver_sifive_clic0 *clic, int id, int enable)
{   
    uint8_t mask, val;

    mask = 1 << (8 - clic->num_intbits);
    val = __METAL_ACCESS_ONCE((__metal_io_u8 *)(clic->control_base +
                                       METAL_CLIC_MMODE_OFFSET +
                                       METAL_CLIC_INTR_CTRL + id));
    /* Ensure its value is 1 bit wide */
    enable &= 0x1;
    __METAL_ACCESS_ONCE((__metal_io_u8 *)(clic->control_base +
                                 METAL_CLIC_MMODE_OFFSET +
                                 METAL_CLIC_INTR_CTRL + id)) = __METAL_SET_FIELD(val, mask, enable);
    return 0;
}

int __metal_clic0_interrupt_is_vectored (struct __metal_driver_sifive_clic0 *clic, int id)
{
    uint8_t mask, val;

    mask = 1 << (8 - clic->num_intbits);
    val = __METAL_ACCESS_ONCE((__metal_io_u8 *)(clic->control_base +
                                       METAL_CLIC_MMODE_OFFSET +
                                       METAL_CLIC_INTR_CTRL + id));
    return __METAL_GET_FIELD(val, mask);
}

int __metal_clic0_interrupt_enable (struct __metal_driver_sifive_clic0 *clic, int id)
{
    if (id >= clic->num_subinterrupts) {
        return -1;
    }
    __METAL_ACCESS_ONCE((__metal_io_u8 *)(clic->control_base +
                                       METAL_CLIC_MMODE_OFFSET +
                                       METAL_CLIC_INTR_IE + id)) = METAL_ENABLE;
    return 0;
}

int __metal_clic0_interrupt_disable (struct __metal_driver_sifive_clic0 *clic, int id)
{
    if (id >= clic->num_subinterrupts) {
        return -1;
    }
    __METAL_ACCESS_ONCE((__metal_io_u8 *)(clic->control_base +
                                       METAL_CLIC_MMODE_OFFSET +
                                       METAL_CLIC_INTR_IE + id)) = METAL_DISABLE;
    return 0;
}

int __metal_clic0_interrupt_is_enabled (struct __metal_driver_sifive_clic0 *clic, int id)
{
    if (id >= clic->num_subinterrupts) {
        return 0;
    }
    return __METAL_ACCESS_ONCE((__metal_io_u8 *)(clic->control_base +
                                       METAL_CLIC_MMODE_OFFSET +
                                       METAL_CLIC_INTR_IE + id));
}

int __metal_clic0_interrupt_is_pending (struct __metal_driver_sifive_clic0 *clic, int id)
{
    if (id >= clic->num_subinterrupts) {
        return 0;
    }
    return __METAL_ACCESS_ONCE((__metal_io_u8 *)(clic->control_base +
                                       METAL_CLIC_MMODE_OFFSET +
                                       METAL_CLIC_INTR_IP + id));
}

int __metal_clic0_interrupt_set (struct __metal_driver_sifive_clic0 *clic, int id)
{       
    if ((id >= METAL_INTERRUPT_ID_LC0) && (id < clic->num_subinterrupts)) {
    }
    return 0;
}

int __metal_clic0_interrupt_clear (struct __metal_driver_sifive_clic0 *clic, int id)
{
    if ((id >= METAL_INTERRUPT_ID_LC0) && (id < clic->num_subinterrupts)) {
    }
    return 0;
}

void __metal_clic0_configure_privilege (struct __metal_driver_sifive_clic0 *clic, metal_priv_mode priv)
{
    struct __metal_clic_cfg cfg = __metal_clic0_configuration(clic, NULL);

    cfg.nmbits = priv;
    __metal_clic0_configuration(clic, &cfg);
}

void __metal_clic0_configure_level (struct __metal_driver_sifive_clic0 *clic, int level)
{
    struct __metal_clic_cfg cfg = __metal_clic0_configuration(clic, NULL);

    cfg.nlbits = level;
    __metal_clic0_configuration(clic, &cfg);
}

unsigned long long __metal_clic0_mtime_get (struct __metal_driver_sifive_clic0 *clic)
{
    __metal_io_u32 lo, hi;

    /* Guard against rollover when reading */
    do {
	hi = __METAL_ACCESS_ONCE((__metal_io_u32 *)(clic->control_base + METAL_CLIC_MTIME_OFFSET + 4));
	lo = __METAL_ACCESS_ONCE((__metal_io_u32 *)(clic->control_base + METAL_CLIC_MTIME_OFFSET));
    } while (__METAL_ACCESS_ONCE((__metal_io_u32 *)(clic->control_base + METAL_CLIC_MTIME_OFFSET + 4)) != hi);

    return (((unsigned long long)hi) << 32) | lo;
}

int __metal_clic0_mtime_set (struct __metal_driver_sifive_clic0 *clic, unsigned long long time)
{   
    /* Per spec, the RISC-V MTIME/MTIMECMP registers are 64 bit,
     * and are NOT internally latched for multiword transfers.
     * Need to be careful about sequencing to avoid triggering
     * spurious interrupts: For that set the high word to a max
     * value first.
     */
    __METAL_ACCESS_ONCE((__metal_io_u32 *)(clic->control_base + METAL_CLIC_MTIMECMP_OFFSET + 4)) = 0xFFFFFFFF;
    __METAL_ACCESS_ONCE((__metal_io_u32 *)(clic->control_base + METAL_CLIC_MTIMECMP_OFFSET)) = (__metal_io_u32)time;
    __METAL_ACCESS_ONCE((__metal_io_u32 *)(clic->control_base + METAL_CLIC_MTIMECMP_OFFSET + 4)) = (__metal_io_u32)(time >> 32);
    return 0;
}

void __metal_clic0_handler(int id, void *priv) __attribute__((aligned(64)));
void __metal_clic0_handler (int id, void *priv)
{
    int idx;
    struct __metal_driver_sifive_clic0 *clic = priv;

    idx = id - METAL_INTERRUPT_ID_LC0;
    if ( (idx < clic->num_subinterrupts) && (clic->metal_mtvt_table[idx]) ) {
        clic->metal_mtvt_table[idx](id, clic->metal_exint_table[idx].exint_data);
    }
}

void __metal_clic0_default_handler (int id, void *priv) {
    metal_shutdown(300);
}

void __metal_driver_sifive_clic0_init (struct metal_interrupt *controller)
{
    struct __metal_driver_sifive_clic0 *clic =
                              (struct __metal_driver_sifive_clic0 *)(controller);

    if ( !clic->init_done ) {
        int level;
        struct __metal_clic_cfg cfg = __metal_clic_defaultcfg;
        struct metal_interrupt *intc = clic->interrupt_parent;

        /* Initialize ist parent controller, aka cpu_intc. */
        intc->vtable->interrupt_init(intc);
        __metal_controller_interrupt_vector(METAL_SELECTIVE_VECTOR_MODE,
                                          &__metal_clic0_handler);

        /*
         * Register its interrupts with with parent controller,
         * aka sw, timer and ext to its default isr
         */
        for (int i = 0; i < clic->num_interrupts; i++) {
            intc->vtable->interrupt_register(intc,
                                             clic->interrupt_lines[i],
                                             NULL, clic);
        }

        /* Default CLIC mode to per dts */
        cfg.nlbits = (clic->max_levels > METAL_CLIC_MAX_NLBITS) ?
                                METAL_CLIC_MAX_NLBITS : clic->max_levels;
        __metal_clic0_configuration(clic, &cfg);

        level = (1 << cfg.nlbits) - 1;
        for (int i = 0; i < clic->num_subinterrupts; i++) {
            clic->metal_mtvt_table[i] = NULL;
            clic->metal_exint_table[i].sub_int = NULL;
            clic->metal_exint_table[i].exint_data = NULL;
            __metal_clic0_interrupt_disable(clic, i);
            __metal_clic0_interrupt_set_level(clic, i, level);
        }
	clic->init_done = 1;
    }	
}

int __metal_driver_sifive_clic0_register (struct metal_interrupt *controller,
                                        int id, metal_interrupt_handler_t isr,
                                        void *priv)
{
    int rc = -1;
    struct __metal_driver_sifive_clic0 *clic =
                              (struct __metal_driver_sifive_clic0 *)(controller);
    struct metal_interrupt *intc = clic->interrupt_parent;

    /* Register its interrupts with parent controller */
    if ( id < METAL_INTERRUPT_ID_LC0) {
        return intc->vtable->interrupt_register(intc, id, isr, priv);
    }

    /*
     * CLIC (sub-interrupts) devices interrupts start at 16 but offset from 0
     * Reset the IDs to reflects this. 
     */
    id -= METAL_INTERRUPT_ID_LC0;
    if (id < clic->num_subinterrupts) {
        if ( isr) {
            clic->metal_mtvt_table[id] = isr;
            clic->metal_exint_table[id].exint_data = priv;        
        } else {
            clic->metal_mtvt_table[id] = __metal_clic0_default_handler;
            clic->metal_exint_table[id].sub_int = priv;
        }
        rc = 0;
    }
    return rc;
}

int __metal_driver_sifive_clic0_enable (struct metal_interrupt *controller, int id)
{
    struct __metal_driver_sifive_clic0 *clic =
                              (struct __metal_driver_sifive_clic0 *)(controller);
    return __metal_clic0_interrupt_enable(clic, id);
}

int __metal_driver_sifive_clic0_disable (struct metal_interrupt *controller, int id)
{
    struct __metal_driver_sifive_clic0 *clic =
                              (struct __metal_driver_sifive_clic0 *)(controller);
    return __metal_clic0_interrupt_disable(clic, id);
}

int __metal_driver_sifive_clic0_enable_interrupt_vector(struct metal_interrupt *controller,
                                                      int id, metal_vector_mode mode)
{
    struct __metal_driver_sifive_clic0 *clic =
                              (struct __metal_driver_sifive_clic0 *)(controller);

    if (id == METAL_INTERRUPT_ID_BASE) {
        if (mode == METAL_SELECTIVE_VECTOR_MODE) {
            __metal_controller_interrupt_vector(mode, &__metal_clic0_handler);
            return 0;
        }
        if (mode == METAL_HARDWARE_VECTOR_MODE) {
            __metal_controller_interrupt_vector(mode, &clic->metal_mtvt_table);
            return 0;
        }
    }
    if ((id >= METAL_INTERRUPT_ID_LC0) && (id < clic->num_subinterrupts)) {
        if ((mode == METAL_SELECTIVE_VECTOR_MODE) &&
             __metal_controller_interrupt_is_selective_vectored()) {
            __metal_clic0_interrupt_set_vector(clic, id, METAL_ENABLE);
            return 0;
        }

    }
    return -1;
}

int __metal_driver_sifive_clic0_disable_interrupt_vector(struct metal_interrupt *controller, int id)
{
    struct __metal_driver_sifive_clic0 *clic =
                              (struct __metal_driver_sifive_clic0 *)(controller);

    if (id == METAL_INTERRUPT_ID_BASE) {
        __metal_controller_interrupt_vector(METAL_SELECTIVE_VECTOR_MODE, &__metal_clic0_handler);
        return 0;
    }
    if ((id >= METAL_INTERRUPT_ID_LC0) && (id < clic->num_subinterrupts)) {
        if  (__metal_controller_interrupt_is_selective_vectored()) {
            __metal_clic0_interrupt_set_vector(clic, id, METAL_DISABLE);
            return 0;
        }
    }
    return -1;
}

int __metal_driver_sifive_clic0_command_request (struct metal_interrupt *controller,
                                               int command, void *data)
{
    int hartid;
    int rc = -1;
    struct __metal_driver_sifive_clic0 *clic =
                              (struct __metal_driver_sifive_clic0 *)(controller);

    switch (command) {
    case METAL_TIMER_MTIME_GET:
        if (data) {
	    *(unsigned long long *)data = __metal_clic0_mtime_get(clic);
            rc = 0;
        }
        break;
    case METAL_TIMER_MTIME_SET:
        if (data) {
	    __metal_clic0_mtime_set(clic, *(unsigned long long *)data);
            rc = 0;
        }
        break;
    case METAL_SOFTWARE_IPI_CLEAR:
	if (data) {
	    hartid = *(int *)data;
            __METAL_ACCESS_ONCE((__metal_io_u32 *)(clic->control_base +
					       (hartid * 4))) = METAL_DISABLE;
           __METAL_ACCESS_ONCE((__metal_io_u8 *)(clic->control_base +
                                              METAL_CLIC_MMODE_OFFSET +
                                              METAL_CLIC_INTR_IP)) = METAL_DISABLE;
            rc = 0;
        }
        break;
    case METAL_SOFTWARE_IPI_SET:
	if (data) {
	    hartid = *(int *)data;
            __METAL_ACCESS_ONCE((__metal_io_u32 *)(clic->control_base +
					       (hartid * 4))) = METAL_ENABLE;
            __METAL_ACCESS_ONCE((__metal_io_u8 *)(clic->control_base +
                                               METAL_CLIC_MMODE_OFFSET +
                                               METAL_CLIC_INTR_IP)) = METAL_ENABLE;
            rc = 0;
        }
        break;
    case METAL_SOFTWARE_MSIP_GET:
        rc = 0;
	if (data) {
	    hartid = *(int *)data;
            rc = __METAL_ACCESS_ONCE((__metal_io_u32 *)(clic->control_base +
						    (hartid * 4)));
        }
        break;
    default:
	break;
    }

    return rc;
}
