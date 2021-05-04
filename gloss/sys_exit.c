#include <metal/shutdown.h>
#if defined(_USE_SEMIHOST_)
#include <metal/semihosting.h>
#endif

void _exit(int exit_status) {
#if defined(_USE_SEMIHOST_)
    uintptr_t value;

#if __riscv_xlen == 32
    if (exit_status == 0)
        value = (uintptr_t)((2 << 16) + 38);
    else
        value = (uintptr_t)((2 << 16) + 35);
    (void)semihost_call_host(SEMIHOST_SYS_EXIT, value);
#elif __riscv_xlen == 64
    volatile semihostparam_t arg = {.param1 = (uintptr_t)((2 << 16) + 38),
                                    .param2 = (uintptr_t)exit_status};
    (void)semihost_call_host(SEMIHOST_SYS_EXIT, (uintptr_t)&arg);
#endif
#endif
    metal_shutdown(exit_status);
    while (1)
        ;
}
