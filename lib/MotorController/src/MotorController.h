#ifndef MOTORCONTROLLER_H
#define MOTORCONTROLLER_H

#include <Arduino.h>  // Include the Arduino library for digitalWrite, analogWrite, etc.


class MotorController {
private:
    byte motorEnaPin;  ///< Pin for enabling the motor
    byte motorPwmPin;  ///< Pin for controlling the motor speed (PWM)
    bool isMotorOn;    ///< Variable to monitor the motor's current state (on/off)

public:
    /**
     * @brief Construct a new MotorController object.
     * 
     * Initializes the motor control pins and sets the motor to be off initially.
     * 
     * @param _motorEnaPin The pin used to enable the motor.
     * @param _motorPwmPin The pin used to control the motor speed (PWM).
     */
    MotorController(byte _motorEnaPin, byte _motorPwmPin);

    /**
     * @brief Initializes the motor control pins.
     * 
     * Configures the pins as OUTPUT and sets the motor to off initially.
     */
    void init();

    /**
     * @brief Turns on the motor with the specified speed.
     * 
     * Maps the input speed (0-100%) to a PWM value (0-255) and turns on the motor only if it's currently off.
     * 
     * @param speed The speed of the motor in percentage (0 to 100).
     */
    void turnOn(int speed);

    /**
     * @brief Turns off the motor if it's currently on.
     */
    void turnOff();

    /**
     * @brief Returns the current status of the motor (on/off).
     * 
     * @return `true` if the motor is on, `false` if the motor is off.
     */
    bool isMotorOnStatus() const;
};

#endif  // MOTORCONTROLLER_H
