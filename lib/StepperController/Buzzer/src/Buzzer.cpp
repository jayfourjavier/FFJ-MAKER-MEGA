#include "Buzzer.h"

Buzzer::Buzzer(uint8_t pin) : _pin(pin) {}

void Buzzer::begin() {
    pinMode(_pin, OUTPUT);
    digitalWrite(_pin, LOW);
}

void Buzzer::beep(uint8_t times, uint16_t duration, uint16_t pause) {
    for (uint8_t i = 0; i < times; i++) {
        digitalWrite(_pin, HIGH);
        delay(duration);
        digitalWrite(_pin, LOW);
        if (i < times - 1) delay(pause);
    }
}
