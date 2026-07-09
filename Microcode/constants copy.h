

#pragma once
#include <cstdint>

const uint32_t ALU_S0   = 1 << 0;
const uint32_t ALU_S1   = 1 << 1;
const uint32_t ALU_S2   = 1 << 2;
const uint32_t ALU_S3   = 1 << 3;
const uint32_t ALU_M    = 1 << 4;
const uint32_t ALU_CIN  = 1 << 5;
const uint32_t ALU_SEL  = 1 << 6;
const uint32_t FLAGS_EN = 1 << 7;
// ALU_SEL selects ALU -> REG A

const uint32_t PC_LOAD  = 1 << 8;
const uint32_t PC_INC   = 1 << 9;
const uint32_t ROM_OE   = 1 << 10;
const uint32_t MAR_EN   = 1 << 11;
const uint32_t RAM_W    = 1 << 12;
const uint32_t IR_EN    = 1 << 13;
const uint32_t HLT      = 1 << 14;
// bit 15 is not used

const uint32_t BUS_SEL_0 = 1 << 21;
const uint32_t BUS_SEL_1 = 1 << 22;
const uint32_t BUS_SEL_2 = 1 << 23;
const uint32_t A_EN      = 1 << 19;
const uint32_t B_EN      = 1 << 20;
const uint32_t STEP_RST  = 1 << 16;
// bits 21, 22 are not used

const uint32_t OP_ADD = 0b01001; // M=0, S=1001 (A plus B)
const uint32_t OP_SUB = 0b00110; // M=0, S=0110 (A minus B minus 1)
const uint32_t OP_AND = 0b11011; // M=1, S=1011 (AB)
const uint32_t OP_OR  = 0b11110; // M=1, S=1110 (A+B)
const uint32_t OP_XOR = 0b10110; // M=1, S=0110 (A XOR B)
const uint32_t OP_NOT = 0b10000; // M=1, S=0000 (~A)

// decode state is not needed in 
// physical implementation
enum State {
    FETCH, ALU_EXEC, ABS_ADDR_FETCH, 
    RAM_READ_A, RAM_READ_B, RAM_WRITE, 
    IMM_LOAD_A, IMM_LOAD_B, 
    JUMP_UNCOND, JUMP_COND, HALT, 
    RAM_WRITE_HOLD
};
