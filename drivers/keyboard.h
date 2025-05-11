#pragma once
#ifndef KEYBOARD_H
#define KEYBOARD_H

#define KEYBOARD_DATA 0x60

// Keyboard interrupt handler (C part)
void handle_keyboard_interrupt(void);
// Clear keyboard interrupts at initialization
void clear_keyboard_interrupt();

// Returns last scancode or 0
unsigned char get_last_scancode();

// Convert scancode to char array
char* scancode_to_char(unsigned char scancode);

#endif