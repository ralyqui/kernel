#include "io/serial.h"
#include <stdarg.h>
#include <stdint.h>

void print_hex(uint32_t i) { write_serial_hex(COM1, i); }

void print_s(char *s) { write_serial_s(COM1, s); }

void print_c(char c) { write_serial(COM1, c); }

void print_f(const char *str, ...) {
    va_list args;
    va_start(args, str);

    while (*str) {
        if (*str == '%') {
            str++;
            switch (*str) {
            case 'x':
                print_hex(va_arg(args, uint32_t));
                break;
            case 's':
                print_s(va_arg(args, char *));
                break;
            }
        } else {
            print_c(*str);
        }
        str++;
    }
}
