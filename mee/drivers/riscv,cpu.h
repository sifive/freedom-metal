/* Copyright 2018 SiFive, Inc */
/* SPDX-License-Identifier: Apache-2.0 */

#ifndef MEE__DRIVERS__RISCV_CPU_H
#define MEE__DRIVERS__RISCV_CPU_H

#include <mee/cpu.h>
#include <mee/compiler.h>

#define MEE_MAX_CORES             8
#define MEE_MAX_MI                32  /* Per ISA MCause interrupts 32+ are Reserved */
#define MEE_MAX_ME                12  /* Per ISA Exception codes 12+ are Reserved   */
#define MEE_DEFAULT_RTC_FREQ      32768

#define MEE_DISABLE              0
#define MEE_ENABLE               1

#define MEE_MTVEC_DIRECT         0x00
#define MEE_MTVEC_VECTORED       0x01
#define MEE_MTVEC_CLIC           0x02
#define MEE_MTVEC_CLIC_VECTORED  0x03
#define MEE_MCAUSE_INTR          0x80000000UL // gen from mee-header!!
#define MEE_MCAUSE_CAUSE         0x000003FFUL // gen from mee-header!!
#define MEE_LOCAL_INTR(X)        (16 + X)
#define MEE_MCAUSE_EVAL(cause)   (cause & MEE_MCAUSE_INTR)
#define MEE_INTERRUPT(cause)     (MEE_MCAUSE_EVAL(cause) ? 1 : 0)
#define MEE_EXCEPTION(cause)     (MEE_MCAUSE_EVAL(cause) ? 0 : 1)
#define MEE_SW_INTR_EXCEPTION    (MEE_MCAUSE_INTR + 3)
#define MEE_TMR_INTR_EXCEPTION   (MEE_MCAUSE_INTR + 7)
#define MEE_EXT_INTR_EXCEPTION   (MEE_MCAUSE_INTR + 11)
#define MEE_LOCAL_INTR_EXCEPTION(X) (MEE_MCAUSE_INTR + MEE_LOCAL_INTR(X))
#define MEE_LOCAL_INTR_RESERVE0  1
#define MEE_LOCAL_INTR_RESERVE1  2
#define MEE_LOCAL_INTR_RESERVE2  4
#define MEE_LOCAL_INTERRUPT_SW   8             /* Bit3 0x008 */
#define MEE_LOCAL_INTR_RESERVE4  16
#define MEE_LOCAL_INTR_RESERVE5  32
#define MEE_LOCAL_INTR_RESERVE6  64
#define MEE_LOCAL_INTERRUPT_TMR  128           /* Bit7 0x080 */
#define MEE_LOCAL_INTR_RESERVE8  256
#define MEE_LOCAL_INTR_RESERVE9  512
#define MEE_LOCAL_INTR_RESERVE10 1024   
#define MEE_LOCAL_INTERRUPT_EXT  2048          /* Bit11 0x800 */
/* Bit12 to Bit15 are Reserved */
#define MEE_LOCAL_INTERRUPT(X)  (0x10000 << X) /* Bit16+ Start of Custom Local Interrupt */
#define MEE_MIE_INTERRUPT        0x08

typedef enum {
  MEE_MACHINE_PRIVILEGE_MODE,
  MEE_SUPERVISOR_PRIVILEGE_MODE,
  MEE_USER_PRIVILEGE_MODE,
} mee_privilege_mode_e;

