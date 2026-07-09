

#pragma once
#include "constants.h"

// physically, SEL_0 = ROM, SEL_1 = REG A, SEL_2 = RAM
// REG A reads the bus by default
// flags are {N, Z, C}
uint32_t get_control_signals(uint8_t opcode, int state, uint8_t flags) {
    uint32_t signals = 0;

    switch (state) {
        case FETCH:
            signals = BUS_SEL_0 | IR_EN | PC_INC;
            break;
        case ALU_EXEC:
            signals = ALU_SEL | A_EN | FLAGS_EN;

            if (opcode == 0x30) signals |= OP_ADD;
            if (opcode == 0x31) signals |= (OP_SUB | ALU_CIN);
            if (opcode == 0x32) signals |= OP_AND;
            if (opcode == 0x33) signals |= OP_OR;
            if (opcode == 0x34) signals |= OP_XOR;
            if (opcode == 0x35) signals |= OP_NOT;
            break;
        case ABS_ADDR_FETCH:
            signals = BUS_SEL_0 | MAR_EN | PC_INC;
            break;
        case RAM_READ_A:
            signals = BUS_SEL_2 | A_EN;
            break;
        case RAM_READ_B:
            signals = BUS_SEL_2 | B_EN;
            break;
        case RAM_WRITE:
            signals = BUS_SEL_1 | RAM_W;
            break;
        case RAM_WRITE_HOLD:
            signals = BUS_SEL_1;
            break;
        case IMM_LOAD_A:
            signals = BUS_SEL_0 | A_EN | PC_INC;
            break;
        case IMM_LOAD_B:
            signals = BUS_SEL_0 | B_EN | PC_INC;
            break;
        case JUMP_UNCOND:
            signals = BUS_SEL_0 | PC_LOAD;
            break;
        case JUMP_COND: {
            bool allow_jump = false;
            bool is_zero    = (flags & 0b010) != 0;
            bool is_neg     = (flags & 0b100) != 0;

            if (opcode == 0x41 && is_zero) allow_jump = true;
            if (opcode == 0x42 && is_neg)  allow_jump = true;

            if (allow_jump) {
                signals = BUS_SEL_0 | PC_LOAD;
            } else {
                signals = PC_INC;
            }
            break;
        }
        case HALT:
            signals = HLT;
            break;
        default: 
            signals = 0;
            break;
    }

    // invert flag signal to active-low
    signals ^= FLAGS_EN; 
    signals ^= IR_EN;
    signals ^= STEP_RST;
    signals ^= ALU_CIN;

    // signals ^= RAM_W;

    return signals;
}
