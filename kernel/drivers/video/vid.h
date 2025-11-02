
#ifndef VID_H
#define VID_H

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
//#include "../../memory/mem.h"


#define VRAM_ADDRESS 0xB8000
#define GREEN        0x02
#define RED          0x04

#define CURSOR_X (uint8_t*) 0x12800
#define CURSOR_Y (uint8_t*) 0x12804

#define CURRENT NULL

#define WIDTH  80
#define HEIGHT 25

// VGA 
typedef struct {
    uint8_t x;
    uint8_t y;
    uint16_t pos;
} vga_t;

void  setvga(uint8_t x, uint8_t y);
vga_t getvga();


// Output
void putch(char c, int pos, uint8_t color); // Put Char
void clear_screen(void);
void print(const char *str, bool err_mode); // Print string
void print_int(int num);
void reprint(const char *str);                    // Print in the corner
__attribute__((noreturn)) void panic(const char *reason);


#endif