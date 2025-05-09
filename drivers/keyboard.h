#pragma once
#ifndef KEYBOARD_H
#define KEYBOARD_H

#define KEYBOARD_DATA 0x60

void handle_keyboard_interrupt(void);

unsigned char get_last_scancode();

unsigned char scancode_to_char(unsigned char scancode);

#endif