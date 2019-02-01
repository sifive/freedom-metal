/* Copyright 2018 SiFive, Inc */
/* SPDX-License-Identifier: Apache-2.0 */

#include <stdint.h>
#include <mee/io.h>
#include <mee/shutdown.h>
#include <mee/machine.h>

struct mee_cpu *__mee_driver_cpu_get(int hartid)
{
    if (hartid < __MEE_DT_MAX_HARTS) {
        return &(__mee_cpu_table[hartid]->cpu);
    }
    return (struct mee_cpu *)NULL;
}

uintptr_t __mee_myhart_id (void)
{
    uintptr_t myhart;
    asm volatile ("csrr %0, mhartid" : "=r"(myhart));
    return myhart;
}

void __mee_zero_memory (unsigned char *base, unsigned int size)
{
    volatile unsigned char *ptr;
    for (ptr = base; ptr < (base + size); ptr++){
        *ptr = 0;
    }
}

void __mee_interrupt_global_enable (void) {
    uintptr_t m;
    asm volatile ("csrrs %0, mstatus, %1" : "=r"(m) : "r"(MEE_MIE_INTERRUPT));
}

void __mee_interrupt_global_disable (void) {
    uintptr_t m;
    asm volatile ("csrrc %0, mstatus, %1" : "=r"(m) : "r"(MEE_MIE_INTERRUPT));
}

void __mee_interrupt_software_enable (void) {
    uintptr_t m;
    asm volatile ("csrrs %0, mie, %1" : "=r"(m) : "r"(MEE_LOCAL_INTERRUPT_SW));
}

void __mee_interrupt_software_disable (void) {
    uintptr_t m;
    asm volatile ("csrrc %0, mie, %1" : "=r"(m) : "r"(MEE_LOCAL_INTERRUPT_SW));
}

void __mee_interrupt_timer_enable (void) {
    uintptr_t m;
    asm volatile ("csrrs %0, mie, %1" : "=r"(m) : "r"(MEE_LOCAL_INTERRUPT_TMR));
}

void __mee_interrupt_timer_disable (void) {
    uintptr_t m;
    asm volatile ("csrrc %0, mie, %1" : "=r"(m) : "r"(MEE_LOCAL_INTERRUPT_TMR));
}

void __mee_interrupt_external_enable (void) {
    uintptr_t m;
    asm volatile ("csrrs %0, mie, %1" : "=r"(m) : "r"(MEE_LOCAL_INTERRUPT_EXT));
}

void __mee_interrupt_external_disable (void) {
    unsigned long m;
    asm volatile ("csrrc %0, mie, %1" : "=r"(m) : "r"(MEE_LOCAL_INTERRUPT_EXT));
}

void __mee_interrupt_local_enable (int id) {
    uintptr_t b = 1 << id;
    uintptr_t m;
    asm volatile ("csrrs %0, mie, %1" : "=r"(m) : "r"(b));
}

void __mee_interrupt_local_disable (int id) {
    uintptr_t b = 1 << id;
    uintptr_t m;
    asm volatile ("csrrc %0, mie, %1" : "=r"(m) : "r"(b));
}

void __mee_default_exception_handler (struct mee_cpu *cpu, int ecode) {
    mee_shutdown(100);
}

void __mee_default_interrupt_handler (int id, void *priv) {
    mee_shutdown(200);
}

void __mee_default_sw_handler (int id, void *priv) {
    uintptr_t mcause;
    struct __mee_driver_riscv_cpu_intc *intc;
    struct __mee_driver_cpu *cpu = __mee_cpu_table[__mee_myhart_id()];

    asm volatile ("csrr %0, mcause" : "=r"(mcause));
    if ( cpu ) {
        intc = (struct __mee_driver_riscv_cpu_intc *)cpu->interrupt_controller;
        intc->mee_exception_table[mcause & MEE_MCAUSE_CAUSE]((struct mee_cpu *)cpu, id);
    }
}

void __mee_default_timer_handler (int id, void *priv) {
    struct mee_cpu *cpu = __mee_driver_cpu_get(__mee_myhart_id());
    unsigned long long time = __mee_driver_cpu_mtime_get(cpu);

    /* Set a 10 cycle timer */
    __mee_driver_cpu_mtimecmp_set(cpu, time + 10);
}

