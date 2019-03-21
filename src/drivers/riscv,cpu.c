/* Copyright 2018 SiFive, Inc */
/* SPDX-License-Identifier: Apache-2.0 */

#include <stdint.h>
#include <metal/io.h>
#include <metal/shutdown.h>
#include <metal/machine.h>

struct metal_cpu *__metal_driver_cpu_get(int hartid)
{
    if (hartid < __METAL_DT_MAX_HARTS) {
        return &(__metal_cpu_table[hartid]->cpu);
    }
    return (struct metal_cpu *)NULL;
}

uintptr_t __metal_myhart_id (void)
{
    uintptr_t myhart;
    asm volatile ("csrr %0, mhartid" : "=r"(myhart));
    return myhart;
}

void __metal_zero_memory (unsigned char *base, unsigned int size)
{
    volatile unsigned char *ptr;
    for (ptr = base; ptr < (base + size); ptr++){
        *ptr = 0;
    }
}

void __metal_interrupt_global_enable (void) {
    uintptr_t m;
    asm volatile ("csrrs %0, mstatus, %1" : "=r"(m) : "r"(METAL_MIE_INTERRUPT));
}

void __metal_interrupt_global_disable (void) {
    uintptr_t m;
    asm volatile ("csrrc %0, mstatus, %1" : "=r"(m) : "r"(METAL_MIE_INTERRUPT));
}

void __metal_interrupt_software_enable (void) {
    uintptr_t m;
    asm volatile ("csrrs %0, mie, %1" : "=r"(m) : "r"(METAL_LOCAL_INTERRUPT_SW));
}

void __metal_interrupt_software_disable (void) {
    uintptr_t m;
    asm volatile ("csrrc %0, mie, %1" : "=r"(m) : "r"(METAL_LOCAL_INTERRUPT_SW));
}

void __metal_interrupt_timer_enable (void) {
    uintptr_t m;
    asm volatile ("csrrs %0, mie, %1" : "=r"(m) : "r"(METAL_LOCAL_INTERRUPT_TMR));
}

void __metal_interrupt_timer_disable (void) {
    uintptr_t m;
    asm volatile ("csrrc %0, mie, %1" : "=r"(m) : "r"(METAL_LOCAL_INTERRUPT_TMR));
}

void __metal_interrupt_external_enable (void) {
    uintptr_t m;
    asm volatile ("csrrs %0, mie, %1" : "=r"(m) : "r"(METAL_LOCAL_INTERRUPT_EXT));
}

void __metal_interrupt_external_disable (void) {
    unsigned long m;
    asm volatile ("csrrc %0, mie, %1" : "=r"(m) : "r"(METAL_LOCAL_INTERRUPT_EXT));
}

void __metal_interrupt_local_enable (int id) {
    uintptr_t b = 1 << id;
    uintptr_t m;
    asm volatile ("csrrs %0, mie, %1" : "=r"(m) : "r"(b));
}

void __metal_interrupt_local_disable (int id) {
    uintptr_t b = 1 << id;
    uintptr_t m;
    asm volatile ("csrrc %0, mie, %1" : "=r"(m) : "r"(b));
}

void __metal_default_exception_handler (struct metal_cpu *cpu, int ecode) {
    metal_shutdown(100);
}

void __metal_default_interrupt_handler (int id, void *priv) {
    metal_shutdown(200);
}

void __metal_default_sw_handler (int id, void *priv) {
    uintptr_t mcause;
    struct __metal_driver_riscv_cpu_intc *intc;
    struct __metal_driver_cpu *cpu = __metal_cpu_table[__metal_myhart_id()];

    asm volatile ("csrr %0, mcause" : "=r"(mcause));
    if ( cpu ) {
        intc = (struct __metal_driver_riscv_cpu_intc *)cpu->interrupt_controller;
        intc->metal_exception_table[mcause & METAL_MCAUSE_CAUSE]((struct metal_cpu *)cpu, id);
    }
}

