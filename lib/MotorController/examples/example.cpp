#include <Arduino.h>
#include "MotorController.h"

MotorController motor(9, 10);  // Example with pins 9 for enable and 10 for PWM

void setup() {
    motor.init();  // Initialize the motor control pins
    Serial.begin(9600);  // Start serial communication
    Serial.println("Motor Controller Example");

    motor.turnOn(75);  // Turn on the motor at 75% speed
    delay(5000);  // Run motor for 5 seconds
    motor.turnOff();  // Turn off the motor
}

void loop() {
    // Your loop code
    // To turn on the motor at 50% speed:
    // motor.turnOn(50);
    // To turn off the motor:
    // motor.turnOff();
}
