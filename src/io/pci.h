#ifndef PCI_H
#define PCI_H

#include <stdint.h>
#include <stdbool.h>
#include <io/io.h>

#define PCI_CONFIG_ADDRESS 0xCF8
#define PCI_CONFIG_DATA 0xCFC
#define PCI_CA_ENABLE 0x80000000

#define BASIC_FUNC 0x0

#define VENDOR_ID_OFFSET 0x0
#define DEVICE_ID_OFFSET 0x2
#define HEADER_TYPE_OFFSET 0xE

static inline uint16_t pci_read16(uint8_t bus, uint8_t device, uint8_t function, uint8_t reg_offset) {
    uint32_t ca_addr = PCI_CA_ENABLE | (bus << 16) | (device << 11) | (function << 8) | (reg_offset & 0xfc);
    out32(PCI_CONFIG_ADDRESS, ca_addr);

    uint32_t config_data = in32(PCI_CONFIG_DATA);
    uint8_t inner_offset = (reg_offset & 2) * 8;

    return (uint16_t)((config_data >> inner_offset) & 0xffff);
}

static inline uint8_t pci_read8(uint8_t bus, uint8_t device, uint8_t function, uint8_t reg_offset) {
    uint32_t ca_addr = PCI_CA_ENABLE | (bus << 16) | (device << 11) | (function << 8) | (reg_offset & 0xfc);
    out32(PCI_CONFIG_ADDRESS, ca_addr);

    uint32_t config_data = in32(PCI_CONFIG_DATA);
    uint8_t inner_offset = (reg_offset & 2) * 8;

    return (uint8_t)((config_data >> inner_offset) & 0xff);
}

static inline uint8_t read_header_type(uint8_t bus, uint8_t device) {
    return pci_read8(bus, device, BASIC_FUNC, HEADER_TYPE_OFFSET);
}

static inline uint16_t read_vendor_id(uint8_t bus, uint8_t device, uint8_t function) {
    return pci_read16(bus, device, function, VENDOR_ID_OFFSET);
}

static inline uint16_t read_device_id(uint8_t bus, uint8_t device, uint8_t function) {
    return pci_read16(bus, device, function, DEVICE_ID_OFFSET);
}

static inline bool is_multifunc(uint8_t device, uint8_t bus) {
    return read_header_type(bus, device) & 0x80;
}

#endif
