# Motor Controller Library

This project provides a simple library to control a motor using an enable pin and a PWM pin, typically used for controlling pumps, fans, or other DC motors.

## Features

- **Turn On the Motor**: Control motor speed with a percentage value (0-100%).
- **Turn Off the Motor**: Disable the motor.
- **Speed Mapping**: The input speed (0-100%) is mapped to the PWM range (0-255).

## Files

- **MotorController.h**: Header file for the `MotorController` class.
- **MotorController.cpp**: Implementation of the `MotorController` class.
- **Example.cpp**: Example of how to use the `MotorController` class in your Arduino project.

## Setup

1. Download the `MotorController.h` and `MotorController.cpp` files and include them in your Arduino project.
2. In your sketch, include the header file:
    ```cpp
    #include "MotorController.h"
    ```

3. Initialize the motor controller with the appropriate pins:
    ```cpp
    MotorController motor(9, 10);  // Use pin 9 for enabling and pin 10 for PWM control
    ```

4. In the `setup()` function, call `motor.init()` to initialize the pins. Then, use `motor.turnOn(speed)` to set the motor speed (0-100%).

5. To turn off the motor, call `motor.turnOff()`.

## Example

```cpp
#include "MotorController.h"

MotorController motor(9, 10);

void setup() {
    motor.init();  // Initialize the motor control pins
    motor.turnOn(75);  // Turn on the motor at 75% speed
    delay(5000);  // Run motor for 5 seconds
    motor.turnOff();  // Turn off the motor
}

void loop() {
    // Loop logic
}
