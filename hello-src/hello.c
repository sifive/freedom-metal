#include <stdio.h>
#include <metal/cpu.h>

void metal_riscv_cpu_intc_msip_handler() {
    printf("Hello from the software interrupt handler!\n");

    struct metal_cpu cpu = metal_cpu_get(metal_cpu_get_current_hartid());
    metal_cpu_clear_ipi(cpu);
}

void metal_exception_ecall_from_m_mode_handler(struct metal_cpu cpu, int ecode) {
    printf("Caught ecall\n");

    /* Step over the ecall */
    uintptr_t epc = metal_cpu_get_exception_pc(cpu);
    size_t len = metal_cpu_get_instruction_length(cpu, epc);
    metal_cpu_set_exception_pc(cpu, epc + len);
}

int main(void) {
    printf("Hello, world!\n");

    struct metal_cpu cpu = metal_cpu_get(metal_cpu_get_current_hartid());
    metal_cpu_enable_interrupts(cpu);
    metal_cpu_enable_ipi(cpu);
    metal_cpu_set_ipi(cpu);

    __asm__ ("ecall");

    return 0;
}
