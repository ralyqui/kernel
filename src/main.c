#include "io/serial.h"
#include "boot/boot.h"
#include "core/cpu.h"
#include "io/pci.h"

void test_serial() {
    init_serial(COM1);
    // for(;;){
    //     uint8_t c = read_serial(COM1);
    //     write_serial(COM1, c);
    // }
    write_serial_s(COM1, "test");
}

void test_pci() {
    init_serial(COM1);

    uint8_t bus = 0, device = 2;
    write_serial_hex(COM1, read_device_id(bus, device));
    write_serial_hex(COM1, read_vendor_id(bus, device));
}

void kmain(void) {
    boot();

    test_pci();
    // test_serial();
    for(;;){
        cpu_halt();
    }
}
