#include <Arduino.h>
#include "EepromProgrammer.hpp"
#include "ProgramImage.hpp"

EepromProgrammer programmer;

void blink(uint8_t count, uint16_t delayMs = 150) {

    for (uint8_t i = 0; i < count; i++) {
        digitalWrite(LED_BUILTIN, HIGH);
        delay(delayMs);
        digitalWrite(LED_BUILTIN, LOW);
        delay(delayMs);
    }
}

void signalErrorForever() {
    while (true) {
        blink(4, 100);
        delay(500);
    }
}

bool writeAndVerifyByte(uint16_t address, uint8_t data) {
    constexpr uint8_t MAX_ATTEMPTS = 3;
    constexpr unsigned long WRITE_TIMEOUT_MS = 50;

    uint8_t current = programmer.readByte(address);
    if (current == data) {
        return true;
    }

    for (uint8_t attempt = 1; attempt <= MAX_ATTEMPTS; attempt++) {
        programmer.writeByte(address, data);

        unsigned long startTime = millis();
        while (millis() - startTime < WRITE_TIMEOUT_MS) {
            uint8_t actual = programmer.readByte(address);
            if (actual == data) {
                return true;
            }
            delay(1);
        }

        delay(10);
    }

    return programmer.readByte(address) == data;
}

void setup() {
    Serial.begin(115200);

    unsigned long serialStartTime = millis();
    while (!Serial && millis() - serialStartTime < 3000) {
    }

    delay(500);

    Serial.println("Serial connected.");
    Serial.println("Address register test mode.");
    blink(1); // started

    Serial.println("Programming EEPROM...");
    blink(1); // started

    // remove manufacturer lock
    Serial.println("Disabling SDP...");
    programmer.disableSDP();
    Serial.println("SDP disable sequence sent.");

    for (uint16_t i = 0; i < PROGRAM_SIZE; i++) {
        uint16_t address = PROGRAM_START_ADDRESS + i;
        uint8_t data = PROGRAM_IMAGE[i];

        if ((i & 0xFF) == 0) {
            Serial.print("Programming address 0x");
            Serial.println(address, HEX);
        }

        if (!writeAndVerifyByte(address, data)) {
            uint8_t actual = programmer.readByte(address);

            Serial.print("Verify failed at address 0x");
            Serial.println(address, HEX);
            Serial.print("Expected 0x");
            Serial.println(data, HEX);
            Serial.print("Read 0x");
            Serial.println(actual, HEX);
            signalErrorForever();
        }
    }

    Serial.println("Programming successful.");
    digitalWrite(LED_BUILTIN, HIGH);
}

void loop() {
    delay(1000);
}