void __mee_exception_handler(void) __attribute__((interrupt, aligned(128)));
void __mee_exception_handler (void) {
    int id;
    void *priv;
    uintptr_t mcause, mepc, mtval, mtvec;
    struct __mee_driver_riscv_cpu_intc *intc;
    struct __mee_driver_cpu *cpu = __mee_cpu_table[__mee_myhart_id()];

    asm volatile ("csrr %0, mcause" : "=r"(mcause));
    asm volatile ("csrr %0, mepc" : "=r"(mepc));
    asm volatile ("csrr %0, mtval" : "=r"(mtval));
    asm volatile ("csrr %0, mtvec" : "=r"(mtvec));

    if ( cpu ) {
        intc = (struct __mee_driver_riscv_cpu_intc *)cpu->interrupt_controller;
        id = mcause & MEE_MCAUSE_CAUSE;
        if (mcause & MEE_MCAUSE_INTR) {
            if ((id < MEE_INTERRUPT_ID_LC0) || (mtvec & MEE_MTVEC_DIRECT)) {
                priv = intc->mee_int_table[id].exint_data;
                intc->mee_int_table[id].handler(id, priv);
		return;
            }
            if (mtvec & MEE_MTVEC_CLIC) {
    		uintptr_t mtvt;
    		mee_interrupt_handler_t mtvt_handler;

                asm volatile ("csrr %0, mtvt" : "=r"(mtvt));
               	priv = intc->mee_int_table[MEE_INTERRUPT_ID_SW].sub_int;
               	mtvt_handler = (mee_interrupt_handler_t)mtvt;
               	mtvt_handler(id, priv);
		return;
            }
        } else {
            intc->mee_exception_table[id]((struct mee_cpu *)cpu, id);
        }
    }
}

void __mee_controller_interrupt_vector (mee_vector_mode mode, void *vec_table)
{  
    uintptr_t trap_entry, val;

    asm volatile ("csrr %0, mtvec" : "=r"(val));
    val &= ~(MEE_MTVEC_CLIC_VECTORED | MEE_MTVEC_CLIC_RESERVED);
    trap_entry = (uintptr_t)vec_table;

    switch (mode) {
    case MEE_SELECTIVE_VECTOR_MODE:
        asm volatile ("csrw mtvt, %0" :: "r"(trap_entry | MEE_MTVEC_CLIC));
        asm volatile ("csrw mtvec, %0" :: "r"(val | MEE_MTVEC_CLIC));
        break;
    case MEE_HARDWARE_VECTOR_MODE:
        asm volatile ("csrw mtvt, %0" :: "r"(trap_entry | MEE_MTVEC_CLIC_VECTORED));
        asm volatile ("csrw mtvec, %0" :: "r"(val | MEE_MTVEC_CLIC_VECTORED));
        break;
    case MEE_VECTOR_MODE:
        asm volatile ("csrw mtvec, %0" :: "r"(trap_entry | MEE_MTVEC_VECTORED));
        break;
    case MEE_DIRECT_MODE:
        asm volatile ("csrw mtvec, %0" :: "r"(trap_entry & ~MEE_MTVEC_CLIC_VECTORED));
        break;
    }
}

int __mee_valid_interrupt_id (int id)
{
    switch (id) {
    case MEE_INTERRUPT_ID_SW:
    case MEE_INTERRUPT_ID_TMR:
    case MEE_INTERRUPT_ID_EXT:
    case MEE_INTERRUPT_ID_LC0:
    case MEE_INTERRUPT_ID_LC1:
    case MEE_INTERRUPT_ID_LC2:
    case MEE_INTERRUPT_ID_LC3:
    case MEE_INTERRUPT_ID_LC4:
    case MEE_INTERRUPT_ID_LC5:
    case MEE_INTERRUPT_ID_LC6:
    case MEE_INTERRUPT_ID_LC7:
    case MEE_INTERRUPT_ID_LC8:
    case MEE_INTERRUPT_ID_LC9:
    case MEE_INTERRUPT_ID_LC10:
    case MEE_INTERRUPT_ID_LC11:
    case MEE_INTERRUPT_ID_LC12:
    case MEE_INTERRUPT_ID_LC13:
    case MEE_INTERRUPT_ID_LC14:
    case MEE_INTERRUPT_ID_LC15:
        return 1;
    default:
        break;
    }

    return 0;
}

 
int __mee_local_interrupt_enable (struct mee_interrupt *controller,
				  mee_interrupt_id_e id, int enable)
{
    int rc = 0;
  
    if ( !controller) {
        return -1;
    }

    switch (id) {
    case MEE_INTERRUPT_ID_BASE:
        if (enable) {
            __mee_interrupt_global_enable();
        } else {
            __mee_interrupt_global_disable();
        }
        break;
    case MEE_INTERRUPT_ID_SW:
        if (enable) {
            __mee_interrupt_software_enable();
        } else {
            __mee_interrupt_software_disable();
        }
        break;
    case MEE_INTERRUPT_ID_TMR:
        if (enable) {
	    __mee_interrupt_timer_enable();
	} else {
	    __mee_interrupt_timer_disable();
	}
	break;
    case MEE_INTERRUPT_ID_EXT:
        if (enable) {
            __mee_interrupt_external_enable();
        } else {
            __mee_interrupt_external_disable();
        }
        break;
    case MEE_INTERRUPT_ID_LC0:
    case MEE_INTERRUPT_ID_LC1:
    case MEE_INTERRUPT_ID_LC2:
    case MEE_INTERRUPT_ID_LC3:
    case MEE_INTERRUPT_ID_LC4:
    case MEE_INTERRUPT_ID_LC5:
    case MEE_INTERRUPT_ID_LC6:
    case MEE_INTERRUPT_ID_LC7:
    case MEE_INTERRUPT_ID_LC8:
    case MEE_INTERRUPT_ID_LC9:
    case MEE_INTERRUPT_ID_LC10:
    case MEE_INTERRUPT_ID_LC11:
    case MEE_INTERRUPT_ID_LC12:
    case MEE_INTERRUPT_ID_LC13:
    case MEE_INTERRUPT_ID_LC14:
    case MEE_INTERRUPT_ID_LC15:
        if (enable) {
            __mee_interrupt_local_enable(id);
        } else {
            __mee_interrupt_local_disable(id);
        }
        break;
    defaut:
        rc = -1;
    }
    return rc;
}