typedef enum {
  MEE_INTERRUPT_ID_BASE,
  MEE_INTERRUPT_ID_SW   = (MEE_INTERRUPT_ID_BASE + 3),
  MEE_INTERRUPT_ID_TMR  = (MEE_INTERRUPT_ID_BASE + 7),
  MEE_INTERRUPT_ID_EXT  = (MEE_INTERRUPT_ID_BASE + 11),
  MEE_INTERRUPT_ID_LC0  = (MEE_INTERRUPT_ID_BASE + MEE_LOCAL_INTR(0)),
  MEE_INTERRUPT_ID_LC1  = (MEE_INTERRUPT_ID_BASE + MEE_LOCAL_INTR(1)),
  MEE_INTERRUPT_ID_LC2  = (MEE_INTERRUPT_ID_BASE + MEE_LOCAL_INTR(2)),
  MEE_INTERRUPT_ID_LC3  = (MEE_INTERRUPT_ID_BASE + MEE_LOCAL_INTR(3)),
  MEE_INTERRUPT_ID_LC4  = (MEE_INTERRUPT_ID_BASE + MEE_LOCAL_INTR(4)),
  MEE_INTERRUPT_ID_LC5  = (MEE_INTERRUPT_ID_BASE + MEE_LOCAL_INTR(5)),
  MEE_INTERRUPT_ID_LC6  = (MEE_INTERRUPT_ID_BASE + MEE_LOCAL_INTR(6)),
  MEE_INTERRUPT_ID_LC7  = (MEE_INTERRUPT_ID_BASE + MEE_LOCAL_INTR(7)),
  MEE_INTERRUPT_ID_LC8  = (MEE_INTERRUPT_ID_BASE + MEE_LOCAL_INTR(8)),
  MEE_INTERRUPT_ID_LC9  = (MEE_INTERRUPT_ID_BASE + MEE_LOCAL_INTR(9)),
  MEE_INTERRUPT_ID_LC10 = (MEE_INTERRUPT_ID_BASE + MEE_LOCAL_INTR(10)),
  MEE_INTERRUPT_ID_LC11 = (MEE_INTERRUPT_ID_BASE + MEE_LOCAL_INTR(11)),
  MEE_INTERRUPT_ID_LC12 = (MEE_INTERRUPT_ID_BASE + MEE_LOCAL_INTR(12)),
  MEE_INTERRUPT_ID_LC13 = (MEE_INTERRUPT_ID_BASE + MEE_LOCAL_INTR(13)),
  MEE_INTERRUPT_ID_LC14 = (MEE_INTERRUPT_ID_BASE + MEE_LOCAL_INTR(14)),
  MEE_INTERRUPT_ID_LC15 = (MEE_INTERRUPT_ID_BASE + MEE_LOCAL_INTR(15)),
  MEE_INTERRUPT_ID_LCMX,
  MEE_INTERRUPT_ID_GL0 = MEE_INTERRUPT_ID_LCMX,
  MEE_INTERRUPT_ID_GLMX = (MEE_MCAUSE_CAUSE + 1),
} mee_interrupt_id_e;

typedef enum {
  MEE_IAM_EXCEPTION_CODE,     /* Instruction address misaligned */
  MEE_IAF_EXCEPTION_CODE,     /* Instruction access faultd */
  MEE_II_EXCEPTION_CODE,      /* Illegal instruction */
  MEE_BREAK_EXCEPTION_CODE,   /* Breakpoint */
  MEE_LAM_EXCEPTION_CODE,     /* Load address misaligned */
  MEE_LAF_EXCEPTION_CODE,     /* Load access fault */
  MEE_SAMOAM_EXCEPTION_CODE,  /* Store/AMO address misaligned */
  MEE_SAMOAF_EXCEPTION_CODE,  /* Store/AMO access fault */
  MEE_ECALL_U_EXCEPTION_CODE, /* Environment call from U-mode */
  MEE_R9_EXCEPTION_CODE,      /* Reserved */
  MEE_R10_EXCEPTION_CODE,     /* Reserved */
  MEE_ECALL_M_EXCEPTION_CODE, /* Environment call from M-mode */
  MEE_MAX_EXCEPTION_CODE,
} mee_exception_code_e;

typedef enum {
  MEE_TIMER_MTIME_GET = 1,
  MEE_TIMER_MTIME_SET,
  MEE_SOFTWARE_IPI_CLEAR,
  MEE_SOFTWARE_IPI_SET,
  MEE_SOFTWARE_MSIP_GET,
} mee_interrup_cmd_e;

typedef struct __mee_interrupt_data {
    long long pad : 64;
    mee_interrupt_handler_t  handler;
    void *sub_int;
    void *exint_data;
} __mee_interrupt_data;

/* CPU interrupt controller */

unsigned long __mee_myhart_id(void);

struct __mee_driver_interrupt_controller_vtable {
    void (*interrupt_init)(struct mee_interrupt *controller);
    int (*interrupt_register)(struct mee_interrupt *controller,
			      int id, mee_interrupt_handler_t isr, void *priv_data);
    int (*interrupt_enable)(struct mee_interrupt *controller, int id);
    int (*interrupt_disable)(struct mee_interrupt *controller, int id);
    int (*command_request)(struct mee_interrupt *intr, int cmd, void *data);
};

struct __mee_driver_vtable_riscv_cpu_intc {
  struct mee_interrupt_vtable controller_vtable;
};

void __mee_driver_riscv_cpu_controller_interrupt_init(struct mee_interrupt *controller);
int __mee_driver_riscv_cpu_controller_interrupt_register(struct mee_interrupt *controller,
							 int id, mee_interrupt_handler_t isr,
							 void *priv_data);
int __mee_driver_riscv_cpu_controller_interrupt_enable(struct mee_interrupt *controller, int id);
int __mee_driver_riscv_cpu_controller_interrupt_disable(struct mee_interrupt *controller, int id);
int __mee_driver_riscv_cpu_controller_command_request(struct mee_interrupt *controller,
						      int cmd, void *data);

