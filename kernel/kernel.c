#include "drivers/video/vid.h"
#include "drivers/keyboard/kb.h"
#include "shell/sh.h"
#include "memory/mem.h"
#include "fs/fs.h"
#include "exec/exec.h"
#include "safety/safety.h"
#include "drivers/time/time.h"

__attribute__((noreturn)) 
void kmain()
{    
    // init os
    vga_t vga;
    key_obj key;
    int inp_pos = 0;
    
    setmem(MEM_STATUS, START_ADDRESS);
    setmem(BACKSPACE_STATUS, strlen(SHELL));
    
    
    // clear screen
    clear_screen();
    // shell
    setvga(0,0);
    
    char *inp_buf = (char*) kmalloc(MAX_CMD_SIZE+1);
        
    setup_interrupts();
    
    if (!check_fs()) {
        // first running
        struct superblock sb;
        uint8_t bitmap[512] = {0};

        for (int pos = 0; pos < FIRST_DATA_SECTOR; pos++) {
            bitmap[pos] = 1;
        }
        struct nano_fnode root_dir[MAX_FILES] = {0}; 
        
        char *password = set_password();
        strcpy(sb.password, password);

        if (init_fs(&sb) && write_sectors(BITMAP_SECTOR, 1, bitmap)) {
            kprint("Formatting disk\n", true);
        }
        else {
            panic("[ERROR] while formatting disk");
        }

        kprint("Welcome to FereOS ", false);
        kprint(VERSION, false);
        kprint("\nEnter help to info\n", false);
        logo();
    }
    else {
        int attempt = 0;
        bool result = check_password();
        
        while (result == false) {
            if (attempt > MAX_ATTEMPT_toENTER_PASSWORD) {
                panic("Too many attempts. Your device is locked.");
            }
            kprint("Wrong\n", true);
            result = check_password();
            attempt++;
        }
    }

    kprint(SHELL, false);
    
    if (file_exist("pr.bin") == FILE_NOT_EXIST)  {
        uint8_t program[] = {
            0xB4, 1,    // mov ah, 1 (sys_write)
            0xB0, 0x5A, // mov al, 'Z'
            0xCD, 0x80, // int 0x80

            0xB4, 0x00, // mov ah, 0 (sys_exit)
            0xCD, 0x80  // int 0x80
        };

        create_file("pr.bin", EXECUTABLE);
        write_file_int("pr.bin", program, 10);
    }

    vga = getvga();
    
    bool flag = true;
    while (flag) {
        show_time();    
        
        read_key(&key);

        shell(key, &vga.x, &vga.y, inp_buf, &inp_pos); 
    }
     
    while (1) {
        asm("hlt");
    }
}