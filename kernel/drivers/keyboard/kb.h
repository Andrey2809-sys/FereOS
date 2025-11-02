
#ifndef KB_H
#define KB_H

#include <stdint.h>
#include "../video/vid.h"


typedef struct {
    char ascii;
    char scancode;
} key_obj;


key_obj read_key(void);

#endif