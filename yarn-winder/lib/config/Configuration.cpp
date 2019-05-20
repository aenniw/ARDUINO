#include "Configuration.h"

#ifdef __EEPROM__

static bool eeprom_valid() {
    for (int i = ADDRESS_END; i < EEPROM.length(); ++i) {
        if (EEPROM[i] != 0) {
            return false;
        }
    }
    return true;
}

static void eeprom_reset() {
    for (int i = 0; i < EEPROM.length(); ++i)
        eeprom_set(i, 0);
}

#endif

PROFILE Configuration::get_profile() const {
    return profile;
}

void Configuration::set_profile(PROFILE profile) {
    this->profile = profile;
#ifdef __EEPROM__
    eeprom_set(ADDRESS_PROFILE, (uint8_t) this->profile);
#endif
}

LOCALE Configuration::get_locale() const {
    return locale;
}

void Configuration::set_locale(LOCALE locale) {
    this->locale = locale;
#ifdef __EEPROM__
    eeprom_set(ADDRESS_LOCALE, (uint8_t) this->locale);
#endif
}

Configuration *Configuration::get_instance() {
    static auto singleton = new Configuration();
    return singleton;
}

Configuration::Configuration() {
#ifdef __EEPROM__
    if (!eeprom_valid) {
        eeprom_reset();
    }

    uint8_t data = 0;
    EEPROM.get(ADDRESS_PROFILE, data);
    this->profile = (PROFILE) data;
    EEPROM.get(ADDRESS_LOCALE, data);
    this->locale = (LOCALE) data;
#endif
}
