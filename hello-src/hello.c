#include <stdio.h>
#include <metal/cpu.h>

void metal_riscv_cpu_intc_msip_handler() {
    printf("Hello from the software interrupt handler!\n");

    struct metal_cpu cpu = metal_cpu_get(metal_cpu_get_current_hartid());
    metal_cpu_clear_ipi(cpu);
}

int main(void) {
    printf("Hello, world!\n");

    struct metal_cpu cpu = metal_cpu_get(metal_cpu_get_current_hartid());
    metal_cpu_enable_interrupts(cpu);
    metal_cpu_enable_ipi(cpu);
    metal_cpu_set_ipi(cpu);

    return 0;
}