void __metal_default_timer_handler (int id, void *priv) {
    struct metal_cpu *cpu = __metal_driver_cpu_get(__metal_myhart_id());
    unsigned long long time = __metal_driver_cpu_mtime_get(cpu);

    /* Set a 10 cycle timer */
    __metal_driver_cpu_mtimecmp_set(cpu, time + 10);
}

void __metal_exception_handler(void) __attribute__((interrupt, aligned(128)));
void __metal_exception_handler (void) {
    int id;
    void *priv;
    uintptr_t mcause, mepc, mtval, mtvec;
    struct __metal_driver_riscv_cpu_intc *intc;
    struct __metal_driver_cpu *cpu = __metal_cpu_table[__metal_myhart_id()];

    asm volatile ("csrr %0, mcause" : "=r"(mcause));
    asm volatile ("csrr %0, mepc" : "=r"(mepc));
    asm volatile ("csrr %0, mtval" : "=r"(mtval));
    asm volatile ("csrr %0, mtvec" : "=r"(mtvec));

    if ( cpu ) {
        intc = (struct __metal_driver_riscv_cpu_intc *)cpu->interrupt_controller;
        id = mcause & METAL_MCAUSE_CAUSE;
        if (mcause & METAL_MCAUSE_INTR) {
            if ((id < METAL_INTERRUPT_ID_LC0) ||
               ((mtvec & METAL_MTVEC_MASK) == METAL_MTVEC_DIRECT)) {
                priv = intc->metal_int_table[id].exint_data;
                intc->metal_int_table[id].handler(id, priv);
		return;
            }
            if ((mtvec & METAL_MTVEC_MASK) == METAL_MTVEC_CLIC) {
    		uintptr_t mtvt;
    		metal_interrupt_handler_t mtvt_handler;

                asm volatile ("csrr %0, mtvt" : "=r"(mtvt));
               	priv = intc->metal_int_table[METAL_INTERRUPT_ID_SW].sub_int;
               	mtvt_handler = (metal_interrupt_handler_t)mtvt;
               	mtvt_handler(id, priv);
		return;
            }
        } else {
            intc->metal_exception_table[id]((struct metal_cpu *)cpu, id);
        }
    }
}

void __metal_controller_interrupt_vector (metal_vector_mode mode, void *vec_table)
{  
    uintptr_t trap_entry, val;

    asm volatile ("csrr %0, mtvec" : "=r"(val));
    val &= ~(METAL_MTVEC_CLIC_VECTORED | METAL_MTVEC_CLIC_RESERVED);
    trap_entry = (uintptr_t)vec_table;

    switch (mode) {
    case METAL_SELECTIVE_VECTOR_MODE:
        asm volatile ("csrw mtvt, %0" :: "r"(trap_entry | METAL_MTVEC_CLIC));
        asm volatile ("csrw mtvec, %0" :: "r"(val | METAL_MTVEC_CLIC));
        break;
    case METAL_HARDWARE_VECTOR_MODE:
        asm volatile ("csrw mtvt, %0" :: "r"(trap_entry | METAL_MTVEC_CLIC_VECTORED));
        asm volatile ("csrw mtvec, %0" :: "r"(val | METAL_MTVEC_CLIC_VECTORED));
        break;
    case METAL_VECTOR_MODE:
        asm volatile ("csrw mtvec, %0" :: "r"(trap_entry | METAL_MTVEC_VECTORED));
        break;
    case METAL_DIRECT_MODE:
        asm volatile ("csrw mtvec, %0" :: "r"(trap_entry & ~METAL_MTVEC_CLIC_VECTORED));
        break;
    }
}

