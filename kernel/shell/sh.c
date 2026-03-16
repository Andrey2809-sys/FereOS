#include "./sh.h"


void exec(char *cmd, uint8_t *y)
{
    kprint("\n", false);
    
    char commands[MAX_ARGS][8] = {0};
    int args;
    split(cmd, commands, &args);

    if (strcmp(commands[0], INFO)) {
        kprint(" This is FereOS ", false);
        kprint(VERSION, false);
        *y += 1;
    }
    else if (strcmp(commands[0], CLEAR_SCREEN)) {
        clear_screen();
        setvga(0,0);
        *y = 0;
    }
    else if (strcmp(commands[0], CHECK_FS)) {
        if (check_fs()) {
            kprint(" Good", false);
        }
        else {
            kprint(" [ERROR]", true);
        }
        *y += 1;
    }
    else if (strcmp(commands[0], FSI)) {
        kprint(FS_INFO, false);

        kprint("\nFree place(available): ", false);

        int res = 0;
        free_place(&res);
        
        int kb = (res*512)/1024;
        print_int(kb);
        kprint(" KB\n", false);        
                
        uint16_t buf[256];
        read_sectors(SUPERBLOCK_SECTOR, 1, buf);
        struct superblock *sb = (struct superblock*) buf;
        kprint("Mounting at ", false);
        print_date(sb->creation_date);
        kprint("  ", false);
        print_time(sb->creation_time);
        
        *y += 3;
    }
    else if (strcmp(commands[0], CREATE_FILE)) {
            
        uint8_t arg;
        if (strcmp(commands[2], "w")) {
            arg = WRITE;
        }
        else if (strcmp(commands[2], "r")) {
            arg = READ;
        }
        else {
            kprint("Unknown file argument(w, r)", true);
            *y += 1;
            return;
        }

        int result = create_file(commands[1], arg);
        if (result == SUCCESS) {
            kprint("File successfuly create!", false);
            *y += 1;
            return;
        }
        else if (result == FILE_ALREADY_EXIST) {
            kprint("[ERROR] file already exist", true);
            *y += 1;
            return;
        }
        else {
            *y += 1;
            return;
        }
    }
    else if (strcmp(commands[0], SHOW_FILES)) {
        uint8_t y_before = *(CURSOR_Y);
        
        show_files();

        uint8_t y_after = *(CURSOR_Y);
        *y += (y_after - y_before + 1);
    }
    else if (strcmp(commands[0], CAT)) {
        struct nano_fnode file = find_file(commands[1]);

        if (file.attribute == EXECUTABLE) {
            uint8_t *data = kmalloc(MAX_FILE_SIZE);
            if (data == NULL) {
                kprint("[ERROR] can`t allocate memory", true);
                *y += 1;
            }
            int size = 0;
            int result = read_file_int(commands[1], data, &size);

            for (int i = 0; i < size; i++) {
                print_int(data[i]);
                kprint(" ", false);
            }
        }
        else {
            char *data = kmalloc(MAX_FILE_SIZE);
            if (data == NULL) {
                kprint("[ERROR] can`t allocate memory", true);
                *y += 1;
            }
            int result = read_file(commands[1], data);
            if (result == FILE_NOT_EXIST) {
                kprint("[ERROR] file not exist", true);
            }
            else {
                kprint(data, false);
            }
        }
        *y += 1;
    }
    else if (strcmp(commands[0], WRITE_FILE)) {
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
    else if (strcmp(commands[0], FILE_INFO)) {
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

        kprint("Created at ", false);
        print_date(file.creation_date);
        kprint("   ", false);
        print_time(file.creation_time);

        *y += 3;
    }
    else if (strcmp(commands[0], REMOVE_FILE)) {
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
    else if (strcmp(commands[0], CPUINFO)) {
        struct regs16_t *regs = kmalloc(sizeof(struct regs16_t));
        read_regs(regs);

        kprint("AX: ", false);
        print_int(regs->ax);
        kprint("        ", false);
        
        kprint("BX: ", false);
        print_int(regs->bx);
        
        kprint("\nCX: ", false);
        print_int(regs->cx);
        kprint("        ", false);
        
        kprint("DX: ", false);
        print_int(regs->dx);

        kprint("\nSI: ", false);
        print_int(regs->si);
        kprint("        ", false);

        kprint("DI: ", false);
        print_int(regs->di);               

        *y += 3;
    }
    else if (strcmp(commands[0], LOGO)) {
        logo();
        *y += 7;
    }
    else if (strcmp(commands[0], RUN)) {
        uint8_t y_before = *(CURSOR_Y);
        
        int size = 0;
        uint8_t   *file_buf = kmalloc(MAX_FILE_SIZE);
        if (file_buf == NULL) {
            kprint("[ERROR] can`t allocate memory", true);
            *y += 1;
        }
        int result = read_file_int(commands[1], file_buf, &size);
        
        switch (result) {
            case READ_SECTORS_ERROR:
                kprint("[ERROR] while read sectors", true);
                *y += 1;
                return;
            case WRITING_SECTORS_ERROR:
                kprint("[ERROR] while writing sectors", true);
                *y += 1;
                return;
            case FILE_NOT_EXIST:
                kprint("[ERROR] file doesn`t exist", true);
                *y += 1;
                return;
        }
        
        execute_program(file_buf, size);

        uint8_t y_after = *(CURSOR_Y);
        
        *y += (y_after - y_before + 1);
    }
    else if (strcmp(commands[0], CHANGE_PASSWORD)) {
        uint8_t y_before = *(CURSOR_Y);

        if (check_password()) {
            char *password = set_password();
            
            uint16_t superblock_buffer[256];    
            if (!read_sectors(SUPERBLOCK_SECTOR, 1, superblock_buffer)) {
                kprint("[ERROR] while reading sectors\n", true);
                return;
            }
            struct superblock *sb = (struct superblock*) superblock_buffer;
            strcpy(sb->password, password);
            write_sectors(SUPERBLOCK_SECTOR, 1, (uint16_t*)sb);
        }

        uint8_t y_after = *(CURSOR_Y);

        *y += (y_after - y_before + 1);
    }
    else if (strcmp(commands[0], REBOOT)) {
        reboot();
    }
    else if (strcmp(commands[0], POWEROFF)) {
        shutdown();
    }
    else if (strcmp(commands[0], HELP)) {
        kprint(" Available comands: \n info - info about system, cls - clear screen\n checkfs - check file system, fsi - info about file system\n touch <filename.ext> mode - create file, ls - show all files\n cat <filename.ext> - read file, finf <filename.ext> - show info about file \n rm <filename.ext> - remove file\n reboot - reboot your pc, poweroff - shutdown your pc\n cpui - get values from registers of your cpu, logo - print logo\n run <filename.bin> - execute program, chpw - change password", false);
        *y += 9;
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
        
            (buf)[*inp_pos] = (char) key.ascii;
            *inp_pos += 1;
            
            putch(key.ascii, pos, GREEN);     

            if (*x < MAX_CMD_SIZE) {
                *x += 1;          
            }
            else {
                *y += 1;
                *x = 0;
            }
            break;
    }
}

void logo(void)
{
    kprint(" _____  _____  ____   _____    ______   _____                     \n", false);
    kprint("|      |      |    | |        |      | |                        \n", false);
    kprint("|_____ |_____ |____| |_____   |      | |_____      \n", false);
    kprint("|      |      |\\     |        |      |       |    \n", false);
    kprint("|      |_____ |  \\   |_____   |______|  _____|     \n\n", false);
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

void split(char *cmd, char buf[MAX_ARGS][8], int *size)
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
