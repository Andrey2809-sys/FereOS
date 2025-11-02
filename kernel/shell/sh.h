#ifndef SHELL_H
#define SHELL_H


#include <stdint.h>
#include "../drivers/keyboard/kb.h"
#include "../drivers/video/vid.h"
#include "../memory/mem.h"
#include "../drivers/power/power.h"

#define ESCAPE      0x01
#define BACKSPACE   0x0E
#define LEFT_ARROW  0x4B
#define TOP_ARROW   0x48
#define RIGHT_ARROW 0x4D
#define DOWN_ARROW  0x50
#define ENTER       0x1C  

#define MAX_CMD_SIZE 80

#define INPUT_STATUS 0x12920
#define MAGIC_NUMBER 0xDEAD

#define BACKSPACE_STATUS 0x12930

#define SHELL "$ "

void    exec(char *cmd);
void    shell(key_obj key, uint8_t *x, uint8_t *y, char *buf, int *inp_pos);

// Help functions
bool    strcmp(const char *str1, const char *str2);
void    reset_buf(char *buf);
uint8_t strlen(const char *str);


#endif