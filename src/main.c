#include "boot/boot.h"
#include "core/cpu.h"
#include "core/fmt.h"
#include "core/mem.h"
#include "core/paging.h"
#include "core/pmm.h"
#include "graphics/framebuffer.h"
#include "limine.h"
#include <graphics/font.h>

__attribute__((
    used,
    section(
        ".limine_requests"))) static volatile struct limine_paging_mode_request
    pm_request = {.id = LIMINE_PAGING_MODE_REQUEST_ID, .revision = 0};

void kmain(void) {
    boot();
    fb_setup();
    init_font();

    pmm_init();
    pml4_init();

    puts("Starting the shell..", 20, 20);

    for (;;) {
        cpu_halt();
    }
}
