#include "boot/boot.h"
#include "core/cpu.h"
#include "core/fmt.h"
#include "core/paging.h"
#include "core/pmm.h"
#include "graphics/framebuffer.h"
#include "graphics/kshell.h"
#include "io/pci.h"
#include "limine.h"
#include <drivers/nic.h>
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

    for (int i = 0; i < 256; i++) {
        for (int j = 0; j < 16; j++) {
            pci_device_info info;
            if (!pci_read_slot(i, j, 0, &info))
                continue;

            print_f("device id is %x and vendor id is %x\n", info.device_id,
                    info.vendor_id);

            if (info.device_id == I82540EM_DEVICE_ID &&
                info.vendor_id == I82540EM_VENDOR_ID) {
                nic_init(i, j);
            }
        }
    }

    for (;;) {
        cpu_halt();
    }
}
