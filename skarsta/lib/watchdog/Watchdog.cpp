#include "Watchdog.h"
#include <util/atomic.h>

Watchdog::Watchdog(Motor *m, Display *d, uint16_t ti, uint8_t dc, uint8_t oc, uint8_t to) :
        motor(m), display(d), timeout(ti),
        deadlock_change(dc), other_change(oc), tolerance(to) {
}

void Watchdog::cycle() {
    static unsigned long last_tick = millis();
    unsigned long now = millis(), diff = get_period(last_tick, now);

    if (diff >= timeout) {
        unsigned int pos_diff = motor->get_position_change();
        MotorState state = motor->get_state();

        if (pos_diff <= deadlock_change && state != OFF) {
            error(1);
#ifdef __DEBUG__
            Serial.print("w-1 stop d:");
            Serial.println(pos_diff);
#endif
        } else if (pos_diff > other_change && state == OFF) {
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
    if (error_count++ < tolerance - 1) {
#ifdef __DEBUG__
        Serial.print("w ec:");
        Serial.println(error_count);
#endif
        return;
    }
    motor->disable();
    display->disable(cause);
}
