#include "./exec.h"


bool validate_program(uint8_t *program, int opcodes)
{
    int pos = 0;

    while (pos < opcodes) {
        uint8_t opcode = program[pos];

        //  ==== 1-byte opcodes ====
        // interrupts
        if ((opcode == CLI) || (opcode == STI) || (opcode == IRET)) return false;
        
        // cpu control
        if ((opcode == HLT) || (opcode == WAIT)) return false;
        
        // flags
        if ((opcode == PUSHF) || (opcode == POPF)) return false;       
        
        // in-out port through dx
        if ((opcode == IN_DX_8) || (opcode == IN_DX_16) ||
            (opcode == OUT_DX_8) || (opcode == OUT_DX_16)) {
                return false;
        }
        
        // ==== 2-byte opcodes ====
        
        if (opcode == INT) {
            if ((pos+1) >= opcodes) return false;
            if (program[pos+1] != SYSTEM_INTERRUPT) return false;
            pos += 2;
            continue;
        }

        // in-out ports
        if ((opcode == IN_imm8) || (opcode == IN_imm16) || (opcode == OUT_imm8) || (opcode == OUT_imm16)) {
            if ((pos+1) >= opcodes) return false;
            pos += 2;
            continue;
        }

        pos += 1;
    }
    return true;
}


void load_program(uint8_t *program, int opcodes) 
{
    //fill in with 0
    for (uint16_t offset = 0; offset < MAX_PROGRAM_SIZE; offset++) {
        *((uint8_t*)(PROGRAM_LOAD_ADDR + offset)) = 0;
    }
    // load in the address    
    for (uint8_t offset = 0; offset < opcodes; offset++) {
        *((uint8_t*)(PROGRAM_LOAD_ADDR + offset)) = *(program + offset);
    }
}

void switch_to_program()
{
    SET_STACK();
    
    void (*code)(void) = (void(*)(void)) LOAD_OFFSET;    
    code();
    
    RESTORE_STACK();
}


void execute_program(uint8_t *program, int size)
{
    if (validate_program(program, size)) {
        load_program(program, size);
        switch_to_program();
    }
    else {
        kprint("[ERROR] program can`t be executed", true);
        return;
    }   
}
