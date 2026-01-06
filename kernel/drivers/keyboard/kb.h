
#ifndef KEYBOARD_H
#define KEYBOARD_H

#include <stdint.h>
#include "../video/vid.h"


typedef struct {
    char ascii;
    char scancode;
} key_obj;


void read_key(key_obj *_Key);


#endif