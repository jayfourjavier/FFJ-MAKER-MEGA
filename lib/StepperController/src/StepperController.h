/**
 * @file StepperController.h
 * @brief Header file for the StepperController class.
 *
 * This file contains the declaration of the StepperController class, which is responsible for controlling
 * a stepper motor using a specified pulse and direction pins. The class provides functions to move the 
 * motor to a target position, move it until a limit switch is triggered, and get the current position of the motor.
 * 
 * The motor's direction and pulse rate can be configured, and the current position is maintained relative to the 
 * motor's home position.
 * 
 * @version 1.0
 * @date 2025-05-04
 * 
 * @author [Your Name]
 */

 #ifndef STEPPERCONTROLLER_H
 #define STEPPERCONTROLLER_H
 
 #include <Arduino.h>
 #include "LimitSwitch.h"  ///< Include the LimitSwitch class for limit switch functionality
 
 /**
  * @class StepperController
  * @brief A class to control a stepper motor using pulse and direction pins.
  * 
  * This class provides methods to control the movement of a stepper motor, including moving to a specific position, 
  * moving until a limit switch is triggered, and getting the current position.
  */
 class StepperController {
 public:
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
     StepperController(byte _pulPin, byte _dirPin, int _pulseInterval = 1, bool _positiveDirection = true);
 
     /**
      * @brief Initializes the stepper motor control pins.
      * 
      * Configures the pins for pulse and direction as OUTPUT.
      */
     void init();
 
     /**
      * @brief Moves the stepper motor by a specific number of steps based on the sign of the steps.
      * 
      * The direction is determined by the sign of the `steps` value. Positive steps mean forward, and negative steps
      * mean reverse.
      * 
      * @param steps The number of steps to move the motor. A positive value moves the motor forward, and a negative 
      * value moves it in reverse.
      */
     void moveTo(long steps);
 
     /**
      * @brief Moves the stepper motor until a limit switch is triggered.
      * 
      * Moves the motor based on the direction determined by the sign of `steps` until the limit switch is activated.
      * 
      * @param steps The number of steps to move the motor. A positive value moves the motor forward, and a negative 
      * value moves it in reverse.
      * @param limitSwitch A reference to the limit switch object to detect activation.
      */
     void moveToLimit(long steps, LimitSwitch& limitSwitch);
 
     /**
      * @brief Returns the current position of the stepper motor.
      * 
      * The position is measured in steps, relative to the initial position when the motor was homed.
      * 
      * @return The current position of the stepper motor.
      */
     long getPosition() const;
 
     /**
      * @brief Sets the pulse interval for the stepper motor.
      * 
      * This modifies the time between pulses, which affects the speed of the motor.
      * 
      * @param interval The time interval between pulses (in microseconds).
      */
     void setPulseInterval(int interval);
 
 private:
     byte pulPin;         ///< Pin used for pulse signal
     byte dirPin;         ///< Pin used for direction signal
     int pulseInterval;   ///< Time interval between pulses (controls motor speed)
     bool positiveDirection; ///< Boolean to set the motor's positive direction
     long currentPosition; ///< Current position of the motor, relative to the home position
 };
 
 #endif  // STEPPERCONTROLLER_H
 