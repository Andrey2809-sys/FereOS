#ifndef PORT_H
#define PORT

#include <stdint.h>

// added functions
uint8_t     inb(uint16_t port);
void        outb(uint16_t port, uint8_t value);

#endif