
#ifndef SHELL_H
#define SHELL_H


#include <stdint.h>
#include "../drivers/keyboard/kb.h"
#include "../drivers/video/vid.h"
#include "../memory/mem.h"
#include "../drivers/power/power.h"
#include "../fs/fs.h"
#include "../exec/exec.h"
#include "../safety/safety.h"

#define MAX_CMD_SIZE 80
#define MAX_ARGS     10

#define INPUT_STATUS (KERNEL_END + 0x4)
#define MAGIC_NUMBER 0xDEAD

#define BACKSPACE_STATUS (KERNEL_END + 0x6)

//#define SHELL "$ "
#define SHELL "root@fereos:~$ "

#define VERSION "v2.0"

// all comands
#define INFO            "info"
#define CLEAR_SCREEN    "cls"
#define CHECK_FS        "checkfs"
#define FSI             "fsi"
#define CREATE_FILE     "touch"
#define SHOW_FILES      "ls"
#define CAT             "cat"
#define WRITE_FILE      "write"
#define FILE_INFO       "finf"
#define REMOVE_FILE     "rm"
#define CPUINFO         "cpui"
#define LOGO            "logo"
#define RUN             "run"
#define CHANGE_PASSWORD "chpw"
#define REBOOT          "reboot"
#define POWEROFF        "poweroff"
#define HELP            "help"



void    exec(char *cmd, uint8_t *y);
void    shell(key_obj key, uint8_t *x, uint8_t *y, char *buf, int *inp_pos);
void    logo(void);


// Help functions
bool    strcmp(const char *str1, const char *str2);
void    reset_buf(char *buf);
void    split(char *cmd, char buf[MAX_ARGS][8], int *size);
uint8_t strlen(const char *str);


#endif