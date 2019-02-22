/* Copyright 2019 SiFive, Inc */
/* SPDX-License-Identifier: Apache-2.0 */

#include <metal/lock.h>
#include <metal/compiler.h>

#define METAL_STORE_AMO_ACCESS_FAULT 7

inline int metal_lock_init(struct metal_lock *lock) {
#ifdef __riscv_atomic
    lock->_state = 0;

    return 0;
#else
    return 1;
#endif
}

inline int metal_lock_take(struct metal_lock *lock) {
#ifdef __riscv_atomic
    int old = 1;
    int new = 1;

    while(old != 0) {
        __asm__ volatile("amoswap.w.aq %[old], %[new], (%[state])"
                         : [old] "=r" (old)
                         : [new] "r" (new), [state] "r" (&(lock->_state))
                         : "memory");
    }

    return 0;
#else
    /* Store the memory address in mtval like a normal store/amo access fault */
    __asm__ ("csrw mtval, %[state]"
             :: [state] "r" (&(lock->_state)));

    /* Trigger a Store/AMO access fault */
    _metal_trap(METAL_STORE_AMO_ACCESS_FAULT);

    /* If execution returns, indicate failure */
    return 1;
#endif
}

inline int metal_lock_give(struct metal_lock *lock) {
#ifdef __riscv_atomic
    __asm__ volatile("amoswap.w.rl x0, x0, (%[state])"
                     :: [state] "r" (&(lock->_state))
                     : "memory");

    return 0;
#else
    /* Store the memory address in mtval like a normal store/amo access fault */
    __asm__ ("csrw mtval, %[state]"
             :: [state] "r" (&(lock->_state)));

    /* Trigger a Store/AMO access fault */
    _metal_trap(METAL_STORE_AMO_ACCESS_FAULT);

    /* If execution returns, indicate failure */
    return 1;
#endif
}
