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
    uint16_t temp = read_vendor_id();
    init_serial(COM1);

    write_serial_hex(COM1, temp);
}

void kmain(void) {
    boot();

    test_pci();
    // test_serial();
    for(;;){
        cpu_halt();
    }
}
