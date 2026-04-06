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

struct test {
    uint32_t a;
    char c;
    uint32_t b;
} __attribute((packed));
extern volatile uint64_t lm_kernel_virt;

void kmain(void) {
    boot();
    draw();

    pmm_init();
    pml4_init();

    print_f("rsp virtual: %l", get_rsp());
    print_f("kernel virtual: %l", lm_kernel_virt);

    for (;;) {
        cpu_halt();
    }
}
