#ifndef ARDUINO_PROJECTS_CONFIGURATION_H
#define ARDUINO_PROJECTS_CONFIGURATION_H

#ifdef __EEPROM__
#include <EEPROM.h>
#endif

typedef enum {
    Manual, Auto
} PROFILE;


typedef enum {
    EN, CS
} LOCALE;

#ifdef __EEPROM__

const int ADDRESS_PROFILE = EEPROM.begin();
const int ADDRESS_LOCALE = ADDRESS_PROFILE + sizeof(uint8_t);
const int ADDRESS_BRIGHTNESS = ADDRESS_LOCALE + sizeof(uint8_t);
const int ADDRESS_END = ADDRESS_BRIGHTNESS + sizeof(uint8_t);

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

class Configuration {
private:
    PROFILE profile = Manual;
    LOCALE locale = EN;

    Configuration();

public:
    PROFILE get_profile() const;

    void set_profile(PROFILE profile);

    LOCALE get_locale() const;

    void set_locale(LOCALE locale);

    static Configuration *get_instance();
};


#endif //ARDUINO_PROJECTS_CONFIGURATION_H
