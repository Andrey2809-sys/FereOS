#include "vid.h"


void setvga(uint8_t x, uint8_t y)
{
    uint8_t *cursor_x =  CURSOR_X;
    uint8_t *cursor_y =  CURSOR_Y;
    
    *cursor_x = x;
    *cursor_y = y;
}

vga_t getvga()
{
    volatile uint8_t *cursor_x = CURSOR_X;
    volatile uint8_t *cursor_y = CURSOR_Y;
    uint16_t pos = (*cursor_x) + (*cursor_y)*WIDTH;

    return (vga_t){
        .x   = *cursor_x,
        .y   = *cursor_y,
        .pos = pos
    };
}


void putch(char c, int pos, uint8_t color)
{
    asm volatile ("cli");
    volatile uint16_t *vid = (volatile uint16_t*) VRAM_ADDRESS;
    vid[pos] = (color << 8) | c;
    asm volatile ("sti");
}


void clear_screen(void) 
{
    for (int pos = 0; pos < WIDTH * HEIGHT; pos++) {
        putch(' ', pos, 0x0F00);
    }
}


void kprint(const char *_Str, bool err_mode) {
    uint8_t color = err_mode ? RED : BASE_COLOR;
    
    volatile uint8_t *x = CURSOR_X;
    volatile uint8_t *y = CURSOR_Y;
    
    
    for (int pos = 0; _Str[pos] != '\0'; pos++) {
        if (_Str[pos] == '\n') {
            setvga(0, *y+1);    
        }
        else {
            if (*x == WIDTH) {
                return;
            }
            putch(_Str[pos], *y*WIDTH+*x, color);
            *x += 1;
        }
    }
    
}


void print_int(const int _Num) {
    char buffer[16];
    char *ptr = buffer + 15;
    *ptr = '\0';
    
    int is_negative = _Num < 0;
    unsigned int n = is_negative ? -_Num : _Num;
    
    // Lookup table с цифрами
    const char digits[] = "0123456789";
    
    do {
        *--ptr = digits[n % 10];  
        n /= 10;
    } while (n > 0);
    
    if (is_negative) {
        *--ptr = '-';
    }
    
    kprint(ptr, false);
}


void reprint(const char *str)
{
    volatile uint16_t *vid = (volatile uint16_t*) VRAM_ADDRESS;
    
    while(*str) {
        *vid++ = (GREEN << 8) | *str++;
    }
}


__attribute__((noreturn)) void panic(const char *_Reason)
{
    clear_screen();
    setvga(0,0);
    
    kprint(_Reason, true);
    
    asm volatile ("cli;hlt");
}
