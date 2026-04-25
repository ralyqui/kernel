#include "core/fmt.h"
#include "io/serial.h"
#include "kheap.h"
#include <stdarg.h>
#include <stdint.h>

#define FMT_MAX_STR_SIZE 256

void print_hex(uint32_t i) { write_serial_hex(COM1, i); }

void print_l(uint64_t i) { write_serial_l(COM1, i); }

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
            case 'l':
                print_l(va_arg(args, uint64_t));
                break;
            case 'c':
                print_c(va_arg(args, int));
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

static char *sappend(char *dest, char *src) {
    while (*src) {
        *(dest++) = *(src++);
    }

    return dest;
}
static char *fmt_l(uint64_t num) {
    char *res = kmalloc(20);

    res[0] = '0';
    res[1] = 'x';

    int idx = 2;

    for (int i = 60; i >= 0; i -= 4) {
        uint8_t digit = num >> i & 0xf;

        res[idx] = digit >= 10 ? 'a' + digit - 10 : '0' + digit;
        idx++;
    }

    return res;
}

char *fmt(const char *str, ...) {
    va_list args;
    va_start(args, str);

    char *res = kmalloc(FMT_MAX_STR_SIZE);

    char *res_start = res;
    char *res_last = res + FMT_MAX_STR_SIZE - 1;

    while (*str && res < res_last) {
        if (*str == '%') {
            str++;
            switch (*str) {
            case 'l': {
                char *substr = fmt_l(va_arg(args, uint64_t));
                res = sappend(res, substr);
                break;
            }
            case 's': {
                char *substr = va_arg(args, char *);
                res = sappend(res, substr);
                break;
            }
            }
        } else {
            print_s(res);
            *res = *str;
            res++;
        }
        str++;
    }

    *res = '\0';

    return res_start;
}