int __mee_exception_register (struct mee_interrupt *controller,
                              int ecode, mee_exception_handler_t isr)
{
    struct __mee_driver_riscv_cpu_intc *intc = (void *)(controller);

    if ((ecode < MEE_MAX_EXCEPTION_CODE) && isr) {
        intc->mee_exception_table[ecode] = isr;
        return 0;
    }
    return -1;
}

void __mee_driver_riscv_cpu_controller_interrupt_init (struct mee_interrupt *controller)
{
    struct __mee_driver_riscv_cpu_intc *intc = (void *)(controller);

    if ( !intc->init_done ) {
        /* Default to use direct interrupt, setup sw cb table*/
        for (int i = 0; i < MEE_MAX_MI; i++) {
            intc->mee_int_table[i].handler = NULL;
            intc->mee_int_table[i].sub_int = NULL;
            intc->mee_int_table[i].exint_data = NULL;
	}
	for (int i = 0; i < MEE_MAX_ME; i++) {
	    intc->mee_exception_table[i] = __mee_default_exception_handler;
	}
        __mee_controller_interrupt_vector(MEE_DIRECT_MODE, &__mee_exception_handler);
	intc->init_done = 1;
    }
}

int __mee_driver_riscv_cpu_controller_interrupt_register(struct mee_interrupt *controller,
							 int id, mee_interrupt_handler_t isr,
							 void *priv)
{
    int rc = 0;
    struct __mee_driver_riscv_cpu_intc *intc = (void *)(controller);
 
    if ( !__mee_valid_interrupt_id(id) ) {
        return -11;
    }

    if (isr) {
        intc->mee_int_table[id].handler = isr;
        intc->mee_int_table[id].exint_data = priv;
    } else {
	switch (id) {
	case MEE_INTERRUPT_ID_SW:
            intc->mee_int_table[id].handler = __mee_default_sw_handler;
            intc->mee_int_table[id].sub_int = priv;
	  break;
	case MEE_INTERRUPT_ID_TMR:
            intc->mee_int_table[id].handler = __mee_default_timer_handler;
            intc->mee_int_table[id].sub_int = priv;
	  break;
	case MEE_INTERRUPT_ID_EXT:
	case MEE_INTERRUPT_ID_LC0:
	case MEE_INTERRUPT_ID_LC1:
	case MEE_INTERRUPT_ID_LC2:
	case MEE_INTERRUPT_ID_LC3:
	case MEE_INTERRUPT_ID_LC4:
	case MEE_INTERRUPT_ID_LC5:
	case MEE_INTERRUPT_ID_LC6:
	case MEE_INTERRUPT_ID_LC7:
	case MEE_INTERRUPT_ID_LC8:
	case MEE_INTERRUPT_ID_LC9:
	case MEE_INTERRUPT_ID_LC10:
	case MEE_INTERRUPT_ID_LC11:
	case MEE_INTERRUPT_ID_LC12:
	case MEE_INTERRUPT_ID_LC13:
	case MEE_INTERRUPT_ID_LC14:
	case MEE_INTERRUPT_ID_LC15:
            intc->mee_int_table[id].handler = __mee_default_interrupt_handler;
            intc->mee_int_table[id].sub_int = priv;
	  break;
	defaut:
	  rc = -12;
	}
    }
    return rc;
}