int __metal_valid_interrupt_id (int id)
{
    switch (id) {
    case METAL_INTERRUPT_ID_SW:
    case METAL_INTERRUPT_ID_TMR:
    case METAL_INTERRUPT_ID_EXT:
    case METAL_INTERRUPT_ID_LC0:
    case METAL_INTERRUPT_ID_LC1:
    case METAL_INTERRUPT_ID_LC2:
    case METAL_INTERRUPT_ID_LC3:
    case METAL_INTERRUPT_ID_LC4:
    case METAL_INTERRUPT_ID_LC5:
    case METAL_INTERRUPT_ID_LC6:
    case METAL_INTERRUPT_ID_LC7:
    case METAL_INTERRUPT_ID_LC8:
    case METAL_INTERRUPT_ID_LC9:
    case METAL_INTERRUPT_ID_LC10:
    case METAL_INTERRUPT_ID_LC11:
    case METAL_INTERRUPT_ID_LC12:
    case METAL_INTERRUPT_ID_LC13:
    case METAL_INTERRUPT_ID_LC14:
    case METAL_INTERRUPT_ID_LC15:
        return 1;
    default:
        break;
    }

    return 0;
}

 
int __metal_local_interrupt_enable (struct metal_interrupt *controller,
				  metal_interrupt_id_e id, int enable)
{
    int rc = 0;
  
    if ( !controller) {
        return -1;
    }

    switch (id) {
    case METAL_INTERRUPT_ID_BASE:
        if (enable) {
            __metal_interrupt_global_enable();
        } else {
            __metal_interrupt_global_disable();
        }
        break;
    case METAL_INTERRUPT_ID_SW:
        if (enable) {
            __metal_interrupt_software_enable();
        } else {
            __metal_interrupt_software_disable();
        }
        break;
    case METAL_INTERRUPT_ID_TMR:
        if (enable) {
	    __metal_interrupt_timer_enable();
	} else {
	    __metal_interrupt_timer_disable();
	}
	break;
    case METAL_INTERRUPT_ID_EXT:
        if (enable) {
            __metal_interrupt_external_enable();
        } else {
            __metal_interrupt_external_disable();
        }
        break;
    case METAL_INTERRUPT_ID_LC0:
    case METAL_INTERRUPT_ID_LC1:
    case METAL_INTERRUPT_ID_LC2:
    case METAL_INTERRUPT_ID_LC3:
    case METAL_INTERRUPT_ID_LC4:
    case METAL_INTERRUPT_ID_LC5:
    case METAL_INTERRUPT_ID_LC6:
    case METAL_INTERRUPT_ID_LC7:
    case METAL_INTERRUPT_ID_LC8:
    case METAL_INTERRUPT_ID_LC9:
    case METAL_INTERRUPT_ID_LC10:
    case METAL_INTERRUPT_ID_LC11:
    case METAL_INTERRUPT_ID_LC12:
    case METAL_INTERRUPT_ID_LC13:
    case METAL_INTERRUPT_ID_LC14:
    case METAL_INTERRUPT_ID_LC15:
        if (enable) {
            __metal_interrupt_local_enable(id);
        } else {
            __metal_interrupt_local_disable(id);
        }
        break;
    defaut:
        rc = -1;
    }
    return rc;
}

int __metal_exception_register (struct metal_interrupt *controller,
                              int ecode, metal_exception_handler_t isr)
{
    struct __metal_driver_riscv_cpu_intc *intc = (void *)(controller);

    if ((ecode < METAL_MAX_EXCEPTION_CODE) && isr) {
        intc->metal_exception_table[ecode] = isr;
        return 0;
    }
    return -1;
}

void __metal_driver_riscv_cpu_controller_interrupt_init (struct metal_interrupt *controller)
{
    struct __metal_driver_riscv_cpu_intc *intc = (void *)(controller);
    uintptr_t val;

    if ( !intc->init_done ) {
        /* Default to use direct interrupt, setup sw cb table*/
        for (int i = 0; i < METAL_MAX_MI; i++) {
            intc->metal_int_table[i].handler = NULL;
            intc->metal_int_table[i].sub_int = NULL;
            intc->metal_int_table[i].exint_data = NULL;
	}
	for (int i = 0; i < METAL_MAX_ME; i++) {
	    intc->metal_exception_table[i] = __metal_default_exception_handler;
	}
        __metal_controller_interrupt_vector(METAL_DIRECT_MODE, &__metal_exception_handler);
	asm volatile ("csrr %0, misa" : "=r"(val));
	if (val & (METAL_ISA_D_EXTENSIONS | METAL_ISA_F_EXTENSIONS | METAL_ISA_Q_EXTENSIONS)) {
	    /* Floating point architecture, so turn on FP register saving*/
	    asm volatile ("csrr %0, mstatus" : "=r"(val));
	    asm volatile ("csrw mstatus, %0" :: "r"(val | METAL_MSTATUS_FS_INIT));
	}
	intc->init_done = 1;
    }
}

