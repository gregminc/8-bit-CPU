

#include <iomanip>
#include <fstream>
#include <iostream>
#include "generate_microcode.hpp"

uint8_t reverse_bits(uint8_t b) {
    b = (b & 0xF0) >> 4 | (b & 0x0F) << 4;
    b = (b & 0xCC) >> 2 | (b & 0x33) << 2;
    b = (b & 0xAA) >> 1 | (b & 0x55) << 1;
    return b;
}

int get_address(int opcode, int flags, int halt_pin, int step) {
    uint8_t flipped_opcode = reverse_bits((uint8_t)opcode);
    
    int address = flipped_opcode; 

    if (flags & 0b001) address |= (1 << 8);  // Flag 0 -> A8
    if (flags & 0b010) address |= (1 << 9);  // Flag 1 -> A9
    if (flags & 0b100) address |= (1 << 11); // Flag 2 -> A11

    if (step & 0b01) address |= (1 << 12);   // Step 0 -> A12
    if (step & 0b10) address |= (1 << 14);   // Step 1 -> A14

    if (halt_pin) address |= (1 << 13);

    return address;
}

int main() {
    const int ROM_SIZE = 1 << 15; // 32768
    uint32_t rom[ROM_SIZE];

    for (int i = 0; i < ROM_SIZE; i++) {
        rom[i] = 0;
    }

    for (int step = 0; step <= 3; step++) {
        for (int halt_pin = 0; halt_pin <= 1; halt_pin++) {
            for (int flags = 0; flags <= 7; flags++) {
                for (int opcode = 0; opcode <= 255; opcode++) {
                    int current_address = get_address(opcode, flags, halt_pin, step);

                    if (halt_pin == 1) {
                        rom[current_address] = HLT;
                        continue;
                    } 

                    if (step == 0) {
                        rom[current_address] = get_control_signals(opcode, FETCH, flags);
                        continue;
                    }

                    uint32_t signals = 0;
                    bool is_last_step = false;

                    if (opcode == 0x00) {
                        if (step == 1) {
                            is_last_step = true;
                        }
                    }

                    else if (opcode == 0x01) {
                        if (step == 1) signals = get_control_signals(opcode, HALT, flags);
                    }

                    else if (opcode == 0x10) {
                        if (step == 1) {
                            signals = get_control_signals(opcode, IMM_LOAD_A, flags);
                            is_last_step = true;
                        }
                    }

                    else if (opcode == 0x11) {
                        if (step == 1) signals = get_control_signals(opcode, ABS_ADDR_FETCH, flags);
                        if (step == 2) {
                            signals = get_control_signals(opcode, RAM_READ_A, flags);
                            is_last_step = true;
                        }
                    }

                    else if (opcode == 0x12) {
                        if (step == 1) {
                            signals = get_control_signals(opcode, IMM_LOAD_B, flags);
                            is_last_step = true;
                        }
                    }

                    else if (opcode == 0x13) {
                        if (step == 1) signals = get_control_signals(opcode, ABS_ADDR_FETCH, flags);
                        if (step == 2) {
                            signals = get_control_signals(opcode, RAM_READ_B, flags);
                            is_last_step = true;
                        }
                    }

                    else if (opcode == 0x20) {
                        if (step == 1) {
                            signals = get_control_signals(opcode, ABS_ADDR_FETCH, flags);
                        }

                        if (step == 2) {
                            signals = get_control_signals(opcode, RAM_WRITE, flags);
                        }

                        if (step == 3) {
                            signals = get_control_signals(opcode, RAM_WRITE_HOLD, flags);
                            is_last_step = true;
                        }
                    }

                    else if (opcode >= 0x30 && opcode <= 0x35) {
                        if (step == 1) {
                            signals = get_control_signals(opcode, ALU_EXEC, flags);
                            is_last_step = true;
                        } 
                    }

                    else if (opcode == 0x40) {
                        if (step == 1) {
                            signals = get_control_signals(opcode, JUMP_UNCOND, flags);
                            is_last_step = true;
                        }
                    }

                    else if (opcode == 0x41 || opcode == 0x42) {
                        if (step == 1) {
                            signals = get_control_signals(opcode, JUMP_COND, flags);
                            is_last_step = true;
                        }
                    }    

                    if (is_last_step) {
                    signals &= ~STEP_RST; 
                    }

                    rom[current_address] = signals;
                }
            }
        }
    }

    std::ofstream chip1("rom_logic.hex");
    std::ofstream chip2("rom_memory.hex");
    std::ofstream chip3("rom_traffic_cop.hex");

    for (int i = 0; i < ROM_SIZE; i++) {
        uint8_t c1_byte = rom[i] & 0xFF;
        uint8_t c2_byte = (rom[i] >> 8) & 0xFF;
        uint8_t c3_byte = (rom[i] >> 16) & 0xFF;

        chip1 << std::hex << std::uppercase << std::setfill('0') << std::setw(2) << (int)c1_byte << "\n";
        chip2 << std::hex << std::uppercase << std::setfill('0') << std::setw(2) << (int)c2_byte << "\n";
        chip3 << std::hex << std::uppercase << std::setfill('0') << std::setw(2) << (int)c3_byte << "\n";
    }

    chip1.close();
    chip2.close();
    chip3.close();

    std::cout << "Files generated succesfully." << std::endl;

    return 0;

}