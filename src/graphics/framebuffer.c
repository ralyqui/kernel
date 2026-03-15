#include "framebuffer.h"
#include <stddef.h>

__attribute__((
    used,
    section(
        ".limine_requests"))) static volatile struct limine_framebuffer_request
    framebuffer_request = {.id = LIMINE_FRAMEBUFFER_REQUEST_ID, .revision = 0};

void draw() {
    if (framebuffer_request.response == NULL ||
        framebuffer_request.response->framebuffer_count < 1) {
        return;
    }

    struct limine_framebuffer *framebuffer =
        framebuffer_request.response->framebuffers[0];

    for (size_t i = 0; i < framebuffer->height; i++) {
        volatile uint32_t *fb_ptr = framebuffer->address;
        fb_ptr[i * (framebuffer->pitch / 4) + i] = 0xffffff;
    }
}
