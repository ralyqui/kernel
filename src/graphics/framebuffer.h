#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H
#include <limine.h>

typedef struct {
    volatile uint32_t *ptr;
    uint64_t pitch;
    uint64_t width;
    uint64_t height;
} _fb_global_info;

extern _fb_global_info *fb_global_info;

void fb_setup();

#endif