int __mee_driver_riscv_cpu_controller_interrupt_enable (struct mee_interrupt *controller,
                                                        int id)
{
    return __mee_local_interrupt_enable(controller, id, MEE_ENABLE);
}

int __mee_driver_riscv_cpu_controller_interrupt_disable (struct mee_interrupt *controller,
                                                         int id)
{   
    return __mee_local_interrupt_enable(controller, id, MEE_DISABLE);
}

int __mee_driver_riscv_cpu_controller_interrupt_enable_vector(struct mee_interrupt *controller,
                                                             int id, mee_vector_mode mode)
{
    struct __mee_driver_riscv_cpu_intc *intc = (void *)(controller);

    if (id == MEE_INTERRUPT_ID_BASE) {
        if (mode == MEE_DIRECT_MODE) {
            __mee_controller_interrupt_vector(mode, &__mee_exception_handler);
            return 0;
        }   
        if (mode == MEE_VECTOR_MODE) {
            __mee_controller_interrupt_vector(mode, &intc->mee_mtvec_table);
            return 0;
        }
    }
    return -1;
}

int __mee_driver_riscv_cpu_controller_interrupt_disable_vector(struct mee_interrupt *controller,
                                                              int id)
{
    struct __mee_driver_riscv_cpu_intc *intc = (void *)(controller);

    if (id == MEE_INTERRUPT_ID_BASE) {
        __mee_controller_interrupt_vector(MEE_DIRECT_MODE, &__mee_exception_handler);
        return 0;
    }
    return -1;
}

int __mee_driver_riscv_cpu_controller_command_request (struct mee_interrupt *controller,
						       int cmd, void *data)
{
    /* NOP for now, unless local interrupt lines the like of clic, clint, plic */
    return 0;
}

extern inline int __mee_controller_interrupt_is_selective_vectored(void);

/* CPU driver !!! */

unsigned long long __mee_driver_cpu_timer_get(struct mee_cpu *cpu)
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

unsigned long long __mee_driver_cpu_timebase_get(struct mee_cpu *cpu)
{
    struct __mee_driver_cpu *_cpu = (void *)(cpu);
    if (!_cpu) {
        return 0;
    }

    return _cpu->timebase;
}

unsigned long long  __mee_driver_cpu_mtime_get (struct mee_cpu *cpu)
{
    unsigned long long time = 0;
    struct mee_interrupt *tmr_intc;
    struct __mee_driver_riscv_cpu_intc *intc;
    struct __mee_driver_cpu *_cpu = (void *)cpu;

    if (_cpu->interrupt_controller) {
        intc = (void *)_cpu->interrupt_controller;
        tmr_intc = intc->mee_int_table[MEE_INTERRUPT_ID_TMR].sub_int;
        if (tmr_intc) {
            tmr_intc->vtable->command_request(tmr_intc,
                                              MEE_TIMER_MTIME_GET, &time);
        }
    }
    return time;
}

int __mee_driver_cpu_mtimecmp_set (struct mee_cpu *cpu, unsigned long long time)
{
    int rc = -1;
    struct mee_interrupt *tmr_intc;
    struct __mee_driver_riscv_cpu_intc *intc;
    struct __mee_driver_cpu *_cpu = (void *)cpu;

    if (_cpu->interrupt_controller) {
        intc = (void *)_cpu->interrupt_controller;
        tmr_intc = intc->mee_int_table[MEE_INTERRUPT_ID_TMR].sub_int;
        if (tmr_intc) {
            rc = tmr_intc->vtable->command_request(tmr_intc,
                                                   MEE_TIMER_MTIME_SET, &time);
        }
    }
    return rc;
}

struct mee_interrupt *
__mee_driver_cpu_timer_controller_interrupt(struct mee_cpu *cpu)
{
#ifdef __MEE_DT_RISCV_CLINT0_HANDLE
    return __MEE_DT_RISCV_CLINT0_HANDLE;
#else
#ifdef __MEE_DT_SIFIVE_CLIC0_HANDLE
    return __MEE_DT_SIFIVE_CLIC0_HANDLE;
#else
#warning "There is no interrupt controller for Timer interrupt"
    return NULL;
#endif
#endif
}

int __mee_driver_cpu_get_timer_interrupt_id(struct mee_cpu *cpu)
{
    return MEE_INTERRUPT_ID_TMR;
}

