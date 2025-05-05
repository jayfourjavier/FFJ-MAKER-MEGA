#ifndef EEPROM_STATUS_H
#define EEPROM_STATUS_H

#include <Arduino.h>
#include <EEPROM.h>

/**
 * @brief A simple EEPROM-based status flag handler for Arduino.
 * 
 * Stores a boolean status at a specified EEPROM address and retrieves it.
 */
class EEPROMStatus {
public:
    /**
     * @brief Construct a new EEPROMStatus object.
     * 
     * @param eepromAddress The EEPROM address where the boolean status is stored.
     */
    EEPROMStatus(int eepromAddress);

    /**
     * @brief Set the status value in EEPROM.
     * 
     * Stores 1 if `status` is true, otherwise stores 0.
     * 
     * @param status The boolean value to store.
     */
    void setStatus(bool status);

    /**
     * @brief Check if the stored value is positive (true).
     * 
     * @return true if stored value is 1, false otherwise.
     */
    bool isPositive();

private:
    int address; ///< The EEPROM address for storing the status.
};

#endif // EEPROM_STATUS_H
