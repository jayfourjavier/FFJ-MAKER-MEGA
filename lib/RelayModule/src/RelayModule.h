#ifndef RELAY_MODULE_H
#define RELAY_MODULE_H

#include <Arduino.h>

/**
 * @brief A class to control an active-low relay module with internal state tracking.
 */
class RelayModule {
private:
    byte relayPin;     ///< GPIO pin connected to the relay module
    bool _isOn;         ///< Internal status of the relay (true = ON, false = OFF)

public:
    /**
     * @brief Constructor for RelayModule.
     * @param _relayPin The pin connected to the relay's control input.
     */
    RelayModule(byte _relayPin);

    /**
     * @brief Initializes the relay pin as output and sets it to OFF (HIGH).
     */
    void init();

    /**
     * @brief Turns the relay ON by setting the pin LOW.
     */
    void turnOn();

    /**
     * @brief Turns the relay OFF by setting the pin HIGH.
     */
    void turnOff();

    /**
     * @brief Gets the current status of the relay.
     * @return true if the relay is ON, false otherwise.
     */
    bool isOn() const;
};

#endif // RELAY_MODULE_H
