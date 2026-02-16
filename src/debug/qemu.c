#include "debug/qemu.h"
#include "io/io.h"

static inline void char_out(char c) {
    out8(QEMU_DBG_IO, c);
}

void qemu_debug(const char* s) {
    if(!s) return;

    while(*s != '\0') {
        char_out(*s);
        s++;
    }
}
