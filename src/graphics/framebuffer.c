#include "framebuffer.h"
#include <stddef.h>

__attribute__((
    used,
    section(
        ".limine_requests"))) static volatile struct limine_framebuffer_request
    framebuffer_request = {.id = LIMINE_FRAMEBUFFER_REQUEST_ID, .revision = 0};

volatile uint32_t *fb_ptr;
uint64_t fb_pitch;
uint64_t fb_width;

void fb_setup() {
    if (framebuffer_request.response == NULL ||
        framebuffer_request.response->framebuffer_count < 1) {
        return;
    }

    struct limine_framebuffer *framebuffer =
        framebuffer_request.response->framebuffers[0];

    fb_ptr = framebuffer->address;
    fb_pitch = framebuffer->pitch;
    fb_width = framebuffer->width;
}
