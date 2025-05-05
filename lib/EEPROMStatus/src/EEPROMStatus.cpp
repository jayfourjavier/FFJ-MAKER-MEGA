/**
 * @file EEPROMStatus.cpp
 * @brief Implementation of the EEPROMStatus class for managing a boolean flag in EEPROM.
 */

 #include "EEPROMStatus.h"

 /**
  * @brief Constructs an EEPROMStatus object at the specified EEPROM address.
  * 
  * @param eepromAddress The EEPROM address where the boolean flag is stored.
  */
 EEPROMStatus::EEPROMStatus(int eepromAddress) {
     address = eepromAddress;
 }
 
 /**
  * @brief Stores a boolean status in EEPROM.
  * 
  * This function stores a 1 if `status` is true, and a 0 if false.
  * Uses EEPROM.update() to reduce EEPROM wear.
  * 
  * @param status Boolean value to store.
  */
 void EEPROMStatus::setStatus(bool status) {
     EEPROM.update(address, status ? 1 : 0);
 }
 
 /**
  * @brief Reads the stored value and returns it as a boolean.
  * 
  * @return true if the stored value is 1, false if 0 or any other value.
  */
 bool EEPROMStatus::isPositive() {
     return EEPROM.read(address) == 1;
 }
 