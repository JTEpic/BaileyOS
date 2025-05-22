#pragma once
#ifndef SERIALS_H
#define SERIALS_H

#define SERIAL_PORT 0x3F8

// recieve in char
unsigned char inb(unsigned short port);

// send out char
void outb(unsigned short port, unsigned char val);

// send out word
void outw(unsigned short port, unsigned short val);

// singular char write
void serial_write(unsigned char c);

// message until \0
void serial_message(const unsigned char* msg);

// update cursor position
void updateCursor(unsigned int pos);

#endif