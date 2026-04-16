#ifndef DEBUG_MEM_H
#define DEBUG_MEM_H

#include <core/fmt.h>
#include <stdint.h>

extern volatile uint64_t lm_hhdm_offset;

#define ENTRY_ADDR_MASK 0x000FFFFFFFFFF000ull
#define PTE_PRESENT (1ULL << 0)
#define PTE_LARGE_PAGE (1ULL << 7)
#define PML4_IDX(addr) (((addr) >> 39) & 0x1ff)
#define PDPT_IDX(addr) (((addr) >> 30) & 0x1ff)
#define PD_IDX(addr) (((addr) >> 21) & 0x1ff)
#define PT_IDX(addr) (((addr) >> 12) & 0x1ff)

static inline void debug_paging(uint64_t *pml4_base, uint64_t vaddr) {
    print_f("debug virt addr: %l\n", vaddr);
    print_f("hhdm offset: %l\n", lm_hhdm_offset);

    uint64_t pml4_idx = PML4_IDX(vaddr);
    uint64_t pml4_entry = pml4_base[pml4_idx];
    print_f("PML4 Index: %l | Entry: %l | %s\n", pml4_idx, pml4_entry,
            (pml4_entry & PTE_PRESENT) ? "PRESENT" : "NOT PRESENT");
    if (!(pml4_entry & PTE_PRESENT))
        return;

    uint64_t *pdpt_base =
        (uint64_t *)((pml4_entry & ENTRY_ADDR_MASK) + lm_hhdm_offset);
    print_f("Pdpt base is %l\n", pdpt_base);
    uint64_t pdpt_idx = PDPT_IDX(vaddr);
    uint64_t pdpt_entry = pdpt_base[pdpt_idx];
    print_f("pdpt entry is %l\n", pdpt_base[pdpt_idx]);
    print_f("PDPT Index: %l | Entry: %l | %s\n", pdpt_idx, pdpt_entry,
            (pdpt_entry & PTE_PRESENT) ? "PRESENT" : "NOT PRESENT");

    if (!(pdpt_entry & PTE_PRESENT))
        return;
    if (pdpt_entry & PTE_LARGE_PAGE) {
        print_f("1GB Huge Page\n");
        return;
    }

    print_f("pdpt_entry & ENTRY_ADDR_MASK:%l \n", pdpt_entry & ENTRY_ADDR_MASK);

    print_s("Checking page dir");
    uint64_t *pd_base =
        (uint64_t *)((pdpt_entry & ENTRY_ADDR_MASK) + lm_hhdm_offset);
    print_f("Pd base is %l\n", pd_base);
    uint64_t pd_idx = PD_IDX(vaddr);
    print_f("pd idx is %l", pd_idx);
    uint64_t pd_entry = pd_base[pd_idx];
    print_f("pd entry is %l", pd_entry);
    print_f("PD   Index: %l | Entry: %l | %s\n", pd_idx, pd_entry,
            (pd_entry & PTE_PRESENT) ? "PRESENT" : "NOT PRESENT");

    if (!(pd_entry & PTE_PRESENT))
        return;

    uint64_t *pt_base =
        (uint64_t *)((pd_entry & ENTRY_ADDR_MASK) + lm_hhdm_offset);
    uint64_t pt_idx = PT_IDX(vaddr);
    uint64_t pt_entry = pt_base[pt_idx];
    print_f("PT   Index: %l | Entry: %l | %s\n", pt_idx, pt_entry,
            (pt_entry & PTE_PRESENT) ? "PRESENT" : "NOT PRESENT");

    if (!(pt_entry & PTE_PRESENT))
        return;

    uint64_t phys_page = pt_entry & ENTRY_ADDR_MASK;
    uint64_t offset = vaddr & 0xFFFull;
    print_f("Resulting Physical Address: %l\n", phys_page + offset);
}

#endif