int __metal_driver_riscv_cpu_controller_interrupt_register(struct metal_interrupt *controller,
							 int id, metal_interrupt_handler_t isr,
							 void *priv)
{
    int rc = 0;
    struct __metal_driver_riscv_cpu_intc *intc = (void *)(controller);
 
    if ( !__metal_valid_interrupt_id(id) ) {
        return -11;
    }

    if (isr) {
        intc->metal_int_table[id].handler = isr;
        intc->metal_int_table[id].exint_data = priv;
    } else {
	switch (id) {
	case METAL_INTERRUPT_ID_SW:
            intc->metal_int_table[id].handler = __metal_default_sw_handler;
            intc->metal_int_table[id].sub_int = priv;
	  break;
	case METAL_INTERRUPT_ID_TMR:
            intc->metal_int_table[id].handler = __metal_default_timer_handler;
            intc->metal_int_table[id].sub_int = priv;
	  break;
	case METAL_INTERRUPT_ID_EXT:
	case METAL_INTERRUPT_ID_LC0:
	case METAL_INTERRUPT_ID_LC1:
	case METAL_INTERRUPT_ID_LC2:
	case METAL_INTERRUPT_ID_LC3:
	case METAL_INTERRUPT_ID_LC4:
	case METAL_INTERRUPT_ID_LC5:
	case METAL_INTERRUPT_ID_LC6:
	case METAL_INTERRUPT_ID_LC7:
	case METAL_INTERRUPT_ID_LC8:
	case METAL_INTERRUPT_ID_LC9:
	case METAL_INTERRUPT_ID_LC10:
	case METAL_INTERRUPT_ID_LC11:
	case METAL_INTERRUPT_ID_LC12:
	case METAL_INTERRUPT_ID_LC13:
	case METAL_INTERRUPT_ID_LC14:
	case METAL_INTERRUPT_ID_LC15:
            intc->metal_int_table[id].handler = __metal_default_interrupt_handler;
            intc->metal_int_table[id].sub_int = priv;
	  break;
	defaut:
	  rc = -12;
	}
    }
    return rc;
}

int __metal_driver_riscv_cpu_controller_interrupt_enable (struct metal_interrupt *controller,
                                                        int id)
{
    return __metal_local_interrupt_enable(controller, id, METAL_ENABLE);
}

int __metal_driver_riscv_cpu_controller_interrupt_disable (struct metal_interrupt *controller,
                                                         int id)
{   
    return __metal_local_interrupt_enable(controller, id, METAL_DISABLE);
}

int __metal_driver_riscv_cpu_controller_interrupt_enable_vector(struct metal_interrupt *controller,
                                                             int id, metal_vector_mode mode)
{
    struct __metal_driver_riscv_cpu_intc *intc = (void *)(controller);

    if (id == METAL_INTERRUPT_ID_BASE) {
        if (mode == METAL_DIRECT_MODE) {
            __metal_controller_interrupt_vector(mode, &__metal_exception_handler);
            return 0;
        }   
        if (mode == METAL_VECTOR_MODE) {
            __metal_controller_interrupt_vector(mode, &intc->metal_mtvec_table);
            return 0;
        }
    }
    return -1;
}

int __metal_driver_riscv_cpu_controller_interrupt_disable_vector(struct metal_interrupt *controller,
                                                              int id)
{
    struct __metal_driver_riscv_cpu_intc *intc = (void *)(controller);

    if (id == METAL_INTERRUPT_ID_BASE) {
        __metal_controller_interrupt_vector(METAL_DIRECT_MODE, &__metal_exception_handler);
        return 0;
    }
    return -1;
}

