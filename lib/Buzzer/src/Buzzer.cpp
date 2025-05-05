#include "Buzzer.h"

/**
 * @brief Buzzer class constructor.
 * 
 * @param pin The GPIO pin where the buzzer is connected.
 */
Buzzer::Buzzer(uint8_t pin) : _pin(pin) {}

/**
 * @brief Initializes the buzzer pin.
 */
void Buzzer::begin() {
    pinMode(_pin, OUTPUT);
    digitalWrite(_pin, LOW);
}

/**
 * @brief Produces a sequence of beeps.
 * 
 * @param times Number of beeps.
 * @param duration Length of each beep in milliseconds.
 * @param pause Time to wait between beeps in milliseconds.
 */
void Buzzer::beep(uint8_t times, uint16_t duration, uint16_t pause) {
    for (uint8_t i = 0; i < times; i++) {
        digitalWrite(_pin, HIGH);
        delay(duration);
        digitalWrite(_pin, LOW);
        if (i < times - 1) delay(pause);
    }

    delay(1000);
}
