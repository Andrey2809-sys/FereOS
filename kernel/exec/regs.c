#include "./exec.h"


void read_regs(struct regs16_t *_Reg)
{
    uint16_t *ptr = (uint16_t*)_Reg;
    
    asm volatile (
        "movw %%ax, 0(%0)\n\t"    // _Reg->ax
        "movw %%bx, 2(%0)\n\t"    // _Reg->bx  
        "movw %%cx, 4(%0)\n\t"    // _Reg->cx
        "movw %%dx, 6(%0)\n\t"    // _Reg->dx
        "movw %%si, 8(%0)\n\t"    // _Reg->si
        "movw %%di, 10(%0)\n\t"   // _Reg->di
        :
        : "r" (ptr)
        : "memory"
    );
}


uint16_t get_ax(void)
{
    uint16_t _ax;
    asm volatile ("movw %%ax, %0\n" : "=r" (_ax));
    return _ax;
}

uint16_t get_bx(void)
{
    uint16_t _bx;
    asm volatile ("movw %%bx, %0\n" : "=r" (_bx));
    return _bx;
}