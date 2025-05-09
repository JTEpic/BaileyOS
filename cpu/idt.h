#pragma once
#ifndef IDT_H
#define IDT_H

// Default handler for undefined if PIC enabled
extern void default_handler(void);
void handle_default_interrupt(void);
// IRQ1 (33) for Keyboard and External interrupt handler (defined in kernel.asm & drivers)
#define IRQ1 0x21
extern void keyboard_handler(void);

// IDT, Interrupt Descriptor Table, entry structure
struct idt_entry {
    unsigned short base_low;
    unsigned short sel;
    unsigned char zero;
    unsigned char flags;
    //unsigned short base_mid;
    unsigned short base_high;
    //unsigned int reserved;
} __attribute__((packed));

struct idt_ptr {
    unsigned short limit;
    unsigned int base;
} __attribute__((packed));

void set_idt_entry(int index, unsigned int handler, unsigned short sel, unsigned char flags);
//32bit
void init_idt();

#endif