#include "sh.h"


void exec(char *cmd, uint8_t *y)
{
    kprint("\n", false);
    
    char commands[MAX_ARGS][10] = {0};
    int args;
    split(cmd, commands, &args);

    if (strcmp(commands[0], "info")) {
        kprint(" This is FereOS v1.3", false);
        *y += 1;
    }
    else if (strcmp(commands[0], "cls")) {
        clear_screen();
        setvga(0,0);
        *y = 0;
    }
    else if (strcmp(commands[0], "checkfs")) {
        if (check_fs()) {
            kprint(" good", false);
        }
        else {
            kprint(" [ERROR]", true);
        }
        *y += 1;
    }
    else if (strcmp(commands[0], "fsi")) {
        kprint(FS_INFO, false);

        kprint("\nfree place: ", false);

        int res = 0;
        free_place(&res);
        
        int kb = (res*512)/1024;
        print_int(kb);
        kprint(" KB", false);        
                
        *y += 2;
    }
    else if (strcmp(commands[0], "touch")) {
            
        uint8_t arg;
        if (strcmp(commands[2], "w")) {
            arg = WRITE;
        }
        else if (strcmp(commands[2], "r")) {
            arg = READ;
        }
        else if (strcmp(commands[2], "e")) {
            arg = EXECUTABLE;
        }
        else {
            kprint("Unknown file argument(w, r, e)", true);
            *y += 1;
            return;
        }

        int result = create_file(commands[1], arg);
        if (result == SUCCESS) {
            kprint("File successfuly create!", false);
            *y += 1;
            return;
        }
        else {
            *y += 1;
            return;
        }
    }
    else if (strcmp(commands[0], "ls")) {
        show_files();
        *y += 1;
    }
    else if (strcmp(commands[0], "cat")) {
        
        char *data = {0};
        int result = read_file(commands[1], data);
        if (result == FILE_NOT_EXIST) {
            kprint("[ERROR] file not exist", true);
        }
        else {
            kprint(data, false);
        }
        *y += 1;
    }
    else if (strcmp(commands[0], "write")) {
        struct nano_fnode file = find_file(commands[1]);
        if (file.attribute == SYSTEM_FILE) {
            kprint("[ERROR] you can`t write in system file", true);
            *y += 1;
            return;
        }
        int word = 2;
        int word_len = 0;
        for (int i = word; i <= args; i++) {
            word_len += strlen(commands[i]);
            if (i > word) {
                word_len++;
            }
        }
        char data[word_len];
        
        int pos = 0;
        for (int i = word; i <= args; i++) {
            if (i > word) {
                data[pos++] = ' ';
            }
            strcpy(data + pos, commands[i]);
            pos += strlen(commands[i]);
        }
        data[pos] = '\0';
        
        int result = write_file(commands[1], data);
        switch (result) {
            case READ_SECTORS_ERROR:
                kprint("[ERROR] while read sectors", true);
                *y += 1;
                break;
            case WRITING_SECTORS_ERROR:
                kprint("[ERROR] while writing sectors", true);
                *y += 1;
                break;
            case READONLY_FILE:
                kprint("[ERROR] readonly file", true);
                *y += 1;
                break;
            case FILE_NOT_EXIST:
                kprint("[ERROR] file doesn`t exist", true);
                *y += 1;
                break;
            default:
                *y += 1;
                break;
        }

    }
    else if (strcmp(commands[0], "finf")) {
        struct nano_fnode file = find_file(commands[1]);
        if (file.attribute == 0) {
            kprint("File doesn`t exist", true);
            *y += 1;
            return;
        }

        kprint("Mode:", false);
        switch (file.attribute) {
            case WRITE:
                kprint("write", false);
                break;
            case READ:
                kprint("readonly", false);
                break;
            case SYSTEM_FILE:
                kprint("system file", false);
                break;
            case EXECUTABLE:
                kprint("executable file", false);
                break;
        }
        kprint("\nSize:",false);
        print_int(file.file_size);

        *y += 2;
    }
    else if (strcmp(commands[0], "rm")) {
        struct nano_fnode file = find_file(commands[1]);
        if (file.attribute == SYSTEM_FILE) {
            kprint("[ERROR] you can`t delete system file", true);
            *y += 1;
            return;
        }
        int result = remove_file(commands[1]); 
        switch (result) {
            case READ_SECTORS_ERROR:
                kprint("[ERROR] while read sectors", true);
                break;
            case WRITING_SECTORS_ERROR:
                kprint("[ERROR] while writing sectors", true);
                break;
            case FILE_NOT_EXIST:
                kprint("[ERROR] file doesn`t exist", true);
                break;
        }   
        *y += 1;
    }
    else if (strcmp(commands[0], "reboot")) {
        reboot();
    }
    else if (strcmp(commands[0], "shutdown")) {
        shutdown();
    }
    else if (strcmp(commands[0], "help")) {
        kprint(" Available comands: \n info - info about system, cls - clear screen\n checkfs - check file system, fsi - info about file system\n touch <filename.ext> mode - create file, ls - show all files,\n write <filename.ext> data - write data into a file\n rm <filename.ext> - remove file\n cat <filename.ext> - read file, finf <filename.ext> - show info about file \n reboot - reboot your pc, shutdown - shutdown your pc", false);
        *y += 8;
    }
    else {
        kprint(" Unknown comand: ", true);
        kprint(cmd, true);
        *y += 1;
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
            
            break;
        
        case ENTER:    
            exec(buf, y);             
            

            kprint("\n", false);
            kprint(SHELL, false);

            reset_buf(buf);
            *inp_pos = 0;

            *x = strlen(SHELL);
            *y += 1;
            //*y += 1;
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

void split(char *cmd, char buf[MAX_ARGS][10], int *size)
{
    int word = 0;
    int word_pos = 0;
    for (int i = 0; i <= strlen(cmd); i++) {
        if (cmd[i] == ' '){
            buf[word][word_pos] = 0;
            word += 1;
            word_pos = 0;
        }
        else{

            buf[word][word_pos] = cmd[i];
            word_pos++;
        }
    
    }
    buf[word][word_pos] = 0;
    *size = word;
}
