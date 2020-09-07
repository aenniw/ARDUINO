#include "Watchdog.h"

SafetyTrigger::SafetyTrigger(Motor *m, Display *d, ErrorType t, uint8_t to) :
        motor(m), display(d), type(t), tolerance(to) {
}

bool SafetyTrigger::trip(ErrorType t) {
    if (type != t) return false;
    if (error_count++ < tolerance - 1) {
        LOG("t | t:%d ec:%d", type, error_count);
        return false;
    }

    motor->disable();
    display->disable(t);
    return true;
}

void SafetyTrigger::reset() {
    LOG("t | reset");
    error_count = 0;
}

Watchdog::Watchdog(Motor *m, uint16_t ti, uint8_t dc, uint8_t oc, uint16_t os) :
        motor(m), timeout(ti), other_sleep(os), deadlock_change(dc), other_change(oc) {
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

        if (state != OFF && pos_diff) {
            lastRotation = 0;
        }

        LOG("w | d:%d s:%d", pos_diff, state);
        if (pos_diff <= deadlock_change && state != OFF) {
            for (auto trigger: triggers)
                if (trigger->trip(STOPPED))
                    break;
        } else if (pos_diff > other_change && state == OFF) {
            if (other_sleep && lastRotation >= other_sleep) {
                LOG("w | other - sleep");
                lastRotation = other_sleep;
            } else {
                for (auto trigger: triggers)
                    if (trigger->trip(STARTED))
                        break;
            }
        } else {
            for (auto trigger: triggers)
                trigger->reset();
        }

        elapsed = 0;
    }
}
