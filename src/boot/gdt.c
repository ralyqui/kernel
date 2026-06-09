#include "gdt.h"
#include "core/fmt.h"

struct gdtr_t {
    uint16_t limit;
    uint64_t base;
} __attribute__((packed));

extern void load_gdt();

volatile uint64_t gdt_table[3];
volatile struct gdtr_t gdtr_ptr;

void init_gdt() {
    gdt_table[0] = create_desc(0x0, 0x0, 0x0);
    gdt_table[1] = create_desc(0x08, 0x0, GDT_CODE_PL0);
    gdt_table[2] = create_desc(0x10, 0x0, GDT_DATA_PL0);

    gdtr_ptr.limit = sizeof(gdt_table) - 1;
    gdtr_ptr.base = (uint64_t)gdt_table;

    load_gdt();

    print_f("Gdt succesfully updated\n");
}
