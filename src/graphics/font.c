#include "font.h"
#include "framebuffer.h"
#include <stdbool.h>
#include <stdint.h>

#define PSF1_WIDTH_BITS 8
#define FONT_SPACING_WIDTH 1
#define FONT_SPACING_HEIGHT 1
#define HEADER_SIZE 4
#define TEXT_COLOR 0xc0caf5

struct font_info {
    psf1_t *header;
    char *data_start;
};

static struct font_info finf;

void kputs(char *str, uint64_t fb_x, uint64_t fb_y) {
    while (*str) {
        kputc(*str, fb_x, fb_y);
        fb_x += PSF1_WIDTH_BITS + FONT_SPACING_WIDTH;
        if (fb_x >= fb_global_info->width) {
            fb_x %= fb_global_info->width;
            fb_y += finf.header->char_size + FONT_SPACING_HEIGHT;
        }

        str++;
    }
}

void kputc(char c, uint64_t fb_x, uint64_t fb_y) {
    char *glyph = finf.data_start + c * finf.header->char_size;

    for (uint8_t y = 0; y < finf.header->char_size; y++) {
        for (uint8_t x = 0; x < PSF1_WIDTH_BITS; x++) {
            if (*(glyph + y) & (0x80 >> x)) {
                fb_global_info
                    ->ptr[(fb_y + y) * (fb_global_info->pitch / 4) + fb_x + x] =
                    TEXT_COLOR;
            }
        }
    }
}

void init_font() {
    finf.header = (psf1_t *)&_binary_zap_psf_start;
    finf.data_start = &_binary_zap_psf_start + HEADER_SIZE;
}
