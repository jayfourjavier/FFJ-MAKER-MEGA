#include "MotorController.h"

MotorController::MotorController(byte _motorEnaPin, byte _motorPwmPin) {
    this->motorEnaPin = _motorEnaPin;
    this->motorPwmPin = _motorPwmPin;
    this->isMotorOn = false;  ///< Initial motor state is off
}

void MotorController::init() {
    pinMode(motorEnaPin, OUTPUT);  ///< Set enable pin as output
    pinMode(motorPwmPin, OUTPUT);  ///< Set PWM pin as output
    digitalWrite(motorEnaPin, LOW);  ///< Ensure the motor is off initially
    analogWrite(motorPwmPin, 0);  ///< Ensure the PWM is set to 0 initially
}

void MotorController::turnOn(int speed) {
    if (!isMotorOn) {  ///< Check if the motor is off before turning it on
        int pwmValue = map(speed, 0, 100, 255 , 0);  ///< Map speed to PWM value
        digitalWrite(motorEnaPin, HIGH);  ///< Enable the motor
        analogWrite(motorPwmPin, pwmValue);  ///< Set the motor speed using PWM
        isMotorOn = true;  ///< Update the motor status to on
    }
}

void MotorController::turnOff() {
    if (isMotorOn) {  ///< Check if the motor is on before turning it off
        digitalWrite(motorEnaPin, LOW);  ///< Disable the motor
        analogWrite(motorPwmPin, 0);  ///< Set the motor speed to 0 (off)
        isMotorOn = false;  ///< Update the motor status to off
    }
}

bool MotorController::isMotorOnStatus() const {
    return isMotorOn;  ///< Return the current motor status
}
