#include "RelayModule.h"

/**
 * @brief Constructor for the RelayModule class.
 * 
 * Initializes the relay pin and sets the initial state to OFF (inactive).
 * 
 * @param _relayPin The digital pin connected to the relay control input.
 */
RelayModule::RelayModule(byte _relayPin) {
    this->relayPin = _relayPin;
    this->_isOn = false;  // Default to OFF
}

/**
 * @brief Initializes the relay pin as OUTPUT and sets it to OFF (HIGH for active-low relay).
 */
void RelayModule::init() {
    pinMode(relayPin, OUTPUT);
    digitalWrite(relayPin, HIGH);  // Active-low: HIGH = OFF
    _isOn = false;
    Serial.print("Relay on pin ");
    Serial.print(relayPin);
    Serial.println(" initialized to OFF (HIGH)");
}

/**
 * @brief Turns ON the relay by setting the pin LOW (active-low) if it is not already ON.
 */
void RelayModule::turnOn() {
    if (!_isOn) {
        digitalWrite(relayPin, LOW);  // Active-low: LOW = ON
        _isOn = true;
        Serial.print("Relay on pin ");
        Serial.print(relayPin);
        Serial.println(" turned ON (LOW)");
    }
}

/**
 * @brief Turns OFF the relay by setting the pin HIGH if it is not already OFF.
 */
void RelayModule::turnOff() {
    if (_isOn) {
        digitalWrite(relayPin, HIGH); // Active-low: HIGH = OFF
        _isOn = false;
        Serial.print("Relay on pin ");
        Serial.print(relayPin);
        Serial.println(" turned OFF (HIGH)");
    }
}

/**
 * @brief Returns the current logical status of the relay.
 * 
 * @return true if the relay is ON (energized), false if it is OFF.
 */
bool RelayModule::isOn() const {
    Serial.print("Relay on pin ");
    Serial.print(relayPin);
    Serial.print(" is currently ");
    Serial.println(_isOn ? "ON" : "OFF");
    return _isOn;
}
