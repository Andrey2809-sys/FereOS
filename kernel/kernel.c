#include "drivers/video/vid.h"
#include "drivers/keyboard/kb.h"
#include "shell/sh.h"
#include "memory/mem.h"
#include "fs/fs.h"

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
        
    
    if (!check_fs()) {
        // first running
        struct superblock sb;
        uint8_t bitmap[512] = {0};

        for (int pos = 0; pos < FIRST_DATA_SECTOR; pos++) {
            bitmap[pos] = 1;
        }
        struct nano_fnode root_dir[MAX_FILES] = {0}; 
        
        if (init_fs(&sb) && write_sectors(BITMAP_SECTOR, 1, bitmap) && write_sectors(ROOT_DIR_SECTOR, 1, (uint16_t*)root_dir)) {
            kprint("formatting disk\n", true);
            kprint("Welcome to FereOS v1.3! Enter help to info\n", false);
        }
        else {
            panic("[ERROR] while formatting disk");
        }
    }
    
    
    kprint(SHELL, false);
    

    vga = getvga();
    
    bool flag = true;
    while (flag) {
        
        read_key(&key);

        shell(key, &vga.x, &vga.y, inp_buf, &inp_pos);     
    }
     
    while (1) {
        asm("hlt");
    }
}