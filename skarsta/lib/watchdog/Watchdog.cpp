#include "Watchdog.h"
#include <util/atomic.h>

Watchdog::Watchdog(Motor *m, Display *d) {
    this->motor = m;
    this->display = d;
}

void Watchdog::cycle() {
    static unsigned long last_tick = millis();
    unsigned long now = millis(), diff = get_period(last_tick, now);

    if (diff >= WATCHDOG_TIMEOUT) {
        unsigned int pos_diff = motor->get_position_change();
        MotorState state = motor->get_state();

        if (pos_diff <= WATCHDOG_DEADLOCK_CHANGE && state != OFF) {
            error(1);
#ifdef __DEBUG__
            Serial.print("w-1 stop d:");
            Serial.println(pos_diff);
#endif
        } else if (pos_diff > WATCHDOG_OTHER_CHANGE && state == OFF) {
            error(2);
#ifdef __DEBUG__
            Serial.print("w-2 stop d:");
            Serial.println(pos_diff);
#endif
        } else {
            error_count = 0;
#ifdef __DEBUG__
            Serial.print("w d:");
            Serial.println(pos_diff);
#endif
        }

        last_tick = now;
    }
}

void Watchdog::error(uint8_t cause) {
    if (error_count++ < WATCHDOG_TOLERANCE - 1) {
#ifdef __DEBUG__
        Serial.print("w ec:");
        Serial.println(error_count);
#endif
        return;
    }
    motor->disable();
    display->disable(cause);
}
