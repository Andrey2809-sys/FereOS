#include "kb.h"



void read_key(key_obj *_Key)
{    
    asm volatile (
        "mov $0x00, %%ax\n"
        "int $0x16\n"
        "mov %%al, %[ascii]\n"
        "mov %%ah, %[scan_code]\n"
        : [ascii]"=r"(_Key->ascii), [scan_code]"=r"(_Key->scancode)
        :
        : "ax", "cc"
        
    );
}
