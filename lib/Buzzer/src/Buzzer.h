#ifndef BUZZER_H
#define BUZZER_H

#include <Arduino.h>

/**
 * @class Buzzer
 * @brief A simple class to control a digital buzzer using a GPIO pin.
 */
class Buzzer {
  public:
    /**
     * @brief Construct a new Buzzer object
     * 
     * @param pin The digital pin connected to the buzzer.
     */
    Buzzer(uint8_t pin);

    /**
     * @brief Initialize the buzzer pin (sets pinMode to OUTPUT and LOW).
     */
    void begin();

    /**
     * @brief Beep the buzzer multiple times.
     * 
     * @param times Number of beeps (default is 1).
     * @param duration Duration of each beep in milliseconds (default is 200 ms).
     * @param pause Pause between beeps in milliseconds (default is 200 ms).
     */
    void beep(uint8_t times = 1, uint16_t duration = 200, uint16_t pause = 200);

  private:
    uint8_t _pin; ///< GPIO pin used for the buzzer
};

#endif // BUZZER_H
