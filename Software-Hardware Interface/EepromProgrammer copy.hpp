

#pragma once
#include <Arduino.h>

class EepromProgrammer {

public:
    EepromProgrammer();

    uint8_t readByte(uint16_t address);
    void writeByte(uint16_t address, uint8_t data);
    bool writeByteVerified(uint16_t address, uint8_t data);
    void setAddressForTest(uint16_t address);
    void disableSDP();

private:
    void setDataBusMode(uint8_t mode);
    void setAddress(uint16_t address);
    void fastWriteByte(uint16_t address, uint8_t data);
    uint8_t fixDipUTurn(uint8_t data);
};


