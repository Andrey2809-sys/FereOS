#include "sh.h"



void exec(char *cmd)
{
    print("\n", true);
    
    if (strcmp(cmd, "info")) {
        print(" This is FereOS v1.0", false);
    }
    else if (strcmp(cmd, "cls")) {
        clear_screen();
    }
    else if (strcmp(cmd, "reboot")) {
        reboot();
    }
    else if (strcmp(cmd, "shutdown")) {
        shutdown();
    }
    else if (strcmp(cmd, "help")) {
        print(" Available comands: info, cls, reboot, shutdown", false);
    }
    else {
        print(" Unknown comand: ", true);
        print(cmd, true);
    }
}

void shell(key_obj key, uint8_t *x, uint8_t *y, char *buf, int *inp_pos)
{    
    uint16_t pos = *x + *y*80;

    switch (key.scancode) {
        case BACKSPACE:
            if (getmem(BACKSPACE_STATUS) == pos--) {
                return;
            }
            else {
                *x -= 1;
                *inp_pos -= 1;
                buf[*inp_pos] = ' ';
                putch(' ', pos, GREEN);
            }
            break;
    
        case ESCAPE:
            shutdown();
            break;
    
        case ENTER:          
            exec(buf);             

            print("\n", false);
            print(SHELL, false);

            reset_buf(buf);
            *inp_pos = 0;

            *x = strlen(SHELL);
            *y += 2;
            setmem(BACKSPACE_STATUS, *y*80+*x);

            break;
    
        case LEFT_ARROW:
            *x -= 1;
            break;

        case RIGHT_ARROW:
            *x += 1;  
            break;
    
        default:
        
            if (*x < MAX_CMD_SIZE) {
                (buf)[*inp_pos] = (char) key.ascii;
                *inp_pos += 1;
                
                putch(key.ascii, pos, GREEN);               
                *x += 1;          
            }
            break;
    }
}

// Help functions

bool strcmp(const char *str1, const char *str2)
{
    while (*str1 && *str2 && (*str1 == *str2)) {
        str1++;
        str2++;
    }
    return (*str1 == *str2);
}

void reset_buf(char *buf)
{
    for (int i = 0; i < MAX_CMD_SIZE+1; i++) {
        buf[i] = 0;
    }
}

uint8_t strlen(const char *str) {
    const char *s = str;
    while (*s) s++;
    return (s - str);
}