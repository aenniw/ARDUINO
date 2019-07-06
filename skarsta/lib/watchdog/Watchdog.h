#ifndef ARDUINO_PROJECTS_ROOT_WATCHDOG_H
#define ARDUINO_PROJECTS_ROOT_WATCHDOG_H

#include <Service.h>
#include <Motor.h>
#include <Display.h>

/**
 * WATCHDOG_TIMEOUT     represents timeout in millis for watchdog to perform its checks
 * WATCHDOG_TOLERANCE   represents number of failed check in row needed to trigger an error,
 *                      so the reaction time will be (WATCHDOG_TIMEOUT * WATCHDOG_TOLERANCE) ms
 *
 * WATCHDOG_DEADLOCK_CHANGE     represents rotation steps threshold for detecting if motor is stuck and cannot move
 *                              so if rotation change is greater then WATCHDOG_DEADLOCK_CHANGE for WATCHDOG_TOLERANCE times Error 1 will be trigerred
 * WATCHDOG_OTHER_CHANGE        represents rotation steps threshold for detecting if motor was started mainly due to programing errors,
 *                              so if rotation change is greater then WATCHDOG_OTHER_CHANGE for WATCHDOG_TOLERANCE times Error 2 will be trigerred
 */

#ifndef H_BRIDGE_MOTOR
#define WATCHDOG_TIMEOUT 100
#define WATCHDOG_DEADLOCK_CHANGE 2
#define WATCHDOG_OTHER_CHANGE 5
#define WATCHDOG_TOLERANCE 5
#else
#define WATCHDOG_TIMEOUT 100
#define WATCHDOG_DEADLOCK_CHANGE 2
#define WATCHDOG_OTHER_CHANGE 5
#define WATCHDOG_TOLERANCE 3
#endif

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
