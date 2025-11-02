#include "mem.h"

void *kmalloc(uint32_t size)
{ 
    uint32_t current_ptr = getmem(MEM_STATUS);
    
    void *result;
    
    if ((current_ptr + size) >= END_ADDRESS) {
        return NULL;
    }
    else {
        result = (void*)current_ptr;
    }

    setmem(MEM_STATUS, current_ptr+size);

    return result;
}

void setmem(uint32_t address, uint32_t smt)
{
    if ((address < 0x12800) || (address >= 0x13000)) {
        panic("[PANIC] set bad memory");
    }
    volatile uint32_t *mem = (volatile uint32_t*)address;
    *mem = smt;
}


uint32_t getmem(uint32_t address)
{
    if ((address < 0x12800) || (address > 0x13000)) {
        panic("[PANIC] get bad memory");
    }
    volatile uint32_t *mem = (volatile uint32_t*) address;
    return *mem;
}

