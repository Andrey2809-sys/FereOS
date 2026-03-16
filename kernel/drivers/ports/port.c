#include "./port.h"

uint8_t inb(uint16_t port)
{
    uint8_t value;
    asm volatile (
        "inb %1, %0\n\t"
        : "=a" (value)
        : "Nd" (port) 
        :
    );
    return value;
}


void outb(uint16_t port, uint8_t value)
{
    asm volatile (
        "outb %0, %1\n\t"
        :
        : "a" (value), "Nd" (port)
        :
    );
}
