#include <stdint.h>
#include <assert.h>
#include <metal/cpu.h>

#include "encoding.h"

void pop_tf(trapframe_t*);

typedef unsigned long pte_t;


# define l1pt page_tables[0]
#if SATP_MODE_CHOICE == SATP_MODE_SV48
# define NPT 2
# define l2pt page_tables[1]
#elif SATP_MODE_CHOICE == SATP_MODE_SV39
# define NPT 1
# define l1pt page_tables[0]
//# define l2pt page_tables[1]
//# define l3pt page_tables[2]
#else
# error Not support SATP_MODE_CHOICE
#endif

pte_t page_tables[NPT][PTES_PER_PT] __attribute__((aligned(PGSIZE)));

#define VM_MAPPING_END	(8 * GIGAPAGE_SIZE) // Mapping 8GB of memory space
void vm_boot(uintptr_t test_addr)
{
	pte_t attrs = (PTE_V | PTE_R | PTE_W | PTE_X | PTE_A | PTE_D | PTE_U);
	unsigned long base = 0UL;

#if SATP_MODE_CHOICE == SATP_MODE_SV48
	page_tables[0][0] = ((uintptr_t)l2pt >> PGSHIFT << PTE_PPN_SHIFT) | PTE_V; 
	while (base < VM_MAPPING_END) {
		page_tables[1][base >> 30] = ((pte_t)base >> PGSHIFT << PTE_PPN_SHIFT) | attrs;
		base += GIGAPAGE_SIZE;
	}
#elif SATP_MODE_CHOICE == SATP_MODE_SV39
	while (base < VM_MAPPING_END) {
		page_tables[0][base >> 30] = ((pte_t)base >> PGSHIFT << PTE_PPN_SHIFT) | attrs;
		base += GIGAPAGE_SIZE;
	}
#else
# error Not support SATP_MODE_CHOICE
#endif
	uintptr_t vm_choice = SATP_MODE_CHOICE;
	uintptr_t satp_value = ((uintptr_t)l1pt >> PGSHIFT)
						 | (vm_choice * (SATP_MODE & ~(SATP_MODE<<1)));

	flush_pages();
	write_csr(satp, satp_value);
	if (read_csr(satp) != satp_value)
		assert(!"unsupportedsatp mode");

	// Set up PMPs if present, ignoring illegal instruction trap if not.
	uintptr_t pmpc = PMP_NAPOT | PMP_R | PMP_W | PMP_X;
	uintptr_t pmpa = ((uintptr_t)1 << (__riscv_xlen == 32 ? 31 : 53)) - 1;
	asm volatile ("la t0, 1f\n\t"
	              "csrrw t0, mtvec, t0\n\t"
	              "csrw pmpaddr0, %1\n\t"
	              "csrw pmpcfg0, %0\n\t"
	              ".align 2\n\t"
	              "1: csrw mtvec, t0"
	              : : "r" (pmpc), "r" (pmpa) : "t0");

	/* initialize interrupt controller before entring S-mode */
	struct metal_cpu *cpu = metal_cpu_get(0);
	struct metal_interrupt *cpu_int = metal_cpu_interrupt_controller(cpu);
	__metal_driver_riscv_cpu_controller_interrupt_init(cpu_int);

	trapframe_t tf;
	memset(&tf, 0, sizeof(tf));
	tf.epc = test_addr;
	// Prepare S-mode sp
	tf.gpr[2] = read_csr(mscratch);
	// S-mode inherit gp from M-mode
	uintptr_t gp;
	asm volatile("mv %0, gp" : "=r"(gp));
	tf.gpr[3] = gp;
	pop_tf(&tf);
}
