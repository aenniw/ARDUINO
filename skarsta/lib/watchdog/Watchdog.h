#ifndef ARDUINO_PROJECTS_ROOT_WATCHDOG_H
#define ARDUINO_PROJECTS_ROOT_WATCHDOG_H

#include <Service.h>
#include <Motor.h>
#include <Display.h>

#define WATCHDOG_TIMEOUT 250
#define WATCHDOG_DEADLOCK_CHANGE 2
#define WATCHDOG_OTHER_CHANGE 10
#define WATCHDOG_TOLERANCE 2

class Watchdog : Service {
private:
    Motor *motor = nullptr;
    Display *display = nullptr;
    uint8_t error_count = 0;
protected:
    void error(uint8_t cause);
public:
    Watchdog(Motor *m, Display *d);

    void cycle() override;
};


#endif //ARDUINO_PROJECTS_ROOT_WATCHDOG_H
