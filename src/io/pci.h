#ifndef PCI_H
#define PCI_H

#include <stdint.h>
#include <io/io.h>

#define PCI_CA_ADDR 0xCF8
#define PCI_CA_ENABLE 0x80000000

static inline uint16_t read_vendor_id() {
    // 2nd device
    uint32_t ca_addr = 0x2 << 11 | PCI_CA_ENABLE;
    out16(PCI_CA_ADDR, ca_addr);

    //@todo
    return 0x00;
}
#endif
