#ifndef PCI_H
#define PCI_H

#include <stdint.h>
#include <io/io.h>

#define PCI_CONFIG_ADDRESS 0xCF8
#define PCI_CONFIG_DATA 0xCFC
#define PCI_CA_ENABLE 0x80000000

static inline uint16_t read_vendor_id() {
    // 2nd device
    uint32_t ca_addr = 0x2 << 11 | PCI_CA_ENABLE;
    out16(PCI_CONFIG_ADDRESS, ca_addr);

    uint16_t data = in16(PCI_CONFIG_DATA);
    return data;
}

#endif
