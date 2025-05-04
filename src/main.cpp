#include <Arduino.h>
#include <HX711.h>       // For Load cell (not used in this code yet)
#include "LimitSwitch.h"
#include "RelayModule.h"
#include "StepperController.h"  // Assuming this is the correct library for controlling stepper motors



// ======================= Pin Definitions for Chopper Motor =======================
const byte chopperRen = 28;
const byte chopperRpwm = 29;
const byte chopperLen = 26;
const byte chopperLpwm = 27;


// ======================= Stepper + Limit Pins =======================
const byte sliderPulPin = 52, sliderDirPin = 53;
const byte sealerPulPin = 46, sealerDirPin = 47;
const byte mixingPulPin = 49, mixingDirPin = 48;
const byte mixerPulPin  = 50, mixerDirPin  = 51;

const byte sliderHomePin = 9;
const byte sealerDownPin = 10;
const byte sealerUpPin = 11;
const byte mixerDownPin = 12;
const byte mixerUpPin = 13;

const byte cameraRelayPin = 44;
const byte motorRelayPin = 45;

RelayModule camera(cameraRelayPin);
RelayModule motors(motorRelayPin);



// ======================= Limit Switch Instances =======================
LimitSwitch sliderHomeSwitch(sliderHomePin);
LimitSwitch sealerDownSwitch(sealerDownPin);
LimitSwitch sealerUpSwitch(sealerUpPin);
LimitSwitch mixerDownSwitch(mixerDownPin);
LimitSwitch mixerUpSwitch(mixerUpPin);

// ======================= Pump Control =======================
const byte pumpEnaPin = 7;
const byte pumpPwmPin = 8;
byte speed = 200; // PWM value (0–255)

// ======================= Stepper Controller Instances =======================
StepperController sliderStepper(sliderPulPin, sliderDirPin, 1, false);  // 10ms interval, clockwise
StepperController sealerStepper(sealerPulPin, sealerDirPin, 1, true);
StepperController mixingToolStepper(mixingPulPin, mixingDirPin, 1, true);
StepperController mixerStepper(mixerPulPin, mixerDirPin, 1, true);

void turnOnCamera(){
    Serial.println("Turning on camera");
    camera.turnOn();
    delay(1000);
}

void powerUpMotors(){
    Serial.println("Turning on motor power supply");
    motors.turnOn();
    delay(1000);
}

void setupRelay(){
    camera.init();
    motors.init();
    Serial.println("[Setup] Relays initialized.");
}

void setupLimitSwitches(){
    // Initialize limit switches
    sliderHomeSwitch.init();
    sealerDownSwitch.init();
    sealerUpSwitch.init();
    mixerDownSwitch.init();
    mixerUpSwitch.init();
    Serial.println("[Setup] Limit switches initialized.");
}

void setupStepperMotors(){
    
    // Initialize the stepper motors
    sliderStepper.init();
    sealerStepper.init();
    mixingToolStepper.init();
    mixerStepper.init();
    Serial.println("[Setup] Stepper motors initialized.");

}

void resetSlider(){
    sliderStepper.moveToLimit(-10000, sliderHomeSwitch);
}

void moveMixerDown(){
    mixerStepper.moveToLimit(40000, mixerDownSwitch);
}

void moveMixerUp(){
    mixerStepper.moveToLimit(-40000, mixerUpSwitch);
}

void liftCover(){
    sealerStepper.setPulseInterval(1);
    sealerStepper.moveToLimit(10000, sealerUpSwitch);
}

void putCover(){
    sealerStepper.setPulseInterval(3);
    sealerStepper.moveToLimit(-10000, sealerDownSwitch);
}

void moveToMixer(){
    sliderStepper.moveTo(19000);

}

void stir(){
    mixingToolStepper.moveTo(10000);
}

void testLimitSwitch(){
    // Read states (1 = triggered, 0 = not triggered)
    byte s1 = sliderHomeSwitch.isTriggered();
    byte s2 = sealerDownSwitch.isTriggered();
    byte s3 = sealerUpSwitch.isTriggered();
    byte s4 = mixerDownSwitch.isTriggered();
    byte s5 = mixerUpSwitch.isTriggered();

    // Print as a 5-digit binary number
    Serial.print("Limit Switch States: ");
    Serial.print(s1);
    Serial.print(s2);
    Serial.print(s3);
    Serial.print(s4);
    Serial.println(s5);

    delay(500); // Adjust delay as needed
}

void setup() {
    Serial.begin(9600);
    setupRelay();
    setupLimitSwitches();
    setupStepperMotors();


    //turnOnCamera();
    powerUpMotors();

   //sliderStepper.moveTo(100);
    //sliderStepper.moveToLimit(false, sliderHomeSwitch);
    //moveMixerDown();
    //moveMixerUp();
    //mixingToolStepper.moveTo(30000);
    //liftCover();
    //delay(3000);
    //putCover();
    //resetSlider();
    //delay(3000);
    //moveToMixer();
    //stir();

    pinMode(pumpEnaPin, OUTPUT);
    pinMode(pumpPwmPin, OUTPUT);

    digitalWrite(pumpEnaPin, LOW);
    analogWrite(pumpPwmPin, 150);


}

void loop() {
    delay(100);
    //Serial.println(mixerDownSwitch.isTriggered());
}
