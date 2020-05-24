#include "Watchdog.h"
#include <util/atomic.h>

Watchdog::Watchdog(Motor *m, Display *d, uint16_t ti, uint8_t dc, uint8_t oc, uint8_t to) :
        motor(m), display(d), timeout(ti),
        deadlock_change(dc), other_change(oc), tolerance(to) {
}

void Watchdog::cycle() {
    if (elapsed >= timeout) {
        unsigned int pos_diff = motor->get_position_change();
        MotorState state = motor->get_state();

        if (pos_diff <= deadlock_change && state != OFF) {
#ifdef __DEBUG__
            Serial.print(millis());
            Serial.print(F("\t| w | e:"));
            Serial.print(ERROR_STALL);
            Serial.print(F(" stop d:"));
            Serial.print(pos_diff);
            Serial.print(F(" w ec:"));
            Serial.println(error_count);
#endif
            error(ERROR_STALL);
        } else if (pos_diff > other_change && state == OFF) {

#ifdef __DEBUG__
            Serial.print(millis());
            Serial.print(F("\t| w | e:"));
            Serial.print(ERROR_USTART);
            Serial.print(F(" stop d:"));
            Serial.print(pos_diff);
            Serial.print(F(" w ec:"));
            Serial.println(error_count);
#endif
            error(ERROR_USTART);
        } else {
            error_count = 0;
#ifdef __DEBUG__
            if ((pos_diff && state != OFF)) {
                Serial.print(millis());
                Serial.print(F("\t| w | d:"));
                Serial.print(pos_diff);
                Serial.println(F(" reset"));
            }
#endif
        }

        elapsed = 0;
    }
}

void Watchdog::error(uint8_t cause) {
    if (error_count++ < tolerance - 1) {
        return;
    }
    motor->disable();
    display->disable(cause);
}
