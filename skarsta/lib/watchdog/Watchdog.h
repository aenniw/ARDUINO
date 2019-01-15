#ifndef ARDUINO_PROJECTS_ROOT_WATCHDOG_H
#define ARDUINO_PROJECTS_ROOT_WATCHDOG_H

#include <Service.h>
#include <Motor.h>
#include <Display.h>

#define WATCHDOG_TIMEOUT 200

class Watchdog : Service {
private:
    Motor *motor = nullptr;
    Display *display = nullptr;
public:
    Watchdog(Motor *m, Display *d);

    void cycle() override;
};


#endif //ARDUINO_PROJECTS_ROOT_WATCHDOG_H
