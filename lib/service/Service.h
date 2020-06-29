#ifndef ARDUINO_PROJECTS_ROOT_SERVICE_H
#define ARDUINO_PROJECTS_ROOT_SERVICE_H

#include <elapsedMillis.h>
#ifdef __EEPROM__
#include <EEPROM.h>
#endif

#define DISPLAY_NONE "----"

#ifdef __DEBUG__
void __log__(const __FlashStringHelper *fmt, ...);
void __log_init__(Print *stream);
#define LOG_INIT(f, s) f;__log_init__(s)
#define LOG(fmt, ...) __log__(F(fmt), ##__VA_ARGS__)
#else
#define LOG_INIT(...)
#define LOG(...)
#endif

class Service {
public:
    virtual bool begin() {
        return true;
    };
    virtual void cycle() {};
    virtual void disable() {};
    virtual void disable(uint8_t) {
        disable();
    };
};

class TimedService : public Service {
protected:
    elapsedMillis elapsed;
};

#ifdef __EEPROM__

template<typename T>
const T &updateEEPROM(int idx, const T &t) {
    T val;
    EEPROM.get(idx, val);
    if (val != t) {
        EEPROM.put(idx, t);
    }
    return t;
}

#endif

#endif //ARDUINO_PROJECTS_ROOT_SERVICE_H
