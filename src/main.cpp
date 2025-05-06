#include <Arduino.h>
#include <HX711.h>       // For Load cell (not used in this code yet)
#include "LimitSwitch.h"
#include "RelayModule.h"
#include "StepperController.h"  
#include "MotorController.h"  
#include "Buzzer.h"  
#include "EEPROMStatus.h"  
#include <EEPROM.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0X20,16, 2);

/**
 * @class TimerHelper
 * @brief A simple one-shot timer that executes a callback function after a specified number of seconds.
 */
class TimerHelper {
    private:
        unsigned long startTime = 0;          ///< Stores the start time in milliseconds.
        unsigned long durationMs = 0;         ///< Duration in milliseconds.
        bool isRunning = false;               ///< Flag to track if the timer is currently active.
        void (*callback)() = nullptr;         ///< Pointer to the callback function to execute when the timer expires.
    
    public:
        /**
         * @brief Construct a new TimerHelper object.
         */
        TimerHelper() {}
    
        /**
         * @brief Start the timer with a given duration and a function to execute.
         * 
         * @param durationSeconds Duration of the timer in seconds.
         * @param _callback Function to execute after the timer ends.
         */
        void timerStart(unsigned int durationSeconds, void (*_callback)()) {
            startTime = millis();
            durationMs = durationSeconds * 1000UL;
            callback = _callback;
            isRunning = true;
        }
    
        /**
         * @brief Call this in the main loop to check if the timer has expired and execute the callback.
         */
        void timerLoop() {
            if (isRunning && (millis() - startTime >= durationMs)) {
                isRunning = false;
                if (callback != nullptr) {
                    callback();
                }
            }
        }
    
        /**
         * @brief Cancel the timer before it completes.
         */
        void timerCancel() {
            isRunning = false;
        }
    
        /**
         * @brief Check if the timer is currently active.
         * 
         * @return true If the timer is running.
         * @return false If the timer is not running.
         */
        bool isActive() const {
            return isRunning;
        }
};

TimerHelper cameraTimer;


bool hasStarted = false;
bool isSliderAtHome = false;
bool isCameraRunning = false;
const byte cameraWebserverDuration = 5;



EEPROMStatus fermenting (0);

Buzzer buzzer(A15);
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


/**
 * @brief Plays a short startup sequence (5 quick beeps).
 */
void beepStartSequence() {
    buzzer.beep(5, 100, 100);
    delay(1000);
}

/**
 * @brief Plays an end sequence: long-short pattern (1 long + 3 short + 1 long).
 */
void beepEndSequence() {
    buzzer.beep(1, 1000, 300);
    buzzer.beep(3, 200, 150);
    buzzer.beep(1, 1000, 0);
    delay(1000);

}

/**
 * @brief Plays a long triple beep for camera triggering or alert.
 */
void beepCamera() {
    buzzer.beep(3, 2000, 500);
    delay(1000);
}

/**
 * @brief Plays a custom "power on" pattern: rising rhythm (short > medium > long).
 */
void powerOnBeep() {
    buzzer.beep(1, 100, 100);   // short
    buzzer.beep(1, 300, 150);   // medium
    buzzer.beep(1, 600);        // long
    delay(1000);
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

const byte startButtonPin = A0;     //3
const byte resetButtonPin = A1;     //1
const byte cameraButtonPin = A2;    //2


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
LimitSwitch startButton(startButtonPin);
LimitSwitch resetButton(resetButtonPin);
LimitSwitch cameraButton(cameraButtonPin);

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
    delay(3000);
    beepCamera();
}

void turnOffCamera(){
    Serial.println("Shutting down camera");
    camera.turnOff();
    delay(3000);
    beepCamera();
}

void powerUpMotors(){
    Serial.println("Turning on motor power supply");
    motors.turnOn();
    delay(1000);
}

