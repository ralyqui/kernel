#ifndef CPU_H
#define CPU_H

static inline void cpu_pause() {
    __asm__ volatile("pause":::"memory");
}

static inline void cpu_halt() {
    __asm__ volatile("hlt");
}

static inline void cpu_nop() {
    __asm__ volatile("nop");
}

#endif
