#ifndef PMM_H
#define PMM_H

#include <core/fmt.h>
#include <limine.h>
#include <stdbool.h>

typedef struct mem_block {
    uint64_t base;
    uint64_t size;
    struct mem_block *next;
} mem_block;

uint64_t pmm_alloc(uint64_t size);
void pmm_init();

#endif
