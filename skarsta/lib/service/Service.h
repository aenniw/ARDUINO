#ifndef ARDUINO_PROJECTS_ROOT_SERVICE_H
#define ARDUINO_PROJECTS_ROOT_SERVICE_H

#ifdef __EEPROM__

#include <EEPROM.h>

#endif

#define DISPLAY_NONE "----"

class Service
{
  public:
    virtual void cycle() = 0;
};

unsigned long get_period(unsigned long last, unsigned long next);

#ifdef __EEPROM__

template <typename T>
const T &updateEEPROM(int idx, const T &t)
{
    T val;
    EEPROM.get(idx, val);
    if (val != t)
    {
        EEPROM.put(idx, t);
    }
    return t;
}

#endif

#endif //ARDUINO_PROJECTS_ROOT_SERVICE_H
