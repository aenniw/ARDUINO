#include "Watchdog.h"

SafetyTrigger::SafetyTrigger(Motor *m, Display *d, ErrorType t, uint8_t to) :
        motor(m), display(d), type(t), tolerance(to) {
}

bool SafetyTrigger::trip(ErrorType t) {
    if (type != t) return false;
    if (error_count++ < tolerance - 1) {
#ifdef __DEBUG__
        Serial.print(millis());
        Serial.print(F("\t| t | t:"));
        Serial.println(type);
        Serial.print(F(" ec:"));
        Serial.println(error_count);
#endif
        return false;
    }

    motor->disable();
    display->disable(t);
    return true;
}

void SafetyTrigger::reset() {
#ifdef __DEBUG__
    Serial.print(millis());
    Serial.print(F("\t| w | reset"));
#endif
    error_count = 0;
}

Watchdog::Watchdog(Motor *m, uint16_t ti, uint8_t dc, uint8_t oc) :
        motor(m), timeout(ti), deadlock_change(dc), other_change(oc) {
}

Watchdog &Watchdog::add_trigger(Trigger *t) {
    if (t) {
        this->triggers.push_back(t);
    }
    return *this;
}

void Watchdog::cycle() {
    if (elapsed >= timeout) {
        unsigned int pos_diff = motor->get_position_change();
        MotorState state = motor->get_state();

#ifdef __DEBUG__
        Serial.print(millis());
        Serial.print(F("\t| w | d:"));
        Serial.print(pos_diff);
        Serial.print(F(" s:"));
        Serial.println(state);
#endif
        if (pos_diff <= deadlock_change && state != OFF) {
            for (auto trigger: triggers)
                if (trigger->trip(STOPPED))
                    break;
        } else if (pos_diff > other_change && state == OFF) {
            for (auto trigger: triggers)
                if (trigger->trip(STARTED))
                    break;
        } else {
            for (auto trigger: triggers)
                trigger->reset();
        }

        elapsed = 0;
    }
}
