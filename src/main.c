#include <stdint.h>
#include "io/serial.h"
#include "boot/boot.h"
#include "core/cpu.h"

void test_serial() {
    init_serial(COM1);
    for(;;){
        uint8_t c = read_serial(COM1);
        write_serial(COM1, c);
    }
}

void kmain(void) {
    boot();

    // test_serial();
    for(;;){
        cpu_halt();
    }
}
