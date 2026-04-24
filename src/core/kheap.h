#ifndef KHEAP_H
#define KHEAP_H

#include <stdint.h>

__attribute__((malloc)) void *kmalloc(uint64_t size);

#endif
