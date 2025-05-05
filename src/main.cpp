#include <Arduino.h>
#include <HX711.h>       // For Load cell (not used in this code yet)
#include "LimitSwitch.h"
#include "RelayModule.h"
#include "StepperController.h"  // Assuming this is the correct library for controlling stepper motors
#include "MotorController.h"  // Assuming this is the correct library for controlling stepper motors
#include "Buzzer.h"  // Assuming this is the correct library for controlling stepper motors

Buzzer buzzer();
HX711 weighingScale;
const byte hx711DatPin = 2;
const byte hx711SckPin = 3;
float calibrationFactor = 13.40f;  // Adjust after calibration +-20grams margin of error

/**
 * @brief Initializes the HX711 weighing scale.
 * 
 * Sets the data and clock pins, applies the calibration factor,
 * and tares the scale to zero. Call this in setup().
 */
void setupWeighingScale() {
    Serial.println(F("[INFO] Initializing weighing scale..."));
    weighingScale.begin(hx711DatPin, hx711SckPin);
    weighingScale.set_scale(calibrationFactor);
    weighingScale.tare();  // Reset the scale to 0
    delay(2000);
    Serial.println(F("[INFO] Scale is tared. Ready to read weight."));
}

/**
 * @brief Gets the weight reading from the HX711 in grams.
 * 
 * Takes the average of 20 samples. Make sure the scale has been tared and calibrated.
 * 
 * @return float The measured weight in grams. Returns -1.0 if scale is not ready.
 */
float getWeight() {
    if (weighingScale.is_ready()) {
        float weightGrams = weighingScale.get_units(20);
        Serial.print(F("[DATA] Weight: "));
        Serial.print(weightGrams, 2);
        Serial.println(F(" g"));
        return weightGrams;
    } else {
        Serial.println(F("[ERROR] Weighing scale not detected."));
        return -1.0f;
    }
}

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
const byte pumpEnaPin = 5;
const byte pumpPwmPin = 6;
const byte pumpSpeed = 100;

const byte chopperEnaPin = 7;
const byte chopperPwmPin = 8;
const byte chopperSpeed = 50; // PWM value (0–255)


MotorController chopperMotor(chopperEnaPin, chopperPwmPin);
MotorController pumpMotor(pumpEnaPin, pumpPwmPin);

// ======================= Stepper Controller Instances =======================
StepperController sliderStepper(sliderPulPin, sliderDirPin, 1, false);  // 10ms interval, clockwise
StepperController sealerStepper(sealerPulPin, sealerDirPin, 3, true);
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

void setupLimitSwitches() {
    // Initialize limit switches
    sliderHomeSwitch.init();
    sealerDownSwitch.init();
    sealerUpSwitch.init();
    mixerDownSwitch.init();
    mixerUpSwitch.init();
    
    Serial.println("[Setup] Limit switches initialized.");
}


void setupStepperMotors() {
    // Initialize the stepper motors
    sliderStepper.init();
    sealerStepper.init();
    mixingToolStepper.init();
    mixerStepper.init();
    
    Serial.println("[Setup] Stepper motors initialized.");
}

void setupMotors() {
    // Initialize the motors
    pumpMotor.init();
    chopperMotor.init();
    
    Serial.println("[Setup] Motors initialized.");
}

void liftCover() {
    Serial.println("[Action] Lifting cover.");
    sealerStepper.setPulseInterval(1);
    sealerStepper.moveToLimit(10000, sealerUpSwitch);
    Serial.println("[Action] Cover lifted.");
    delay(2000);
}

void moveMixerUp() {
    Serial.println("[Action] Moving mixer up.");
    mixerStepper.moveToLimit(-40000, mixerUpSwitch);
    Serial.println("[Action] Mixer moved up.");
    delay(2000);
}

void resetSlider() {
    Serial.println("[Action] Resetting slider to home position.");
    liftCover();
    moveMixerUp();
    sliderStepper.moveToLimit(-58000, sliderHomeSwitch);
    Serial.println("[Action] Slider reset to home position.");
    delay(2000);
}

void moveMixerDown() {
    Serial.println("[Action] Moving mixer down.");
    mixerStepper.moveToLimit(40000, mixerDownSwitch);
    Serial.println("[Action] Mixer moved down.");
    delay(2000);
}

void putCover() {
    Serial.println("[Action] Putting cover down.");
    sealerStepper.moveToLimit(-10000, sealerDownSwitch);
    Serial.println("[Action] Cover put down.");
    delay(2000);
}

void moveSliderToMixer() {
    resetSlider();
    Serial.println("[Action] Moving to mixer position.");
    sliderStepper.moveTo(18000);
    Serial.println("[Action] Moved to mixer position.");
    delay(2000);
}

void stir() {
    Serial.println("[Action] Stirring.");
    mixingToolStepper.moveTo(10000);
    Serial.println("[Action] Stirring complete.");
    delay(2000);
}

void turnOnPump() {
    Serial.println("[Action] Turning on pump.");
    pumpMotor.turnOn(pumpSpeed);
    Serial.println("[Action] Pump turned on.");
}

void turnOffPump() {
    Serial.println("[Action] Turning off pump.");
    pumpMotor.turnOff();
    Serial.println("[Action] Pump turned off.");
}

void turnOnChopper() {
    Serial.println("[Action] Turning on chopper.");
    chopperMotor.turnOn(chopperSpeed);
    Serial.println("[Action] Chopper turned on.");
}

void turnOffChopper() {
    Serial.println("[Action] Turning off chopper.");
    chopperMotor.turnOff();
    Serial.println("[Action] Chopper turned off.");
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

void mixIngredients(){
    Serial.println("[Action] Mixing ingredients process started");
    moveSliderToMixer();
    delay(1000);
    moveMixerDown();
    delay(1000);
    stir();
    delay(1000);
    moveMixerUp();
    delay(1000);
    Serial.println("[Action] Mixing ingredients process is done");

}

void moveSliderToSealer(){
    resetSlider();
    Serial.println("[Action] Moving to sealer position.");
    sliderStepper.moveTo(57000);
    Serial.println("[Action] Moved to sealer position.");
    delay(2000);
}

void seal(){
    //resetSlider();
    Serial.println("[Action] Sealing process started.");
    moveSliderToSealer();
    putCover();
    Serial.println("[Action] Sealing process successful.");
}




void setup() {
    Serial.begin(9600);

    setupRelay();
    setupLimitSwitches();
    setupStepperMotors();
    setupMotors();
    setupWeighingScale();

    //turnOnCamera();
    powerUpMotors();

    pinMode(A15, OUTPUT);

    beepThreeTimes();

    turnOnPump();
    delay(10000);
    turnOffPump();
    




    //resetSlider();
    //mixIngredients();
    //seal();
}



void loop() {
    //getWeight();  
    delay(100);
}
  
