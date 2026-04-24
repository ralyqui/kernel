#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H
#include <limine.h>

volatile extern uint32_t *fb_ptr;
extern uint64_t fb_pitch;
extern uint64_t fb_width;

void fb_setup();

#endif
