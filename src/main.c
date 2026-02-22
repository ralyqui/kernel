#include "boot/boot.h"
#include "core/cpu.h"
#include "core/fmt.h"
#include "core/mem.h"
#include "graphics/framebuffer.h"

struct test {
    uint32_t a;
    char c;
    uint32_t b;
} __attribute((packed));

void kmain(void) {
    boot();
    draw();

    struct test *t = kmalloc(sizeof(struct test));
    t->a = 0x11;
    t->c = 'a';
    t->b = 0x101;

    print_f("alloc virt %l\n", (uint64_t)t);
    print_f("t struct - %l %l %l", &t->a, &t->c, &t->b);
    // test_pci();

    for (;;) {
        cpu_halt();
    }
}
