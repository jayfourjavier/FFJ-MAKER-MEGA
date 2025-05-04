#include "LimitSwitch.h"

/**
 * @brief Construct a new LimitSwitch object.
 *
 * The constructor sets the pin number where the limit switch is connected.
 *
 * @param pinNumber The pin to which the limit switch is connected.
 */
LimitSwitch::LimitSwitch(byte pinNumber) : pin(pinNumber) {
    pinMode(pin, INPUT);  // Set the pin as input
}

/**
 * @brief Initializes the limit switch pin as input.
 *
 * This function configures the pin mode for the limit switch.
 * It ensures that the pin is set to input mode to properly read the state of the switch.
 */
void LimitSwitch::init() {
    pinMode(pin, INPUT);
}

/**
 * @brief Checks if the limit switch is triggered.
 *
 * This function reads the state of the pin. If the pin is HIGH, it indicates that the limit switch
 * has been triggered.
 *
 * @return true if the limit switch is triggered, false otherwise.
 */
bool LimitSwitch::isTriggered() {
    return digitalRead(pin) == HIGH;  // Return true if the switch is triggered (HIGH)
}
