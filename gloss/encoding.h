#ifndef RISCV_CSR_ENCODING_H
#define RISCV_CSR_ENCODING_H

#define Sv48


#define SATP64_MODE 0xF000000000000000
#define SATP64_ASID 0x0FFFF00000000000
#define SATP64_PPN  0x00000FFFFFFFFFFF

#define SATP_MODE_OFF  0
#define SATP_MODE_SV32 1
#define SATP_MODE_SV39 8
#define SATP_MODE_SV48 9

#define SATP_MODE SATP64_MODE

#if defined(Sv48)
# define SATP_MODE_CHOICE SATP_MODE_SV48
#else
# define SATP_MODE_CHOICE SATP_MODE_SV39
#endif

#define PTE_PPN_SHIFT 10 /* The lower 10 bits are used to record memory atttributes */

/* page table entry (PTE) fields */
#define PTE_V		0x001 /* Valid */
#define PTE_R		0x002 /* Read */
#define PTE_W		0x004 /* Write */
#define PTE_X		0x008 /* Execute */
#define PTE_U		0x010 /* User */
#define PTE_G 		0x020 /* Global */
#define PTE_A		0x040 /* Accessed */
#define PTE_D		0x080 /* Dirty */
#define PTE_SOFT	0x300 /* Reserved for Software */

#define PTE_TABLE(PTE)	(((PTE) & (PTE_V | PTE_R | PTE_W | PTE_X)) == PTE_V)


#define PGSHIFT 12
#define PGSIZE (1UL << PGSHIFT)
#define PTES_PER_PT (1UL << 9)
#define MEGAPAGE_SIZE (PTES_PER_PT * PGSIZE)
#define GIGAPAGE_SIZE (PTES_PER_PT * MEGAPAGE_SIZE)

typedef struct
{
  long gpr[32];
  long sr;
  long epc;
  long badvaddr;
  long cause;
} trapframe_t;

#define read_csr(reg) ({ unsigned long __tmp; \
    asm volatile ("csrr %0, " #reg : "=r"(__tmp)); \
    __tmp; })

#define write_csr(reg, val) ({ \
    asm volatile ("csrw " #reg ", %0" :: "rK"(val)); })

#define flush_pages() asm volatile ("sfence.vma x0, x0 ")



#define PMP_R     0x01
#define PMP_W     0x02
#define PMP_X     0x04
#define PMP_A     0x18
#define PMP_L     0x80
#define PMP_SHIFT 2

#define PMP_TOR   0x08
#define PMP_NA4   0x10
#define PMP_NAPOT 0x18

#endif
