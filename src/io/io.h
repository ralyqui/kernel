#ifndef IO_H
#define IO_H

#include <stdint.h>

static inline void out8(uint16_t port, uint8_t data) {
    asm volatile("out %1, %0" : : "a"(data), "d"(port));
}

static inline uint8_t in8(uint16_t port) {
    uint8_t data;
    asm volatile("in %0, %1": "=a"(data): "d"(port));
    return data;
}

static inline void out16(uint16_t port, uint16_t data) {
    asm volatile("out %1, %0" : : "a"(data), "d"(port));
}

static inline uint16_t in16(uint16_t port) {
    uint16_t data;
    asm volatile("in %0, %1": "=a"(data): "d"(port));
    return data;
}

static inline void out32(uint16_t port, uint32_t data) {
    asm volatile("out %1, %0" : : "a"(data), "d"(port));
}

static inline uint32_t in32(uint32_t port) {
    uint32_t data;
    asm volatile("in %0, %1": "=a"(data): "d"(port));
    return data;
}

#endif
