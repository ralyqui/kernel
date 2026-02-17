#ifndef PCI_H
#define PCI_H

#include <stdint.h>
#include <io/io.h>

#define PCI_CONFIG_ADDRESS 0xCF8
#define PCI_CONFIG_DATA 0xCFC
#define PCI_CA_ENABLE 0x80000000

#define VENDOR_ID_OFFSET 0x0
#define DEVICE_ID_OFFSET 0x2

static inline uint16_t pci_read16(uint8_t bus, uint8_t device, uint8_t function, uint8_t reg_offset){
    uint32_t ca_addr = PCI_CA_ENABLE | (bus << 16) | (device << 11) | (function << 8) | (reg_offset & 0xfc);
    out32(PCI_CONFIG_ADDRESS, ca_addr);

    uint32_t config_data = in32(PCI_CONFIG_DATA);
    uint8_t inner_offset = (reg_offset & 2) * 8;

    return (uint32_t)((config_data >> inner_offset) & 0xffff);
}

static inline uint16_t read_vendor_id(uint8_t bus, uint8_t device) {
    return pci_read16(bus, device, 0, VENDOR_ID_OFFSET);
}

static inline uint16_t read_device_id(uint8_t bus, uint8_t device) {
    return pci_read16(bus, device, 0, DEVICE_ID_OFFSET);
}

#endif
