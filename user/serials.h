#pragma once
#ifndef SERIALS_H
#define SERIALS_H

#define SERIAL_PORT 0x3F8

// recieve in char
unsigned char inb(unsigned short port);

// send out char
void outb(unsigned short port, unsigned char val);

void serial_write(unsigned char c);

void serial_message(unsigned char msg[]);

#endif