void shutdownMotors(){
    Serial.println("Turning off motor power supply");
    motors.turnOff();
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
    startButton.init();
    resetButton.init();
    cameraButton.init();
    
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

void setupBuzzer(){
    buzzer.begin();
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
    beepStartSequence();
    Serial.println("[Action] Resetting slider to home position.");
    liftCover();
    moveMixerUp();
    sliderStepper.moveToLimit(-58000, sliderHomeSwitch);
    Serial.println("[Action] Slider reset to home position.");
    beepEndSequence();
    delay(2000);

}

void moveMixerDown() {
    beepStartSequence();
    Serial.println("[Action] Moving mixer down.");
    mixerStepper.moveToLimit(40000, mixerDownSwitch);
    Serial.println("[Action] Mixer moved down.");
    beepEndSequence();
    delay(2000);
}

void putCover() {
    beepStartSequence();
    Serial.println("[Action] Putting cover down.");
    sealerStepper.moveToLimit(-10000, sealerDownSwitch);
    Serial.println("[Action] Cover put down.");
    delay(2000);
    beepEndSequence();
}

void moveSliderToMixer() {
    beepStartSequence();
    resetSlider();
    Serial.println("[Action] Moving to mixer position.");
    sliderStepper.moveTo(18000);
    Serial.println("[Action] Moved to mixer position.");
    beepEndSequence();
    delay(2000);
}

void stir() {
    beepStartSequence();
    Serial.println("[Action] Stirring.");
    mixingToolStepper.moveTo(10000);
    Serial.println("[Action] Stirring complete.");
    beepEndSequence();
    delay(2000);
}

void turnOnPump() {
    buzzer.beep(1, 3000, 500);
    Serial.println("[Action] Turning on pump.");
    pumpMotor.turnOn(pumpSpeed);
    Serial.println("[Action] Pump turned on.");
}

void turnOffPump() {
    Serial.println("[Action] Turning off pump.");
    pumpMotor.turnOff();
    Serial.println("[Action] Pump turned off.");
    buzzer.beep(1, 3000, 500);

}

void turnOnChopper() {
    buzzer.beep(1, 3000, 500);
    Serial.println("[Action] Turning on chopper.");
    chopperMotor.turnOn(chopperSpeed);
    Serial.println("[Action] Chopper turned on.");
}

void turnOffChopper() {
    Serial.println("[Action] Turning off chopper.");
    chopperMotor.turnOff();
    Serial.println("[Action] Chopper turned off.");
    buzzer.beep(1, 3000, 500);
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
    beepStartSequence();
    Serial.println("[Action] Mixing ingredients process started");
    moveSliderToMixer();
    delay(1000);
    moveMixerDown();
    delay(1000);
    stir();
    delay(1000);
    moveMixerUp();
    Serial.println("[Action] Mixing ingredients process is done");
    beepEndSequence();
    delay(2000);

}

void moveSliderToSealer(){
    beepStartSequence();
    resetSlider();
    Serial.println("[Action] Moving to sealer position.");
    sliderStepper.moveTo(57000);
    Serial.println("[Action] Moved to sealer position.");
    beepEndSequence();
    delay(2000);
}

void seal(){
    //resetSlider();
    beepStartSequence();
    Serial.println("[Action] Sealing process started.");
    moveSliderToSealer();
    putCover();
    Serial.println("[Action] Sealing process successful.");
    beepEndSequence();
    delay(2000);
}




bool checkLcd() {
    bool _isDetected = false;
    byte error, address;
  
    for (address = 1; address < 127; address++) {
      Wire.beginTransmission(address);
      error = Wire.endTransmission();
  
      if (error == 0) {
        Serial.print("I2C device found at address 0x");
        if (address < 16)
          Serial.print("0");
        Serial.println(address, HEX);
  
        if (address == 0x20) {
          Serial.println("✅ LCD at 0x20 is detected.");
          _isDetected = true;
        }
      }
    }
  
    if (!_isDetected) {
      Serial.println("❌ LCD not found at 0x20.");
    }
  
    delay(1000);
    return _isDetected;
  }

/**
 * @brief Displays two lines of text centered on a 16x2 LCD.
 * 
 * @param line1 The first line of text.
 * @param line2 The second line of text.
 * @param autoClear Optional. If true, clears the display after 2 seconds. Default is false.
 */
void lcdPrint(String line1, String line2, bool autoClear = false) {
    lcd.clear();

    int len1 = line1.length();
    int len2 = line2.length();
    int pad1 = max((16 - len1) / 2, 0);
    int pad2 = max((16 - len2) / 2, 0);

    lcd.setCursor(pad1, 0);
    lcd.print(line1);
    lcd.setCursor(pad2, 1);
    lcd.print(line2);

    if (autoClear) {
        delay(2000);
        lcd.clear();
    }
}

  
void setupLcd() {
    lcd.init();
    lcd.backlight();

    for (int i = 0; i < 3; i++) {
        lcdPrint("WELCOME TO", "AUTO FFJ", true);
        delay(1000);
    }
}
  
  

void controller(){
    if (!fermenting.isPositive()){
        if (startButton.isPressed() && !hasStarted){
            Serial.println("Start button is pressed starting machine now");
            hasStarted = true; 
        } else {
            Serial.println("AUTOFFJ is ready. Press \"START\" button to begin.");
        }

        if (hasStarted){
            if(!isSliderAtHome){
                resetSlider();
            }
        }
    } else {
        Serial.println("AUTOFFJ is already fermenting");
    }


}

void setup() {
    Serial.begin(9600);
    Wire.begin();

    powerOnBeep();
    setupRelay();
    setupLimitSwitches();
    setupStepperMotors();
    setupMotors();
    setupWeighingScale();
    setupBuzzer();
    setupLcd();

    //turnOnCamera();
    //powerUpMotors();
    //liftCover();

    //turnOnPump();
    //delay(5000);
    //turnOffPump();

    //turnOnChopper();
    //delay(5000);
    //turnOffChopper();
    //fermenting.setStatus(false);



    
    //resetSlider();
    /*
    mixIngredients();
    seal();
    turnOnCamera();
    delay(3000);
    turnOffCamera();
    shutdownMotors();
    */



    
}

void loopCamera(){
    //Camera turning on or off, 3 long buzzer beeps
    if (cameraButton.isPressed()){
        if(!isCameraRunning){
            Serial.println("Camera button is pressed");
            lcdPrint("Camera webserver", "is now running.", true);
            turnOnCamera();
            cameraTimer.timerStart(cameraWebserverDuration * 60, turnOffCamera);
            Serial.println("Camera webserver is turned on for 300 seconds");
            isCameraRunning = true;

        } else {
            Serial.println("Camera button is pressed");
            lcdPrint("Camera webserver", "is closed.", true);
            turnOffCamera();
            cameraTimer.timerCancel();
            isCameraRunning = false;
        }  
    }
    cameraTimer.timerLoop(); //automatically turn off camera for specified time to save power and avoid overheating of flash light.
}


void loop() {

    loopCamera();
    
    //getWeight();  
    //controller();
    //checkLcd();

    Serial.print(startButton.isPressed());
    Serial.print(resetButton.isPressed());
    Serial.println(cameraButton.isPressed());
    delay(100);


}
  