int __metal_driver_riscv_cpu_controller_command_request (struct metal_interrupt *controller,
						       int cmd, void *data)
{
    /* NOP for now, unless local interrupt lines the like of clic, clint, plic */
    return 0;
}

extern inline int __metal_controller_interrupt_is_selective_vectored(void);

/* CPU driver !!! */

unsigned long long __metal_driver_cpu_timer_get(struct metal_cpu *cpu)
{
    unsigned long long val = 0;

#if __riscv_xlen == 32
    unsigned long hi, hi1, lo;

    asm volatile ("csrr %0, mcycleh" : "=r"(hi));
    asm volatile ("csrr %0, mcycle" : "=r"(lo));
    asm volatile ("csrr %0, mcycleh" : "=r"(hi1));
    if (hi == hi1) {
        val = ((unsigned long long)hi << 32) | lo;
    }
#else
    asm volatile ("csrr %0, mcycle" : "=r"(val));
#endif

    return val;
}

unsigned long long __metal_driver_cpu_timebase_get(struct metal_cpu *cpu)
{
    struct __metal_driver_cpu *_cpu = (void *)(cpu);
    if (!_cpu) {
        return 0;
    }

    return _cpu->timebase;
}

unsigned long long  __metal_driver_cpu_mtime_get (struct metal_cpu *cpu)
{
    unsigned long long time = 0;
    struct metal_interrupt *tmr_intc;
    struct __metal_driver_riscv_cpu_intc *intc;
    struct __metal_driver_cpu *_cpu = (void *)cpu;

    if (_cpu->interrupt_controller) {
        intc = (void *)_cpu->interrupt_controller;
        tmr_intc = intc->metal_int_table[METAL_INTERRUPT_ID_TMR].sub_int;
        if (tmr_intc) {
            tmr_intc->vtable->command_request(tmr_intc,
                                              METAL_TIMER_MTIME_GET, &time);
        }
    }
    return time;
}

int __metal_driver_cpu_mtimecmp_set (struct metal_cpu *cpu, unsigned long long time)
{
    int rc = -1;
    struct metal_interrupt *tmr_intc;
    struct __metal_driver_riscv_cpu_intc *intc;
    struct __metal_driver_cpu *_cpu = (void *)cpu;

    if (_cpu->interrupt_controller) {
        intc = (void *)_cpu->interrupt_controller;
        tmr_intc = intc->metal_int_table[METAL_INTERRUPT_ID_TMR].sub_int;
        if (tmr_intc) {
            rc = tmr_intc->vtable->command_request(tmr_intc,
                                                   METAL_TIMER_MTIME_SET, &time);
        }
    }
    return rc;
}

struct metal_interrupt *
__metal_driver_cpu_timer_controller_interrupt(struct metal_cpu *cpu)
{
#ifdef __METAL_DT_RISCV_CLINT0_HANDLE
    return __METAL_DT_RISCV_CLINT0_HANDLE;
#else
#ifdef __METAL_DT_SIFIVE_CLIC0_HANDLE
    return __METAL_DT_SIFIVE_CLIC0_HANDLE;
#else
#warning "There is no interrupt controller for Timer interrupt"
    return NULL;
#endif
#endif
}

int __metal_driver_cpu_get_timer_interrupt_id(struct metal_cpu *cpu)
{
    return METAL_INTERRUPT_ID_TMR;
}

struct metal_interrupt *
__metal_driver_cpu_sw_controller_interrupt(struct metal_cpu *cpu)
{
#ifdef __METAL_DT_RISCV_CLINT0_HANDLE
    return __METAL_DT_RISCV_CLINT0_HANDLE;
#else
#ifdef __METAL_DT_SIFIVE_CLIC0_HANDLE
    return __METAL_DT_SIFIVE_CLIC0_HANDLE;
#else
#warning "There is no interrupt controller for Software interrupt"
    return NULL;
#endif
#endif
}

int __metal_driver_cpu_get_sw_interrupt_id(struct metal_cpu *cpu)
{
    return METAL_INTERRUPT_ID_SW;
}

