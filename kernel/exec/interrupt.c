#include "./exec.h"

void setup_interrupts()
{
    asm volatile ("cli");

    set_interrupt_handler(SYSTEM_INTERRUPT, &isr_handler);
    
    asm volatile ("sti");
}


void set_interrupt_handler(uint8_t code, void (*int_handler)(void))
{
    *((uint16_t*)(code * 4)) = (uint32_t) int_handler - KERNEL_START;
    *((uint16_t*)(code * 4 + 2)) = KERNEL_CS;
}



__attribute__((naked)) void isr_handler(void)
{
    uint16_t ax = get_ax();
    
    __asm__ volatile (
        "pushw %%ax\n\t"
        :::
    );

    uint8_t ah = (ax >> 8) & 0xFF;
    uint8_t al = ax & 0xFF;
    
    switch (ah) {
        case SYS_WRITE:
            
            char *msg = kmalloc(2);
            msg[0] = al;
            msg[1] = '\0';
            
            kprint(msg, false);
            break;
    
        case SYS_READ:

            char *word = kmalloc(2);
            word[0] = read_ascii();
            word[1] = 0;

            kprint(word, false);            
            break;
    
        case SYS_MALLOC:
                    
            void *ptr = kmalloc(al);
            uint16_t offset = (uint16_t) ptr;
        
            asm volatile (
                "movw $0x1000, %%ax\n\t"
                "movw %%ax, %%gs\n\t"
                
                "movw %[ptr_addr], %%bx\n\t"
                :
                : [ptr_addr] "r" (offset)
                : "bx"
            );
            break;
            
            case SYS_EXIT:
            asm volatile (
                "popw %%ax\n\t"
                "addw $6, %%sp\n\t"
                "ret\n\t"
                ::: "memory"
            );
            
            
            default:
            kprint("\nUndefined syscall number\n", true);
            break;
        }
        __asm__ volatile (
            "popw %%ax\n\t"
            "iretw\n\t"   
            ::: "memory"
        );
}
    
    
 