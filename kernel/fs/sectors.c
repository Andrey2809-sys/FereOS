#include "./fs.h"


bool read_sectors(uint8_t _Start_sector, uint8_t _Total_sectors, uint16_t *_Buffer)
{
    uint8_t error = 0;

    uint16_t segment, offset;
    
    ptr_to_segoff(_Buffer, &segment, &offset);

    asm volatile (
        "cli\n\t"
        "push %%es\n\t"
        "movw %[seg], %%es\n\t"
        "int $0x13\n\t"
        "pop %%es\n\t"
        "sti"
        : "=@ccc"(error)
        : "a" ((0x02 << 8) | _Total_sectors),
          "c" ((0 << 8) | _Start_sector),
          "d" ((0 << 8) | 0x80),
          "b" (offset),
          [seg] "r" (segment)
        : "cc", "memory"
    );
    
    return (error == 0 ? true : false);
}


bool write_sectors(uint8_t _Start_sector, uint8_t _Total_sectors, uint16_t *_Buffer)
{
    uint8_t error = 0;

    uint16_t segment, offset;
    
    ptr_to_segoff(_Buffer, &segment, &offset);

    asm volatile (
        "cli\n\t"
        "push %%es\n\t"
        "movw %[seg], %%es\n\t"
        "int $0x13\n\t"
        "pop %%es\n\t"
        "sti"
        : "=@ccc"(error) 
        : "a"((0x03 << 8) | _Total_sectors),  // AH=0x03, AL=total_sectors
          "c"((0 << 8) | _Start_sector),      // CH=0, CL=start_sector
          "d"((0 << 8) | 0x80),              // DH=0, DL=0x80
          "b"(offset),
          [seg] "r" (segment)
        : "cc", "memory"
    );

    return (error == 0 ? true : false);
}