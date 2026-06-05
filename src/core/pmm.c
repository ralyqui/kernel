#include "pmm.h"
#include <stddef.h>

// just in case limine gives garbage or I mess up, start of phys ram is messy
#define LOWEST_USABLE_ADDR 0x1000
#define MAX_MEM_BLOCKS 1024

extern volatile uint64_t lm_hhdm_offset;

static mem_block mem_pool[MAX_MEM_BLOCKS];
static uint64_t pool_size;

volatile static mem_block *first;
volatile static mem_block *last;

__attribute((
    used,
    section(".limine_requests"))) static volatile struct limine_memmap_request
    memmap_reqeust = {.id = LIMINE_MEMMAP_REQUEST_ID, .revision = 4};

static bool add_block(uint64_t base, uint64_t size) {
    if (pool_size >= MAX_MEM_BLOCKS || base < LOWEST_USABLE_ADDR) {
        return false;
    }

    mem_pool[pool_size].base = base;
    mem_pool[pool_size].size = size;
    mem_pool[pool_size].next = NULL;

    if (first == NULL) {
        first = &mem_pool[pool_size];
    }

    last = &mem_pool[pool_size];
    pool_size++;

    return true;
}

void pmm_init() {
    struct limine_memmap_response *memmap = memmap_reqeust.response;

    for (uint64_t i = 0; i < memmap->entry_count; i++) {
        struct limine_memmap_entry *entry = memmap->entries[i];
        if (entry->type != LIMINE_MEMMAP_USABLE ||
            entry->base < LOWEST_USABLE_ADDR)
            continue;

        add_block(entry->base, entry->length);
    }
}

uint64_t pmm_alloc(uint64_t size) {
    for (uint64_t i = 0; i < pool_size; i++) {
        if (mem_pool[i].size >= size) {
            uint64_t resp_base = mem_pool[i].base;
            mem_pool[i].base += size;
            mem_pool[i].size -= size;
            return resp_base;
        }
    }

    print_f("Out of memory! Requested %l bytes", size);
    return 0;
}

uint64_t pmm_alloc_128(uint64_t size) {
    for (uint64_t i = 0; i < pool_size; i++) {
        uint64_t raw_base = mem_pool[i].base;
        uint64_t aligned_base = (raw_base + 127) & ~127;
        uint64_t padding = aligned_base - raw_base;

        if (mem_pool[i].size >= (padding + size)) {
            mem_pool[i].base = aligned_base + size;
            mem_pool[i].size -= (padding + size);
            return aligned_base;
        }
    }
    return 0;
}

void *phys_to_virt(uint64_t phys_addr) {
    return (uint64_t *)(phys_addr + lm_hhdm_offset);
}
