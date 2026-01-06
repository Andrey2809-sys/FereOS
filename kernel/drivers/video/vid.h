
#ifndef VID_H
#define VID_H

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "../../memory/mem.h"


#define VRAM_ADDRESS 0xB8000

// colors
#define GREEN        10
#define CYAN         3
#define YELLOW       14
#define PURPLE       5
#define RED          4
#define BASE_COLOR   YELLOW     


#define CURSOR_X (uint8_t*) (KERNEL_END+0x8)
#define CURSOR_Y (uint8_t*) (KERNEL_END+0x10)

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
void putch(char c, int pos, uint8_t color);       // Put Char
void clear_screen(void);
void kprint(const char *_Str, bool err_mode);     // Print string
void print_int(const int _Num);
void reprint(const char *str);                    // Print in the corner
__attribute__((noreturn)) void panic(const char *_Reason);


#endif