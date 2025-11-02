#include "kb.h"



key_obj read_key(void)
{
    key_obj key;
    
    asm volatile (
        "mov $0x00, %%ax\n"
        "int $0x16\n"
        "mov %%al, %[ascii]\n"
        "mov %%ah, %[scan_code]\n"
        : [ascii]"=r"(key.ascii), [scan_code]"=r"(key.scancode)
        :
        : "ax", "cc"
        
    );

    return key;
}
