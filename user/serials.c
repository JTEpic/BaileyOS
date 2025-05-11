#include "serials.h"

unsigned char inb(unsigned short port) {
    unsigned char val;
    asm volatile("inb %1, %0" : "=a"(val) : "Nd"(port));
    return val;
}

void outb(unsigned short port, unsigned char val) {
    asm volatile("outb %0, %1" : : "a"(val), "Nd"(port));
}

void serial_write(unsigned char c) {
    if(c=='\n')
        outb(SERIAL_PORT, '\r');
    outb(SERIAL_PORT, c);
}

void serial_message(const unsigned char* msg){
    for (int i = 0; msg[i]!='\0'; i++) {
        serial_write(msg[i]);
    }
}