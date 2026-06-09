#include "gdt.h"
#include "core/fmt.h"

extern void load_gdt();

volatile uint64_t gdt_table[3];

void init_gdt() {
    gdt_table[0] = create_desc(0x0, 0x0, 0x0);
    gdt_table[1] = create_desc(0x08, 0x0, GDT_CODE_PL0);
    gdt_table[2] = create_desc(0x10, 0x0, GDT_DATA_PL0);

    load_gdt();

    print_f("Gdt succesfully updated\n");
}
