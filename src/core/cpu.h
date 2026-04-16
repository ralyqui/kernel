#ifndef CPU_H
#define CPU_H

#include <stdint.h>

static inline void cpu_pause() { __asm__ volatile("pause" ::: "memory"); }

static inline void cpu_halt() { __asm__ volatile("hlt"); }

static inline void cpu_nop() { __asm__ volatile("nop"); }

static inline uint64_t get_rsp(void) {
    register uint64_t rsp asm("rsp");
    return rsp;
}

static inline uint64_t get_rip(void) {
    uint64_t rip;
    asm("lea %0, [rip]" : "=r"(rip));
    return rip;
}

#endif
