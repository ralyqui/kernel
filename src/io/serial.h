#ifndef SERIAL_H
#define SERIAL_H

#define COM1 0x3F8
#define COM2 0x2F8
#define COM3 0x3E8
#define COM4 0x2E8

#include <stdint.h>

int init_serial(uint16_t port);

void write_serial(uint16_t port, uint8_t data);
void write_serial_s(uint16_t port, char *str);
void write_serial_hex(uint16_t port, uint32_t data);
void write_serial_l(uint16_t port, uint64_t data);

uint8_t read_serial(uint16_t port);

#endif
