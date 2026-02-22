#include "serial.h"
#include "core/cpu.h"
#include "io.h"
#include <stdint.h>

#define RT_OFF 0
#define BRL_OFF 0
#define BRH_OFF 1
#define EIR_OFF 1
#define FIFO_OFF 2
#define LCR_OFF 3
#define MCR_OFF 4
#define LSR_OFF 5

#define LSR_TRANSMIT_EMPTY 0x20
#define LSR_DATA_READY 0x01

int init_serial(uint16_t port) {
    out8(port + LCR_OFF, 0x80);

    // baud rate divisor 2
    out8(port + BRL_OFF, 0x02);
    out8(port + BRH_OFF, 0x00);
    out8(port + LCR_OFF, 0x03); // 1 stop bit

    out8(port + EIR_OFF, 0x00);
    out8(port + FIFO_OFF, 0x47);
    out8(port + MCR_OFF, 0x03);
    return 1;
}

static inline int is_empty_transit(uint16_t port) {
    return in8(port + LSR_OFF) & LSR_TRANSMIT_EMPTY;
}

void write_serial(uint16_t port, uint8_t data) {
    while (!is_empty_transit(port)) {
        cpu_pause();
    }

    out8(port, data);
}

void write_serial_s(uint16_t port, char *s) {
    if (!s)
        return;

    while (*s) {
        write_serial(port, *s);
        s++;
    }
}

void write_serial_hex(uint16_t port, uint32_t data) {
    write_serial(port, '0');
    write_serial(port, 'x');

    for (int i = 28; i >= 0; i -= 4) {
        uint8_t digit = data >> i & 0xf;

        write_serial(port, digit >= 10 ? 'a' + digit - 10 : '0' + digit);
    }

    write_serial(port, ' ');
}

void write_serial_l(uint16_t port, uint64_t data) {
    write_serial(port, '0');
    write_serial(port, 'x');

    for (int i = 60; i >= 0; i -= 4) {
        uint8_t digit = data >> i & 0xf;

        write_serial(port, digit >= 10 ? 'a' + digit - 10 : '0' + digit);
    }

    write_serial(port, ' ');
}

static inline int data_ready(uint16_t port) {
    return in8(port + LSR_OFF) & LSR_DATA_READY;
}

uint8_t read_serial(uint16_t port) {
    while (!data_ready(port)) {
        cpu_pause();
    }

    return in8(port);
}
