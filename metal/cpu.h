/* Copyright 2018 SiFive, Inc */

/* SPDX-License-Identifier: Apache-2.0 */

/*! @file cpu.h
 *  @brief API for accessing CPU capabilities.
 */

#ifndef METAL__CPU_H
#define METAL__CPU_H

#include <assert.h>
#include <metal/private/metal_private_cpu.h>
#include <stdint.h>

/*! @brief A device handle for a CPU hart
 */
struct metal_cpu {
    uint32_t __hartid;
};

typedef enum {
    METAL_MACHINE_PRIVILEGE_MODE,
    METAL_SUPERVISOR_PRIVILEGE_MODE,
    METAL_USER_PRIVILEGE_MODE,
} metal_privilege_mode_e;

/*! @brief Get a reference to a CPU hart
 *
 * @param hartid The ID of the desired CPU hart
 * @return A pointer to the CPU device handle
 */
static __inline__ struct metal_cpu metal_cpu_get(unsigned int hartid) {
    assert(hartid < __METAL_DT_NUM_HARTS);
    return (struct metal_cpu){hartid};
}

/*! @brief Get the hartid of the CPU hart executing this function
 *
 * @return The hartid of the current CPU hart */
static __inline__ int metal_cpu_get_current_hartid(void) {
    int mhartid;
    __asm__ volatile("csrr %0, mhartid" : "=r"(mhartid));
    return mhartid;
}

static __inline__ struct metal_cpu metal_cpu_get_current_hart(void) {
    return metal_cpu_get(metal_cpu_get_current_hartid());
}

/*! @brief Get the number of CPU harts
 *
 * @return The number of CPU harts */
static __inline__ int metal_cpu_get_num_harts(void) {
    return __METAL_DT_NUM_HARTS;
}

/*!
 * @brief Enables the global interrupt enable for the hart
 */
void metal_cpu_enable_interrupts(void);

/*!
 * @brief Disables the global interrupt enable for the hart
 */
void metal_cpu_disable_interrupts(void);

/*!
 * @brief Enables the inter-process interrupt for the hart
 */
void metal_cpu_enable_ipi(void);

/*!
 * @brief Disables the inter-process interrupt for the hart
 */
void metal_cpu_disable_ipi(void);

/*!
 * @brief Enables the timer interrupt for the hart
 */
void metal_cpu_enable_timer_interrupt(void);

/*!
 * @brief Disables the timer interrupt for the hart
 */
void metal_cpu_disable_timer_interrupt(void);

/*!
 * @brief Enables the external interrupt for the hart
 */
void metal_cpu_enable_external_interrupt(void);

/*!
 * @brief Diasble sthe external interrupt for the hart
 */
void metal_cpu_disable_external_interrupt(void);

/*! @brief Get the CPU cycle count timer value
 *
 * Get the value of the cycle count timer for a given CPU
 *
 * @param cpu The CPU device handle
 * @return The value of the CPU cycle count timer
 */
unsigned long long metal_cpu_get_timer(struct metal_cpu cpu);

/*! @brief Get the timebase of the CPU
 *
 * Get the value of the timebase of the cycle count timer
 *
 * @param cpu The CPU device handle
 * @return The value of the cycle count timer timebase
 */
unsigned long long metal_cpu_get_timebase(struct metal_cpu cpu);

/*! @brief Get the value of the mtime RTC
 *
 * Get the value of the mtime real-time clock. The CPU interrupt controller
 * must be initialized before this function is called or the return value
 * will be 0.
 *
 * @param cpu The CPU device handle
 * @return The value of mtime, or 0 if failure
 */
uint64_t metal_cpu_get_mtime(struct metal_cpu cpu);

/*! @brief Set the value of the RTC mtimecmp RTC
 *
 * Set the value of the mtime real-time clock compare register. The CPU
 * interrupt controller must be initialized before this function is called
 * or the return value will be -1;
 *
 * @param cpu The CPU device handle
 * @param time The value to set the compare register to
 * @return The value of mtimecmp or -1 if error
 */
int metal_cpu_set_mtimecmp(struct metal_cpu cpu, uint64_t time);

/*!
 * @brief Set the inter-process interrupt for a hart
 *
 * Trigger a software/inter-process interrupt for a hart. The CPU interrupt
 * controller for the CPU handle passed to this function must be initialized
 * before this function is called.
 *
 * @param cpu The CPU device handle for the hart to be interrupted
 * @return 0 upon success
 */
void metal_cpu_set_ipi(struct metal_cpu cpu);

/*!
 * @brief Clear the inter-process interrupt for a hart
 *
 * Clear the software/inter-process interrupt for a hart. The CPU interrupt
 * controller for the CPU handle passed to this function must be initialized
 * before this function is called.
 *
 * @param cpu The CPU device handle for the hart to clear
 * @return 0 upon success
 */
void metal_cpu_clear_ipi(struct metal_cpu cpu);

/*!
 * @brief Get the value of MSIP for the given hart
 *
 * Get the value of the machine software interrupt pending bit for
 * the given hart. The CPU interrupt controller for the CPU handle passed
 * as argument to this function must be initialized before this function
 * is called.
 *
 * @param cpu the CPU device handle for the hart
 * @return 0 upon success
 */
int metal_cpu_get_ipi(struct metal_cpu cpu);

/*!
 * @brief Get the length of an instruction in bytes
 *
 * Get the length of an instruction in bytes.
 *
 * On RISC-V platforms, this is useful for detecting whether an instruction is
 * compressed (2 bytes long) or uncompressed (4 bytes long).
 *
 * This function is useful in conjuction with `metal_cpu_get_exception_pc()`
 * and `metal_cpu_set_exception_pc()` in order to cause the exception handler to
 * return execution after the faulting instruction.
 *
 * @param cpu The CPU device handle
 * @param epc The address of the instruction to measure
 * @return the length of the instruction in bytes
 */
int metal_cpu_get_instruction_length(struct metal_cpu cpu, uintptr_t epc);

/*!
 * @brief Get the program counter of the current exception.
 *
 * This function must be called within an exception handler. The behavior is
 * undefined outside of an exception handler.
 *
 * @param cpu The CPU device handle
 * @return The value of the program counter at the time of the exception
 */
uintptr_t metal_cpu_get_exception_pc(struct metal_cpu cpu);

/*!
 * @brief Set the exception program counter
 *
 * This function must be called within an exception handler. The behavior
 * is undefined outside of an exception handler.
 *
 * This function can be used to cause an exception handler to return execution
 * to an address other than the one that caused the exception.
 *
 * @param cpu the CPU device handle
 * @param epc The address to set the exception program counter to
 * @return 0 upon success
 */
int metal_cpu_set_exception_pc(struct metal_cpu cpu, uintptr_t epc);

#endif
