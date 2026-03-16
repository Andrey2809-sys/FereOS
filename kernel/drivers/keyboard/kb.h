
#ifndef KEYBOARD_H
#define KEYBOARD_H

#include <stdint.h>
#include "../video/vid.h"

#define ESCAPE      0x01
#define BACKSPACE   0x0E
#define LEFT_ARROW  0x4B
#define TOP_ARROW   0x48
#define RIGHT_ARROW 0x4D
#define DOWN_ARROW  0x50
#define ENTER       0x1C  

typedef struct {
    char ascii;
    char scancode;
} key_obj;

void read_key(key_obj *_Key);
char read_ascii(void);

#endif