#include "drivers/video/vid.h"
#include "drivers/keyboard/kb.h"
#include "shell/sh.h"
#include "memory/mem.h"


__attribute__((noreturn)) 
void kmain()
{
    
    // init os
    vga_t vga;
    key_obj key;
    uint16_t pos;
    uint16_t new_backspace_pos;
    int inp_pos = 0;
    
    setmem(MEM_STATUS, START_ADDRESS);
    setmem(BACKSPACE_STATUS, strlen(SHELL));

    // clear screen
    clear_screen();

    // shell
    setvga(0,0);
    
    char *inp_buf = (char*) kmalloc(MAX_CMD_SIZE+1);
    
    print(SHELL, false);
    
    vga = getvga();
    
    bool flag = true;
    while (flag) {
        key = read_key();
        
        shell(key, &vga.x, &vga.y, inp_buf, &inp_pos);
    }
 
    while (1) asm("hlt");
}