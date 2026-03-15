#include "boot/boot.h"
#include "core/cpu.h"
#include "core/fmt.h"
#include "core/mem.h"
#include "core/paging.h"
#include "graphics/framebuffer.h"
#include "limine.h"

__attribute__((
    used,
    section(
        ".limine_requests"))) static volatile struct limine_paging_mode_request
    pm_request = {.id = LIMINE_PAGING_MODE_REQUEST_ID, .revision = 0};

struct test {
    uint32_t a;
    char c;
    uint32_t b;
} __attribute((packed));

void kmain(void) {
    boot();
    draw();
    // setup_paging();
    // print_l((uint64_t)&page_directory);
    pml4_init();
    // struct test *t = kmalloc(sizeof(struct test));

    for (;;) {
        cpu_halt();
    }
}
