#ifndef SERIAL_H
#define SERIAL_H

#define COM1 0x3F8
#define COM2 0x2F8
#define COM3 0x3E8
#define COM4 0x2E8

#include <stdint.h>

int init_serial(uint16_t port);
void write_serial(uint16_t port, uint8_t data);
uint8_t read_serial(uint16_t port);

#endif
