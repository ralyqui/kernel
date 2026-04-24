#ifndef FRAMEBUFFER_FONT_H
#define FRAMEBUFFER_FONT_H

#include <core/fmt.h>
#include <stdint.h>

typedef struct {
    uint16_t magic;
    uint8_t font_mode;
    uint8_t char_size;
} __attribute__((packed)) psf1_t;

extern char _binary_zap_psf_start;
extern char _binary_zap_psf_end;

void puts(char *str, uint64_t fb_x, uint64_t fb_y);
void putc(char c, uint64_t fb_x, uint64_t fb_y);

void init_font();

#endif
