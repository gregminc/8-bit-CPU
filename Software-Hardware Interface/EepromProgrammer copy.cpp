

#include "EepromProgrammer.hpp"

// Shift Register Pins
constexpr uint8_t PIN_SHIFT_DATA  = A0;
constexpr uint8_t PIN_SHIFT_CLOCK = 11;
constexpr uint8_t PIN_SHIFT_LATCH = 12;

// EEPROM Control Pins
constexpr uint8_t PIN_EEPROM_WE = 10;   // Active Low
constexpr uint8_t PIN_EEPROM_OE = A1;   // Active Low

// Data Bus Pins
constexpr uint8_t DATA_PINS[] = {2, 3, 4, 5, 6, 7, 8, 9};

EepromProgrammer::EepromProgrammer() {
    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(LED_BUILTIN, LOW);

    pinMode(PIN_SHIFT_DATA, OUTPUT);
    pinMode(PIN_SHIFT_CLOCK, OUTPUT);
    pinMode(PIN_SHIFT_LATCH, OUTPUT);

    pinMode(PIN_EEPROM_WE, OUTPUT);
    pinMode(PIN_EEPROM_OE, OUTPUT);

    digitalWrite(PIN_EEPROM_WE, HIGH); // write disabled
    digitalWrite(PIN_EEPROM_OE, HIGH); // output disabled
}

void EepromProgrammer::setDataBusMode(uint8_t mode) {
    for (uint8_t i = 0; i < 8; i++) {
        pinMode(DATA_PINS[i], mode);
    }
}

void EepromProgrammer::setAddress(uint16_t address) {
    shiftOut(PIN_SHIFT_DATA, PIN_SHIFT_CLOCK, MSBFIRST, address & 0xFF);
    shiftOut(PIN_SHIFT_DATA, PIN_SHIFT_CLOCK, MSBFIRST, address >> 8);

    digitalWrite(PIN_SHIFT_LATCH, LOW);
    digitalWrite(PIN_SHIFT_LATCH, HIGH);
    digitalWrite(PIN_SHIFT_LATCH, LOW);
}

void EepromProgrammer::setAddressForTest(uint16_t address) {
    setAddress(address);
}

uint8_t EepromProgrammer::readByte(uint16_t address) {
    setAddress(address);
    setDataBusMode(INPUT);

    digitalWrite(PIN_EEPROM_WE, HIGH); // make sure not writing
    digitalWrite(PIN_EEPROM_OE, LOW);  // EEPROM drives data bus

    uint8_t data = 0;

    for (uint8_t i = 0; i < 8; i++) {
        if (digitalRead(DATA_PINS[i])) {
            data |= (1 << i);
        }
    }

    digitalWrite(PIN_EEPROM_OE, HIGH); // disable EEPROM output again

    return fixDipUTurn(data);
}

void EepromProgrammer::writeByte(uint16_t address, uint8_t data) {
    setAddress(address);

    data = fixDipUTurn(data);

    digitalWrite(PIN_EEPROM_OE, HIGH); // EEPROM must not drive bus
    setDataBusMode(OUTPUT);

    for (uint8_t i = 0; i < 8; i++) {
        digitalWrite(DATA_PINS[i], data & (1 << i));
    }

    digitalWrite(PIN_EEPROM_WE, LOW);
    delayMicroseconds(1);
    digitalWrite(PIN_EEPROM_WE, HIGH);

    delay(10);
}

bool EepromProgrammer::writeByteVerified(uint16_t address, uint8_t data) {
    writeByte(address, data);
    return readByte(address) == data;
}

void EepromProgrammer::fastWriteByte(uint16_t address, uint8_t data) {
    
    data = fixDipUTurn(data);

    setAddress(address);

    for (uint8_t i = 0; i < 8; i++) {
        digitalWrite(DATA_PINS[i], data & (1 << i));
    }

    digitalWrite(PIN_EEPROM_WE, LOW);
    delayMicroseconds(1);
    digitalWrite(PIN_EEPROM_WE, HIGH);
}

void EepromProgrammer::disableSDP() {
    digitalWrite(PIN_EEPROM_OE, HIGH);
    setDataBusMode(OUTPUT); 

    noInterrupts(); 

    fastWriteByte(0x5555, 0xAA);
    fastWriteByte(0x2AAA, 0x55);
    fastWriteByte(0x5555, 0x80);
    fastWriteByte(0x5555, 0xAA);
    fastWriteByte(0x2AAA, 0x55);
    fastWriteByte(0x5555, 0x20);

    interrupts(); 
    
    delay(10); 
}

uint8_t EepromProgrammer::fixDipUTurn(uint8_t data) {
    uint8_t lower3 = data & 0x07; // Keep bits 0, 1, 2 exactly as they are (0000 0111)
    
    // Extract and mirror the upper 5 bits
    uint8_t b3 = (data & 0x08) << 4; // Move bit 3 to position 7
    uint8_t b4 = (data & 0x10) << 2; // Move bit 4 to position 6
    uint8_t b5 = (data & 0x20);      // Bit 5 stays perfectly in the middle
    uint8_t b6 = (data & 0x40) >> 2; // Move bit 6 to position 4
    uint8_t b7 = (data & 0x80) >> 4; // Move bit 7 to position 3
    
    // Recombine and return the patched byte
    return lower3 | b3 | b4 | b5 | b6 | b7;
}