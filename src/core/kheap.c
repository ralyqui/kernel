#include <core/kheap.h>
#include <core/pmm.h>

extern volatile uint64_t lm_hhdm_offset;

__attribute__((malloc)) void *kmalloc(uint64_t size) {
    uint64_t phys_block = pmm_alloc(size);

    if (phys_block == 0) {
        return 0;
    }

    return (void *)(phys_block + lm_hhdm_offset);
}
