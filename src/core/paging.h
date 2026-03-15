#ifndef PAGING_H
#define PAGING_H

#include <core/fmt.h>
#include <limine.h>
#include <stdint.h>

#define PML4_IDX(addr) (((addr) >> 39) & 0x1ff)
#define PDPT_IDX(addr) (((addr) >> 30) & 0x1ff)

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

volatile static _pml4e pml4[512] __attribute__((aligned(4096)));
volatile static _pdpte pdpt[512] __attribute__((aligned(4096)));
volatile static uint64_t page_dir[512] __attribute__((aligned(4096)));

void pml4_init() {
    unsigned int i;

    for (i = 0; i < 512; i++) {
        pdpt[i].present = 1;
    }

    for (i = 0; i < 512; i++) {
        pml4[i] = (_pml4e){0};
    }

    pml4[0].present = 0x1;
    pml4[0].rw_allow = 0x1;
    pml4[0].pdpt_addr = (uint64_t)(&pdpt) - lm_kernel_virt + lm_kernel_phys;
    print_f("global addr pdpt: %l\n", (uint64_t)&pdpt);
    print_f("int addr: %l\n", (uint64_t)&i);
    print_f("hhdm offset: %l\n", lm_hhdm_offset);
    print_f("kernel phys: %l\n", lm_kernel_phys);
    print_f("kernel virt: %l\n", lm_kernel_virt);

    print_f("pdpt 0 phys addr: %l", pml4[0].pdpt_addr);
}

volatile static uint64_t page_directory[512] __attribute__((aligned(4096)));

static inline void setup_paging() {
    unsigned int i;
    for (i = 0; i < 1024; i++) {
        page_directory[i] = 0x00000002;
    }

    uint64_t first_page_table[1024] __attribute__((aligned(4096)));

    for (i = 0; i < 512; i++) {
        first_page_table[i] = (i * 0x1000) | 3;
    }

    page_directory[0] = ((uint64_t)first_page_table) | 3;
};

#endif
