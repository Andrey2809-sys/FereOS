#include "power.h"

void shutdown(void)
{
    asm volatile (
        "mov $0x5307, %%ax\n\t"
        "mov $0x0001, %%bx\n\t"
        "mov $0x0003, %%cx\n\t"
        "int $0x15"
        : : : "ax", "bx", "cx"
    );
}

void reboot(void)
{
    asm volatile (
        "mov $0xFE, %%al\n\t"
        "out %%al, $0x64\n\t"
        : : : "al"
    );
}