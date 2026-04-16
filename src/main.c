#include "boot/boot.h"
#include "core/cpu.h"
#include "core/fmt.h"
#include "core/mem.h"
#include "core/paging.h"
#include "core/pmm.h"
#include "graphics/framebuffer.h"
#include "limine.h"

__attribute__((
    used,
    section(
        ".limine_requests"))) static volatile struct limine_paging_mode_request
    pm_request = {.id = LIMINE_PAGING_MODE_REQUEST_ID, .revision = 0};

void print_cr3() {
    uint64_t cr3_val;

    __asm__ volatile("mov %0, cr3" : "=r"(cr3_val));
    print_f("CR3 value is %l\n", cr3_val);
}

void kmain(void) {
    boot();
    draw();

    print_cr3();

    pmm_init();
    pml4_init();

    print_cr3();

    for (;;) {
        cpu_halt();
    }
}
