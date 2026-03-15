#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H
#include <limine.h>

struct fb_draw_request {
    uint64_t start_x;
    uint64_t start_y;
    uint64_t end_x;
    uint64_t end_y;
};

void draw();

#endif
