#ifndef LIMITSWITCH_H
#define LIMITSWITCH_H

#include <Arduino.h>

/**
 * @brief A class to manage the functionality of a limit switch.
 *
 * This class is designed to read the status of a limit switch connected to a specified pin.
 * The switch can be checked to see if it has been triggered (i.e., if the pin is HIGH).
 */
class LimitSwitch {
private:
    const byte pin; ///< The pin the limit switch is connected to

public:
    /**
     * @brief Construct a new LimitSwitch object.
     *
     * This constructor initializes the class with a pin number where the limit switch is connected.
     *
     * @param pinNumber The pin to which the limit switch is connected.
     */
    LimitSwitch(byte pinNumber);

    /**
     * @brief Initializes the limit switch pin as input.
     *
     * This function sets up the pin as an input for reading the limit switch state.
     */
    void init();

    /**
     * @brief Checks if the limit switch is triggered.
     *
     * This function reads the state of the limit switch and returns whether it is triggered.
     * A limit switch is considered triggered if the pin reads HIGH.
     *
     * @return true if the limit switch is triggered, false otherwise.
     */
    bool isTriggered();
};

#endif // LIMITSWITCH_H
