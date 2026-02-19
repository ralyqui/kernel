#include "boot/boot.h"
#include "core/cpu.h"
#include "core/fmt.h"
#include "io/pci.h"
#include "io/serial.h"

void test_pci() {
    init_serial(COM1);

    pci_device_info cur_info;

    for (int bus = 0; bus < 256; bus++) {
        for (int device = 0; device < 32; device++) {
            if (!pci_read_slot(bus, device, BASIC_FUNC, &cur_info))
                continue;

            print_f("Vendor id: %x\n", cur_info.vendor_id);
            print_f("Device id: %x\n", cur_info.device_id);
            print_f("Header type: %x\n", cur_info.header_type);
            // write_serial_hex(COM1, cur_info.vendor_id);
            // write_serial_hex(COM1, cur_info.device_id);
            // write_serial_hex(COM1, cur_info.header_type);
            // write_serial_s(COM1, "\n");
        }
    }
}

void kmain(void) {
    boot();

    test_pci();

    for (;;) {
        cpu_halt();
    }
}
