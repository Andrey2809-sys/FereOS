#ifndef EXEC_H
#define EXEC_H

#include <stdint.h>
#include <stdbool.h>
#include "../drivers/video/vid.h"
#include "../drivers/power/power.h"
#include "../memory/mem.h"
#include "../drivers/keyboard/kb.h"
#include "../fs/fs.h"


#define KERNEL_CS            0x1000
#define KERNEL_START         0x10000
#define KERNEL_STACK_POINTER 0x7C00

#define PROGRAM_LOAD_ADDR     ((KERNEL_END + 0x6000) & 0x1F000)
#define PROGRAM_STACK_POINTER 0x9000
#define LOAD_OFFSET           (PROGRAM_LOAD_ADDR - KERNEL_START)  

#define PROGRAM_CS         (PROGRAM_LOAD_ADDR >> 4)


#define SYSTEM_INTERRUPT 0x80

// syscalls
#define SYS_EXIT        0
#define SYS_WRITE       1
#define SYS_READ        2
#define SYS_MALLOC      3

#define MAX_PROGRAM_SIZE 512

// bad opcodes
#define CLI       0xFA
#define STI       0xFB
#define IRET      0xCF

#define HLT       0xF4
#define WAIT      0x9B

#define PUSHF     0x9C
#define POPF      0x9D

#define INT       0xCD

#define IN_DX_8   0xEC
#define IN_DX_16  0xED
#define IN_imm8   0xE4
#define IN_imm16  0xE5

#define OUT_imm8  0xE6
#define OUT_imm16 0xE7
#define OUT_DX_8  0xEE
#define OUT_DX_16 0xEF


#define SET_STACK() asm volatile (   \
        "cli\n\t"                    \
        "movw %0, %%sp\n\t"          \
        "sti\n\t"                    \
        :                            \
        : "i"(PROGRAM_STACK_POINTER) \
        : "memory"                   \
    );

#define RESTORE_STACK() asm volatile (   \
        "cli\n\t"                        \
        "movw %0, %%sp\n\t"              \
        "sti\n\t"                        \
        :                                \
        : "i"(KERNEL_STACK_POINTER)      \
        : "memory"                       \
    );


struct regs16_t {
    uint16_t ax;    // accumilator register
    uint16_t bx;    // base register
    uint16_t cx;    // count register
    uint16_t dx;    // data register

    uint16_t si;    // source index
    uint16_t di;    // destination index
} __attribute__((packed));


void read_regs(struct regs16_t * _Reg);

// interrupts
void setup_interrupts();
void set_interrupt_handler(uint8_t code, void (*int_handler)(void));
__attribute__((naked)) void isr_handler(void);

// get registers
uint16_t get_ax(void);
uint16_t get_bx(void);


// execute program
bool validate_program(uint8_t *program, int opcodes);
void load_program(uint8_t *program, int opcodes);
void switch_to_program();
void execute_program(uint8_t *program, int size);


#endif