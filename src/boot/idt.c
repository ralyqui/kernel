#include "idt.h"
#include "boot/gdt.h"
#include "core/fmt.h"

static __attribute__((aligned(0x10))) idt_entry_t idt_table[IDT_NUM_ENTRIES];
static idtr_t idtr;

extern void *isr_stub_table[];

static void idt_set_descriptor(uint8_t vector, void *isr, uint8_t flags) {
    idt_entry_t *descriptor = &idt_table[vector];

    descriptor->isr_low = (uint64_t)isr & 0xFFFF;
    descriptor->kernel_cs = GDT_CODE_PL0_OFFSET;
    descriptor->ist = 0;
    descriptor->attributes = flags;
    descriptor->isr_mid = ((uint64_t)isr >> 16) & 0xFFFF;
    descriptor->isr_high = ((uint64_t)isr >> 32) & 0xFFFFFFFF;
    descriptor->reserved = 0;
}

void exception_handler() {
    print_f("Exception handler called!\n");
    __asm__ volatile("cli; hlt");
}

void init_idt() {
    idtr.base = (uint64_t)&idt_table[0];
    idtr.limit = (uint16_t)sizeof(idt_entry_t) * IDT_NUM_ENTRIES - 1;

    for (uint16_t i = 0; i < IDT_NUM_EXCEPTIONS; i++) {
        idt_set_descriptor(i, isr_stub_table[i], 0x8e);
    }

    __asm__ volatile("lidt %0" : : "m"(idtr));
    __asm__ volatile("sti");

    print_f("IDT setup success\n");
}