__MEE_DECLARE_VTABLE(__mee_driver_vtable_riscv_cpu_intc) = {
    .controller_vtable.interrupt_init = __mee_driver_riscv_cpu_controller_interrupt_init,
    .controller_vtable.interrupt_register = __mee_driver_riscv_cpu_controller_interrupt_register,
    .controller_vtable.interrupt_enable   = __mee_driver_riscv_cpu_controller_interrupt_enable,
    .controller_vtable.interrupt_disable  = __mee_driver_riscv_cpu_controller_interrupt_disable,
    .controller_vtable.command_request    = __mee_driver_riscv_cpu_controller_command_request,
};

struct __mee_driver_riscv_cpu_intc {
    struct mee_interrupt controller;
    const struct __mee_driver_vtable_riscv_cpu_intc *vtable;
    int init_done;
    int interrupt_controller;
    __mee_interrupt_data mee_mtvec_table[MEE_MAX_MI];
    mee_exception_handler_t mee_exception_table[MEE_MAX_ME];
};

/* CPU driver*/
struct __mee_driver_vtable_cpu {
  struct mee_cpu_vtable cpu_vtable;
};

int  __mee_driver_cpu_timer_get(struct mee_cpu *cpu, int hartid, unsigned long long *tv);
int  __mee_driver_cpu_timebase_get(struct mee_cpu *cpu, unsigned long long *tb);
unsigned long long
      __mee_driver_cpu_mtime_get(struct mee_cpu *cpu);
int  __mee_driver_cpu_mtimecmp_set(struct mee_cpu *cpu, unsigned long long time);
struct mee_interrupt*
     __mee_driver_cpu_timer_controller_interrupt(struct mee_cpu *cpu);
int  __mee_driver_cpu_get_timer_interrupt_id(struct mee_cpu *cpu);
struct mee_interrupt*
     __mee_driver_cpu_sw_controller_interrupt(struct mee_cpu *cpu);
int  __mee_driver_cpu_get_sw_interrupt_id(struct mee_cpu *cpu);
int  __mee_driver_cpu_set_sw_ipi(struct mee_cpu *cpu, int hartid);
int  __mee_driver_cpu_clear_sw_ipi(struct mee_cpu *cpu, int hartid);
int  __mee_driver_cpu_get_msip(struct mee_cpu *cpu, int hartid);
struct mee_interrupt*
     __mee_driver_cpu_controller_interrupt(struct mee_cpu *cpu);
int  __mee_driver_cpu_enable_interrupt_vector(struct mee_cpu *cpu);
int  __mee_driver_cpu_disable_interrupt_vector(struct mee_cpu *cpu);
int  __mee_driver_cpu_exception_register(struct mee_cpu *cpu, int ecode,
					 mee_exception_handler_t isr);
int  __mee_driver_cpu_get_instruction_length(struct mee_cpu *cpu, unsigned long epc);
unsigned long  __mee_driver_cpu_get_exception_pc(struct mee_cpu *cpu);
int  __mee_driver_cpu_set_exception_pc(struct mee_cpu *cpu, unsigned long epc);

__MEE_DECLARE_VTABLE(__mee_driver_vtable_cpu) = {
    .cpu_vtable.timer_get     = __mee_driver_cpu_timer_get,
    .cpu_vtable.timebase_get  = __mee_driver_cpu_timebase_get,
    .cpu_vtable.mtime_get = __mee_driver_cpu_mtime_get,
    .cpu_vtable.mtimecmp_set = __mee_driver_cpu_mtimecmp_set,
    .cpu_vtable.tmr_controller_interrupt = __mee_driver_cpu_timer_controller_interrupt,
    .cpu_vtable.get_tmr_interrupt_id = __mee_driver_cpu_get_timer_interrupt_id,
    .cpu_vtable.sw_controller_interrupt = __mee_driver_cpu_sw_controller_interrupt,
    .cpu_vtable.get_sw_interrupt_id = __mee_driver_cpu_get_sw_interrupt_id,
    .cpu_vtable.set_sw_ipi = __mee_driver_cpu_set_sw_ipi,
    .cpu_vtable.clear_sw_ipi = __mee_driver_cpu_clear_sw_ipi,
    .cpu_vtable.get_msip = __mee_driver_cpu_get_msip,
    .cpu_vtable.controller_interrupt = __mee_driver_cpu_controller_interrupt,
    .cpu_vtable.exception_register = __mee_driver_cpu_exception_register,
    .cpu_vtable.get_ilen = __mee_driver_cpu_get_instruction_length,
    .cpu_vtable.get_epc = __mee_driver_cpu_get_exception_pc,
    .cpu_vtable.set_epc = __mee_driver_cpu_set_exception_pc,
};

struct __mee_driver_cpu {
    struct mee_cpu cpu;
    const struct __mee_driver_vtable_cpu *vtable;
    const int timebase;    
    struct mee_interrupt *interrupt_controller;
};

#endif
