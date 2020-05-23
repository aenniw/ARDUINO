#ifndef ARDUINO_PROJECTS_ROOT_WATCHDOG_H
#define ARDUINO_PROJECTS_ROOT_WATCHDOG_H

#include <Service.h>
#include <Motor.h>
#include <Display.h>

#define ERROR_STALL     1
#define ERROR_USTART    2
#define ERROR_INIT      4

class Watchdog : TimedService {
private:
    Motor *motor = nullptr;
    Display *display = nullptr;
    uint8_t error_count = 0;

    const uint16_t timeout;
    const uint8_t deadlock_change, other_change, tolerance;
protected:
    void error(uint8_t cause);

public:
    Watchdog(Motor *m, Display *d, uint16_t timeout,
        uint8_t deadlock_change, uint8_t other_change, uint8_t tolerance);

    void cycle() override;
};


#endif //ARDUINO_PROJECTS_ROOT_WATCHDOG_H
