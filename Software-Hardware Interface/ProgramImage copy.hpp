
                       
#pragma once
#include <Arduino.h>

constexpr uint16_t PROGRAM_START_ADDRESS = 0x00;

constexpr uint8_t PROGRAM_IMAGE[] = {
    0x10,
    0xA6,
    0x20,
    0x21,
    0x10,
    0x59,
    0x20,
    0x84,
    0x11,
    0x21,
    0x13,
    0x84,
    0x30,
    0x20,
    0x40,
    0x01,
};

constexpr uint16_t PROGRAM_SIZE = sizeof(PROGRAM_IMAGE) / sizeof(PROGRAM_IMAGE[0]);

