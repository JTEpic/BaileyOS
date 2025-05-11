#include "idt.h"
#include "pic.h"
#include "serials.h"

// Global IDT
#define IDT_SIZE 256
struct idt_entry idt[IDT_SIZE];
struct idt_ptr idtp;

void set_idt_entry(int index, unsigned int handler, unsigned short sel, unsigned char flags) {
    idt[index].base_low = handler & 0xFFFF;
    idt[index].base_high = (handler >> 16) & 0xFFFF;
    idt[index].sel = sel;
    idt[index].zero = 0;
    idt[index].flags = flags;
}

void handle_default_interrupt(void) {
    outb(PIC1_COMMAND, 0x20); // Send EOI to master PIC
    outb(PIC2_COMMAND, 0x20); // Send EOI to slave PIC if PIC2,IRQ8-15
}

void init_idt() {
    //idtp.limit = sizeof(idt) - 1;
    idtp.limit = (sizeof(struct idt_entry) * IDT_SIZE) - 1;
    idtp.base = (unsigned int)&idt;

    // Set all IDT entries to default handler
    for (int i = 0; i < IDT_SIZE; i++) {
        // Clear IDT
        //set_idt_entry(i, 0, 0, 0);
        set_idt_entry(i, (unsigned int)default_handler, 0x08, 0x8E); // 0x8E: present, ring 0, interrupt gate
    }

    // Set keyboard handler (IRQ1, vector 0x21 (33))
    set_idt_entry(IRQ1, (unsigned int)keyboard_handler, 0x08, 0x8E);

    // Load IDT
    asm volatile("lidt %0" : : "m"(idtp));
}