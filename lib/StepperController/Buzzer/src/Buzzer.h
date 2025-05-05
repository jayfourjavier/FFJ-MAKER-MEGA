#ifndef BUZZER_H
#define BUZZER_H

#include <Arduino.h>

class Buzzer {
  public:
    Buzzer(uint8_t pin);
    void begin();
    void beep(uint8_t times = 1, uint16_t duration = 200, uint16_t pause = 200);

  private:
    uint8_t _pin;
};

#endif // BUZZER_H
