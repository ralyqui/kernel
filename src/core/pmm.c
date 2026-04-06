#include "pmm.h"
#include <stddef.h>

// just in case limine gives garbage or I mess up, start of phys ram is messy
#define LOWEST_USABLE_ADDR 0x1000
#define MAX_MEM_BLOCKS 1024

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

    print_f("mem pool entries: %l\n", memmap->entry_count);

    for (uint64_t i = 0; i < memmap->entry_count; i++) {
        struct limine_memmap_entry *entry = memmap->entries[i];
        if (entry->type != LIMINE_MEMMAP_USABLE ||
            entry->base < LOWEST_USABLE_ADDR)
            continue;

        add_block(entry->base, entry->length);
    }

    print_f("First mem block is %l with length %l\n", first->base, first->size);
    print_f("Last mem block is %l with length %l\n", last->base, last->size);
    print_l(pool_size);
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

    print_s("Out of memory!");
    return 0;
}
