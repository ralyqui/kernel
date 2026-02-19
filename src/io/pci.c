#include "pci.h"

bool is_multifunc(pci_device_info *info) { return info->header_type & 0x80; }

bool pci_read_slot(uint8_t bus, uint8_t slot, uint8_t func,
                   pci_device_info *info) {
    uint16_t vendor_id = read_vendor_id(bus, slot, func);

    if (vendor_id == 0xffff)
        return false;

    info->vendor_id = vendor_id;
    info->device_id = read_device_id(bus, slot, func);
    info->header_type = read_header_type(bus, slot);

    info->function = func;
    info->slot = slot;
    info->bus = bus;

    return true;
}
