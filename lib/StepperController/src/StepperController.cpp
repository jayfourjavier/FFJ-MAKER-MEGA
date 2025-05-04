#include "StepperController.h"

/**
 * @brief Construct a new StepperController object.
 *
 * Initializes the stepper motor control pins, pulse interval, and direction.
 * The motor position is initialized to zero.
 * 
 * @param _pulPin The pin used for the pulse signal.
 * @param _dirPin The pin used for the direction signal.
 * @param _pulseInterval The time interval between pulses to control the speed of the motor (default is 1).
 * @param _positiveDirection Boolean to set the motor's positive direction (true for clockwise, false for counterclockwise).
 */
StepperController::StepperController(byte _pulPin, byte _dirPin, int _pulseInterval, bool _positiveDirection) {
    this->pulPin = _pulPin;
    this->dirPin = _dirPin;
    this->pulseInterval = _pulseInterval;
    this->positiveDirection = _positiveDirection;
    this->currentPosition = 0;
}

/**
 * @brief Initializes the stepper motor control pins.
 *
 * Configures the pins for pulse and direction as OUTPUT.
 */
void StepperController::init() {
    pinMode(pulPin, OUTPUT);  ///< Set pulse pin as output
    pinMode(dirPin, OUTPUT);  ///< Set direction pin as output
}

/**
 * @brief Moves the stepper motor by a specific number of steps based on the sign of the steps.
 * 
 * The direction is determined by the sign of the `steps` value. Positive steps mean forward, and negative steps
 * mean reverse, depending on the `positiveDirection` setting.
 * 
 * @param steps The number of steps to move the motor. A positive value moves the motor forward, and a negative 
 * value moves it in reverse.
 */
void StepperController::moveTo(long steps) {
    // Determine the direction based on steps and positiveDirection
    bool dir = (steps > 0) ? positiveDirection : !positiveDirection;
    digitalWrite(dirPin, dir ? HIGH : LOW);  ///< Set direction pin

    for (long i = 0; i < abs(steps); i++) {
        digitalWrite(pulPin, HIGH);  ///< Send pulse signal
        delay(pulseInterval);
        digitalWrite(pulPin, LOW);  ///< End pulse signal
        delay(pulseInterval);
    }

    currentPosition += dir ? steps : -steps;  ///< Update current position based on direction
}

/**
 * @brief Moves the stepper motor until a limit switch is triggered.
 * 
 * Moves the motor based on the direction determined by the sign of `steps` until the limit switch is activated.
 * 
 * @param steps The number of steps to move the motor. A positive value moves the motor forward, and a negative 
 * value moves it in reverse.
 * @param limitSwitch A reference to the limit switch object to detect activation.
 */
void StepperController::moveToLimit(long steps, LimitSwitch& limitSwitch) {
    // Determine the direction based on steps and positiveDirection
    bool dir = (steps > 0) ? positiveDirection : !positiveDirection;
    digitalWrite(dirPin, dir ? HIGH : LOW);  ///< Set direction pin

    long stepsTaken = 0;
    while (!limitSwitch.isTriggered()) {  ///< Continue moving until limit switch is triggered
        digitalWrite(pulPin, HIGH);  ///< Send pulse signal
        delay(pulseInterval);
        digitalWrite(pulPin, LOW);  ///< End pulse signal
        delay(pulseInterval);
        stepsTaken++;
    }

    currentPosition += dir ? stepsTaken : -stepsTaken;  ///< Update current position based on direction
}

/**
 * @brief Returns the current position of the stepper motor.
 * 
 * The position is measured in steps, relative to the initial position when the motor was homed.
 * 
 * @return The current position of the stepper motor.
 */
long StepperController::getPosition() const {
    return currentPosition;  ///< Return the current position
}

/**
 * @brief Sets the pulse interval for the stepper motor.
 * 
 * This modifies the time between pulses, which affects the speed of the motor.
 * 
 * @param interval The time interval between pulses (in microseconds).
 */
void StepperController::setPulseInterval(int interval) {
    this->pulseInterval = interval;  ///< Set the new pulse interval
}
