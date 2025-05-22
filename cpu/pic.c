#include "pic.h"
#include "lib/serials.h"
#include "keyboard.h"

void init_pic(){
    // ICW1: Start Initialize PICs
    outb(PIC1_COMMAND, 0x11);
    outb(PIC2_COMMAND, 0x11);

    // ICW2: Vector offsets
    outb(PIC1_DATA, 0x20); // PIC1 vector offset (0x20), Master PIC: IRQs 0-7 -> 0x20-0x27
    outb(PIC2_DATA, 0x28); // Slave PIC: IRQs 8-15 -> 0x28-0x2F

    // ICW3: Master/Slave wiring
    outb(PIC1_DATA, 0x04); // Master PIC: Slave at IRQ2, Tell PIC1 that PIC2 is at IRQ2
    outb(PIC2_DATA, 0x02); // Slave PIC: Cascade identity

    // ICW4: 8086 mode
    outb(PIC1_DATA, 0x01);
    outb(PIC2_DATA, 0x01);

    // Unmask IRQ1 (keyboard)
    //outb(PIC1_DATA, 0xFD); // Enable IRQ1 (keyboard) only
    //outb(PIC2_DATA, 0xFF); // Disable PIC2 as unneeded
    // Unmask all
    outb(PIC1_DATA, 0x00);
    outb(PIC2_DATA, 0x00);
    // Mask all
    //outb(PIC1_DATA, 0xFF);
    //outb(PIC2_DATA, 0xFF);
    // Explicitly enable IRQ1
    //unsigned char mask = inb(PIC1_DATA);
    //mask &= ~(1 << IRQ1); // Clear bit for IRQ1
    //outb(PIC1_DATA, mask);
}

void clearPIC(){
    //clear keyboard (if bottleneck) on PIC1, (interrupt then calls in order of keys), last_scancode reset at end
    clear_keyboard_interrupt();
    
    //inb(KEYBOARD_DATA);       // "recieves" first event
    //outb(PIC1_COMMAND, 0x20); // sends EOI
    //cascade in order of events
    //reset any variables
    
    //both needed if on PIC2
    //outb(PIC1_COMMAND, 0x20); // EOI to PIC1, IRQ0-7
    //outb(PIC2_COMMAND, 0x20); // EOI to PIC2, IRQ8-15
}