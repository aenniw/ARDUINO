#ifndef ARDUINO_PROJECTS_ROOT_WATCHDOG_H
#define ARDUINO_PROJECTS_ROOT_WATCHDOG_H

#include <vector>
#include <Service.h>
#include <Motor.h>
#include <Display.h>
#include <Triggers.h>

class SafetyTrigger : public Trigger {
private:
    Motor *motor = nullptr;
    Display *display = nullptr;
    uint8_t error_count = 0;

    const ErrorType type;
    const uint8_t tolerance;
public:
    SafetyTrigger(Motor *m, Display *d, ErrorType t, uint8_t tolerance);

    bool trip(ErrorType t) override;

    void reset() override;
};

class Watchdog : public TimedService {
private:
    Motor *motor = nullptr;
    std::vector<Trigger *> triggers;

    const uint16_t timeout;
    const uint8_t deadlock_change, other_change;
public:
    Watchdog(Motor *m, uint16_t timeout, uint8_t deadlock_change, uint8_t other_change);

    Watchdog &add_trigger(Trigger *t);

    void cycle() override;
};


#endif //ARDUINO_PROJECTS_ROOT_WATCHDOG_H
