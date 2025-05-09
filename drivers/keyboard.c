#include "keyboard.h"
#include "serials.h"
#include "pic.h"
#include "idt.h"

// Global variable to store last scancode
static volatile unsigned char last_scancode;

// Keyboard interrupt handler (C part)
void handle_keyboard_interrupt() {
    last_scancode = inb(KEYBOARD_DATA);
    outb(PIC1_COMMAND, 0x20); // Send EOI, end of interrupt
    //outb(PIC2_COMMAND, 0x20);
}

unsigned char get_last_scancode() {
    unsigned char scancode = last_scancode;
    last_scancode = 0; // Clear after reading
    return scancode;
}

unsigned char scancode_to_char(unsigned char scancode) {
    if (scancode == 0x1E) return 'A';
    if (scancode == 0x30) return 'B';
    return '~'; // Unknown
}