int __metal_driver_cpu_set_sw_ipi (struct metal_cpu *cpu, int hartid)
{
    int rc = -1;
    struct metal_interrupt *sw_intc;
    struct __metal_driver_riscv_cpu_intc *intc;
    struct __metal_driver_cpu *_cpu = (void *)cpu;

    if (_cpu->interrupt_controller) {
        intc = (void *)_cpu->interrupt_controller;
        sw_intc = intc->metal_int_table[METAL_INTERRUPT_ID_SW].sub_int;
        if (sw_intc) {
            rc = sw_intc->vtable->command_request(sw_intc,
                                                  METAL_SOFTWARE_IPI_SET, &hartid);
        }
    }
    return rc;
}

int __metal_driver_cpu_clear_sw_ipi (struct metal_cpu *cpu, int hartid)
{
    int rc = -1;
    struct metal_interrupt *sw_intc;
    struct __metal_driver_riscv_cpu_intc *intc;
    struct __metal_driver_cpu *_cpu = (void *)cpu;

    if (_cpu->interrupt_controller) {
        intc = (void *)_cpu->interrupt_controller;
        sw_intc = intc->metal_int_table[METAL_INTERRUPT_ID_SW].sub_int;
        if (sw_intc) {
            rc = sw_intc->vtable->command_request(sw_intc,
                                                  METAL_SOFTWARE_IPI_CLEAR, &hartid);
        }
    }
    return rc;
}

int __metal_driver_cpu_get_msip (struct metal_cpu *cpu, int hartid)
{
    int rc = 0;
    struct metal_interrupt *sw_intc;
    struct __metal_driver_riscv_cpu_intc *intc;
    struct __metal_driver_cpu *_cpu = (void *)cpu;

    if (_cpu->interrupt_controller) {
        intc = (void *)_cpu->interrupt_controller;
        sw_intc = intc->metal_int_table[METAL_INTERRUPT_ID_SW].sub_int;
        if (sw_intc) {
            rc = sw_intc->vtable->command_request(sw_intc,
                                                  METAL_SOFTWARE_MSIP_GET, &hartid);
        }
    }
    return rc;
}

struct metal_interrupt *
__metal_driver_cpu_controller_interrupt(struct metal_cpu *cpu)
{
    struct __metal_driver_cpu *cpu0 = (void *)cpu;
    return (struct metal_interrupt *)cpu0->interrupt_controller;
}

int __metal_driver_cpu_enable_interrupt(struct metal_cpu *cpu, void *priv)
{
    struct __metal_driver_cpu *cpu0 = (void *)cpu;
    if (cpu0->interrupt_controller) {
        /* Only support machine mode for now */
        __metal_interrupt_global_enable();
    	return 0;
    }
    return -1;
}

int __metal_driver_cpu_disable_interrupt(struct metal_cpu *cpu, void *priv)
{
    struct __metal_driver_cpu *cpu0 = (void *)cpu;
    if (cpu0->interrupt_controller) {
        /* Only support machine mode for now */
        __metal_interrupt_global_disable();
    	return 0;
    }
    return -1;
}

int __metal_driver_cpu_exception_register(struct metal_cpu *cpu, int ecode,
				        metal_exception_handler_t isr)
{
    struct __metal_driver_cpu *cpu0 = (void *)cpu;
    if (cpu0->interrupt_controller) {
	return __metal_exception_register(cpu0->interrupt_controller, ecode, isr);
    }
    return -1;
}

int  __metal_driver_cpu_get_instruction_length(struct metal_cpu *cpu, uintptr_t epc)
{
    /* Per ISA compressed instruction has last two bits of opcode set */
    return (*(unsigned short*)epc & 3) ? 4 : 2;
}

uintptr_t  __metal_driver_cpu_get_exception_pc(struct metal_cpu *cpu)
{
    uintptr_t mepc;
    asm volatile ("csrr %0, mepc" : "=r"(mepc));
    return mepc;
}

int  __metal_driver_cpu_set_exception_pc(struct metal_cpu *cpu, uintptr_t mepc)
{
    asm volatile ("csrw mepc, %0" :: "r"(mepc));
    return 0;
}
