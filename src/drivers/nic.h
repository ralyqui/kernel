/*
 * Intel® 82540EM polling network driver
 */

#include <core/fmt.h>
#include <stdint.h>

typedef struct {
    uint64_t mmio_base_;
    uint32_t bus;
    uint32_t slot;
} nic_info_t;

void nic_init(uint32_t bus, uint32_t slot);
