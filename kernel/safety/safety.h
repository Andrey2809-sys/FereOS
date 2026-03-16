#ifndef SAFETY_H
#define SAFETY_H

#include "../drivers/keyboard/kb.h"
#include "../fs/fs.h"
#include "../memory/mem.h"
#include "../shell/sh.h"

#define MAX_PASSWORD_SIZE 80
#define MAX_ATTEMPT_toENTER_PASSWORD 3

char *set_password();
bool  check_password();


#endif