#include "boot/boot.h"
#include "core/cpu.h"
#include "core/fmt.h"
#include "core/paging.h"
#include "core/pmm.h"
#include "graphics/framebuffer.h"
#include "graphics/kshell.h"
#include "limine.h"
#include <graphics/font.h>

__attribute__((
    used,
    section(
        ".limine_requests"))) static volatile struct limine_paging_mode_request
    pm_request = {.id = LIMINE_PAGING_MODE_REQUEST_ID, .revision = 0};

void kmain(void) {
    boot();
    pmm_init();
    pml4_init();

    fb_setup();
    init_font();

    kshell_init();
    kprint("Starting the shell");

    for (;;) {
        cpu_halt();
    }
}
