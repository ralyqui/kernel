/*
 * Intel® 82540EM polling network driver
 */

#include <core/fmt.h>
#include <stdint.h>

#define I82540EM_VENDOR_ID 0x8086
#define I82540EM_DEVICE_ID 0x100e
#define I82540EM_BAR0_OFFSET 0x10

typedef struct {
    uint64_t mmio_base_;
    uint32_t bus;
    uint32_t slot;
} nic_info_t;

void nic_init(uint32_t bus, uint32_t slot);
