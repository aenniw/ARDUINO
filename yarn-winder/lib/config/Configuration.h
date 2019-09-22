#ifndef ARDUINO_PROJECTS_CONFIGURATION_H
#define ARDUINO_PROJECTS_CONFIGURATION_H

#ifdef __EEPROM__
#include <EEPROM.h>
#endif

typedef enum {
    Manual, Semi, Auto
} PROFILE;

typedef enum {
    EN, CS
} LOCALE;

#ifdef __EEPROM__

const int ADDRESS_PROFILE = EEPROM.begin();
const int ADDRESS_LOCALE = ADDRESS_PROFILE + sizeof(uint8_t);
const int ADDRESS_BRIGHTNESS = ADDRESS_LOCALE + sizeof(uint8_t);
const int ADDRESS_STALL = ADDRESS_BRIGHTNESS + sizeof(uint8_t);
const int ADDRESS_END = ADDRESS_STALL + sizeof(uint16_t);

template<typename T>
const T &eeprom_set(int idx, const T &t) {
    T val;
    EEPROM.get(idx, val);
    if (val != t) {
        EEPROM.put(idx, t);
    }
    return t;
}

#endif

#endif //ARDUINO_PROJECTS_CONFIGURATION_H