struct mee_interrupt *
__mee_driver_cpu_sw_controller_interrupt(struct mee_cpu *cpu)
{
#ifdef __MEE_DT_RISCV_CLINT0_HANDLE
    return __MEE_DT_RISCV_CLINT0_HANDLE;
#else
#ifdef __MEE_DT_SIFIVE_CLIC0_HANDLE
    return __MEE_DT_SIFIVE_CLIC0_HANDLE;
#else
#warning "There is no interrupt controller for Software interrupt"
    return NULL;
#endif
#endif
}

int __mee_driver_cpu_get_sw_interrupt_id(struct mee_cpu *cpu)
{
    return MEE_INTERRUPT_ID_SW;
}

int __mee_driver_cpu_set_sw_ipi (struct mee_cpu *cpu, int hartid)
{
    int rc = -1;
    struct mee_interrupt *sw_intc;
    struct __mee_driver_riscv_cpu_intc *intc;
    struct __mee_driver_cpu *_cpu = (void *)cpu;

    if (_cpu->interrupt_controller) {
        intc = (void *)_cpu->interrupt_controller;
        sw_intc = intc->mee_int_table[MEE_INTERRUPT_ID_SW].sub_int;
        if (sw_intc) {
            rc = sw_intc->vtable->command_request(sw_intc,
                                                  MEE_SOFTWARE_IPI_SET, &hartid);
        }
    }
    return rc;
}

int __mee_driver_cpu_clear_sw_ipi (struct mee_cpu *cpu, int hartid)
{
    int rc = -1;
    struct mee_interrupt *sw_intc;
    struct __mee_driver_riscv_cpu_intc *intc;
    struct __mee_driver_cpu *_cpu = (void *)cpu;

    if (_cpu->interrupt_controller) {
        intc = (void *)_cpu->interrupt_controller;
        sw_intc = intc->mee_int_table[MEE_INTERRUPT_ID_SW].sub_int;
        if (sw_intc) {
            rc = sw_intc->vtable->command_request(sw_intc,
                                                  MEE_SOFTWARE_IPI_CLEAR, &hartid);
        }
    }
    return rc;
}

int __mee_driver_cpu_get_msip (struct mee_cpu *cpu, int hartid)
{
    int rc = 0;
    struct mee_interrupt *sw_intc;
    struct __mee_driver_riscv_cpu_intc *intc;
    struct __mee_driver_cpu *_cpu = (void *)cpu;

    if (_cpu->interrupt_controller) {
        intc = (void *)_cpu->interrupt_controller;
        sw_intc = intc->mee_int_table[MEE_INTERRUPT_ID_SW].sub_int;
        if (sw_intc) {
            rc = sw_intc->vtable->command_request(sw_intc,
                                                  MEE_SOFTWARE_MSIP_GET, &hartid);
        }
    }
    return rc;
}

struct mee_interrupt *
__mee_driver_cpu_controller_interrupt(struct mee_cpu *cpu)
{
    struct __mee_driver_cpu *cpu0 = (void *)cpu;
    return (struct mee_interrupt *)cpu0->interrupt_controller;
}

int __mee_driver_cpu_enable_interrupt(struct mee_cpu *cpu, void *priv)
{
    struct __mee_driver_cpu *cpu0 = (void *)cpu;
    if (cpu0->interrupt_controller) {
        /* Only support machine mode for now */
        __mee_interrupt_global_enable();
    	return 0;
    }
    return -1;
}

int __mee_driver_cpu_disable_interrupt(struct mee_cpu *cpu, void *priv)
{
    struct __mee_driver_cpu *cpu0 = (void *)cpu;
    if (cpu0->interrupt_controller) {
        /* Only support machine mode for now */
        __mee_interrupt_global_disable();
    	return 0;
    }
    return -1;
}

int __mee_driver_cpu_exception_register(struct mee_cpu *cpu, int ecode,
				        mee_exception_handler_t isr)
{
    struct __mee_driver_cpu *cpu0 = (void *)cpu;
    if (cpu0->interrupt_controller) {
	return __mee_exception_register(cpu0->interrupt_controller, ecode, isr);
    }
    return -1;
}

int  __mee_driver_cpu_get_instruction_length(struct mee_cpu *cpu, uintptr_t epc)
{
    /* Per ISA compressed instruction has last two bits of opcode set */
    return (*(unsigned short*)epc & 3) ? 4 : 2;
}

uintptr_t  __mee_driver_cpu_get_exception_pc(struct mee_cpu *cpu)
{
    uintptr_t mepc;
    asm volatile ("csrr %0, mepc" : "=r"(mepc));
    return mepc;
}

int  __mee_driver_cpu_set_exception_pc(struct mee_cpu *cpu, uintptr_t mepc)
{
    asm volatile ("csrw mepc, %0" :: "r"(mepc));
    return 0;
}
