#ifndef PAGING_H
#define PAGING_H

#include "core/pmm.h"
#include <core/fmt.h>
#include <limine.h>
#include <stdint.h>

#define PML4_IDX(addr) (((addr) >> 39) & 0x1ff)
#define PDPT_IDX(addr) (((addr) >> 30) & 0x1ff)
#define PD_IDX(addr) (((addr) >> 21) & 0x1ff)
#define PT_IDX(addr) (((addr) >> 12) & 0x1ff)

#define PMTE 512
#define PTE_MAX (1 << 12)
#define PDE_MAX (1 << 21)
#define PDPT_MAX (1 << 30)

#define HHDM_PDPT_SIZE 4

extern volatile uint64_t lm_hhdm_offset;
extern volatile uint64_t lm_kernel_phys;
extern volatile uint64_t lm_kernel_virt;

typedef union {
    struct __attribute((packed)) {
        uint64_t present : 1;
        uint64_t rw_allow : 1;
        uint64_t user_mode : 1;
        uint64_t page_write_through : 1;
        uint64_t pl_cache : 1;
        uint64_t accessed : 1;
        uint64_t _ignored1 : 1;
        uint64_t _reserved1 : 1;
        uint64_t _ignored2 : 3;
        uint64_t hlat_restart : 1;
        uint64_t pdpt_addr : 40;
        uint64_t _ignored3 : 11;
        uint64_t execute_disable : 1;
    };
    uint64_t raw;
} _pml4e;

typedef union {
    struct __attribute((packed)) {
        uint64_t present : 1;
        uint64_t rw_allow : 1;
        uint64_t user_mode : 1;
        uint64_t page_write_through : 1;
        uint64_t pl_cache : 1;
        uint64_t accessed : 1;
        uint64_t dirty : 1;
        uint64_t page_size : 1;
        uint64_t global : 1;
        uint64_t _ignored2 : 2;
        uint64_t hlat_restart : 1;
        uint64_t memory_type : 1;
        uint64_t reserved : 17;
        uint64_t physical_address : 22;
        uint64_t _ignored3 : 7;
        uint64_t protection_key : 4;
        uint64_t execute_disable : 1;
    };
    uint64_t raw;
} _pdpte;

typedef union {
    struct __attribute((packed)) {
        uint64_t present : 1;
        uint64_t rw_allow : 1;
        uint64_t user_mode : 1;
        uint64_t page_write_through : 1;
        uint64_t pl_cache : 1;
        uint64_t accessed : 1;
        uint64_t dirty : 1;
        uint64_t page_size : 1;
        uint64_t global : 1;
        uint64_t _ignored2 : 2;
        uint64_t hlat_restart : 1;
        uint64_t memory_type : 1;
        uint64_t reserved : 17;
        uint64_t physical_address : 22;
        uint64_t _ignored3 : 7;
        uint64_t protection_key : 4;
        uint64_t execute_disable : 1;
    };
    uint64_t raw;
} _pde;

typedef union {
    struct __attribute((packed)) {
        uint64_t present : 1;
        uint64_t rw_allow : 1;
        uint64_t user_mode : 1;
        uint64_t page_write_through : 1;
        uint64_t pl_cache : 1;
        uint64_t accessed : 1;
        uint64_t dirty : 1;
        uint64_t pat : 1;
        uint64_t global : 1;
        uint64_t _ignored2 : 2;
        uint64_t hlat_restart : 1;
        uint64_t memory_type : 1;
        uint64_t reserved : 17;
        uint64_t physical_address : 22;
        uint64_t _ignored3 : 7;
        uint64_t protection_key : 4;
        uint64_t execute_disable : 1;
    };
    uint64_t raw;
} _pte;

static inline void zero_init_page(volatile void *p) {
    uint64_t *page = (uint64_t *)p;
    unsigned int i;

    for (i = 0; i < PMTE; i++) {
        *(page + i) = 0;
    }
}

static inline void _kernel_init_paging(uint64_t kernel_phys_addr,
                                       uint64_t kernel_virt_addr,
                                       volatile _pml4e *pml4) {
    uint64_t pdpt_phys_addr = pmm_alloc(PMTE * sizeof(_pdpte));
    volatile _pdpte *master_pdpt = (_pdpte *)(pdpt_phys_addr + lm_hhdm_offset);
    zero_init_page(master_pdpt);

    uint64_t pd_phys_addr = pmm_alloc(PMTE * sizeof(_pde));
    volatile _pde *kernel_pde = (_pde *)(pd_phys_addr + lm_hhdm_offset);
    zero_init_page(kernel_pde);
    master_pdpt[0].present = 1;
    master_pdpt[0].rw_allow = 1;
    master_pdpt[0].physical_address = pd_phys_addr >> 12;

    unsigned int j, k;
    for (j = 0; j < PMTE; j++) {
        uint64_t pt_phys_addr = pmm_alloc(PMTE * sizeof(_pte));
        volatile _pte *kernel_pte = (_pte *)(pt_phys_addr + lm_hhdm_offset);
        kernel_pde[j].present = 1;
        kernel_pde[j].rw_allow = 1;

        kernel_pde[j].physical_address = pt_phys_addr >> 12;
        zero_init_page(kernel_pte);
        for (k = 0; k < PMTE; k++) {
            kernel_pte[k].present = 1;
            kernel_pte[k].rw_allow = 1;
            kernel_pte[k].physical_address =
                kernel_phys_addr + PDE_MAX * j + PTE_MAX * k;
        }
    }

    pml4[PML4_IDX(kernel_virt_addr)].present = 1;
    pml4[PML4_IDX(kernel_virt_addr)].rw_allow = 1;
    pml4[PML4_IDX(kernel_virt_addr)].pdpt_addr = pdpt_phys_addr >> 12;
}

static inline void _hhdm_init_paging(volatile _pml4e *pml4, uint64_t virt_start,
                                     uint64_t phys_start) {
    uint64_t pdpt_phys_addr = pmm_alloc(PMTE * sizeof(_pdpte));
    volatile _pdpte *hhdm_pdpt = (_pdpte *)(pdpt_phys_addr + lm_hhdm_offset);
    zero_init_page(hhdm_pdpt);

    unsigned int i;

    for (i = 0; i < HHDM_PDPT_SIZE; i++) {
        hhdm_pdpt[i].present = 1;
        hhdm_pdpt[i].rw_allow = 1;
        hhdm_pdpt[i].page_size = 1;
        hhdm_pdpt[i].physical_address = (phys_start + PDPT_MAX * i) >> 12;

        print_f("Page %l points to %l\n", i, (phys_start + PDPT_MAX * i));
    }

    pml4[PML4_IDX(virt_start)].present = 1;
    pml4[PML4_IDX(virt_start)].rw_allow = 1;
    pml4[PML4_IDX(virt_start)].pdpt_addr = pdpt_phys_addr >> 12;
}

static inline void pml4_init() {
    uint64_t pml4_phys_addr = pmm_alloc(PMTE * sizeof(_pml4e));
    volatile _pml4e *pml4 = (_pml4e *)(pml4_phys_addr + lm_hhdm_offset);
    zero_init_page(pml4);

    _kernel_init_paging(lm_kernel_phys, lm_kernel_virt, pml4);
    _hhdm_init_paging(pml4, lm_hhdm_offset, 0x0);

    __asm__ volatile("mov cr3, %0" : : "r"(pml4_phys_addr) : "memory");
}

#endif
