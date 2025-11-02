//
// Created by 12345 on 23.10.2025.
//

#ifndef MEMORY_H
#define MEMORY_H

#include "../drivers/video/vid.h"

#define KERNEL_END 0x12800

// ADDRESSes
#define START_ADDRESS 0x13000
#define END_ADDRESS   0x14000

#define MEM_STATUS 0x12910


// Memory allocate functions
void *kmalloc(uint32_t size);

// Memory management
void     setmem(uint32_t address, uint32_t smt);
uint32_t getmem(uint32_t address);

#endif
