#include "kshell.h"
#include "font.h"
#include "framebuffer.h"

#define BG_COLOR 0x24283b

static void fill_background() {
    volatile uint32_t *fb_ptr = fb_global_info->ptr;

    for (uint64_t y = 0; y < fb_global_info->height; y++) {
        for (uint64_t x = 0; x < fb_global_info->width; x++) {
            fb_ptr[y * (fb_global_info->pitch / 4) + x] = BG_COLOR;
        }
    }
}

static void draw_term_frame() { fill_background(); }

void kshell_init() { draw_term_frame(); }

void kprint(char *s) { kputs(s, 20, 20); }
