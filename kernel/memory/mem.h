#ifndef MEMORY_H
#define MEMORY_H

#include "../drivers/video/vid.h"

#define KERNEL_END 0x145FF

// ADDRESSes
#define START_ADDRESS (KERNEL_END + 0x1000)
#define END_ADDRESS   (START_ADDRESS + 0x1000)

#define MEM_STATUS (KERNEL_END + 0x2)


// Memory allocate functions
void *kmalloc(uint32_t size);

// Memory management
void     setmem(uint32_t address, uint32_t smt);
void     setmem8(uint32_t address, uint8_t smt);
uint32_t getmem(uint32_t address);
uint8_t  getmem8(uint32_t address);

void     ptr_to_segoff(void *ptr, uint16_t *seg, uint16_t *off);

// Memory functions



#endif
