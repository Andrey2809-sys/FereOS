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
    volatile uint16_t *vid = (volatile uint16_t*) VRAM_ADDRESS;
    vid[pos] = (color << 8) | c;
}


void clear_screen(void) 
{
    for (int pos = 0; pos < WIDTH*HEIGHT; pos++) {
        putch(' ', pos, 0x0F00);
    }
}

void print(const char *str, bool err_mode) {

    uint8_t color = err_mode ? RED : GREEN;
    
    volatile uint8_t *x = CURSOR_X;
    volatile uint8_t *y = CURSOR_Y;
    
    
    for (int pos = 0; str[pos] != '\0'; pos++) {
        if (str[pos] == '\n') {
            setvga(0, *y+1);    
        }
        else {
            if (*x == WIDTH) {
                return;
            }
            putch(str[pos], *y*WIDTH+*x, color);
            *x += 1;
        }
    }
    
}



void reprint(const char *str)
{
    volatile uint16_t *vid = (volatile uint16_t*) VRAM_ADDRESS;
    
    while(*str) {
        *vid++ = (GREEN << 8) | *str++;
    }
}

__attribute__((noreturn)) void panic(const char *reason)
{
    clear_screen();
    setvga(0,0);
    
    print(reason, true);
    

    asm volatile ("cli;hlt");
}
