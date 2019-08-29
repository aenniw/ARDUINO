#include <climits>
#include "MosfetMotor.h"

static MosfetMotor *motor;

static unsigned long get_period(const unsigned long last, const unsigned long next) {
    if (last > next) {
        return (ULONG_MAX - last) + next;
    }
    return next - last;
}

MosfetMotor::MosfetMotor(PROFILE *profile, uint8_t pwm, uint8_t gate) {
    motor = this;

    pinMode((this->pwm = pwm), OUTPUT);
    pinMode((this->gate = gate), INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(gate), []() {
        if (motor->is_rotating())
            motor->rotary_count++;
    }, IR_TRIGGER);
    this->profile = profile;
}

uint8_t MosfetMotor::get_speed() const {
    if (get_state() == OFF) {
        return 0;
    }
    const auto min = (float) (MIN_SPEED - MIN_STEP);
    return (uint8_t) (((speed - min) / (MAX_SPEED - min)) * 100);
}

void MosfetMotor::set_speed(uint8_t s) {
    if (s) {
        no_spin = false;
    }
    analogWrite(pwm, (speed = s));
}

void MosfetMotor::reset() {
    set_speed(0);
    rotary_count = 0;
}

MosfetMotor::~MosfetMotor() {
    digitalWrite(pwm, 0);
    detachInterrupt(digitalPinToInterrupt(gate));
}

void MosfetMotor::toggle() {
    if (get_state() == ON) {
        motor->set_speed(0);
    } else {
        motor->set_speed(MIN_SPEED);
    }
}

void MosfetMotor::increase_speed() {
    if (speed >= MIN_SPEED && speed + MIN_STEP <= MAX_SPEED) {
        motor->set_speed(speed + MIN_STEP);
    }
}

void MosfetMotor::decrease_speed() {
    if (speed > 0) {
        if (speed - MIN_STEP > MIN_SPEED) {
            motor->set_speed(speed - MIN_STEP);
        } else {
            motor->set_speed(0);
        }
    }
}

MotorState MosfetMotor::get_state() const {
    return speed ? ON : OFF;
}

unsigned long MosfetMotor::get_evolution() const {
    return rotary_count / EVOLUTION;
}

unsigned long MosfetMotor::get_remaining_evolutions() const {
    if (!rotary_count_end || get_evolution() >= rotary_count_end) {
        return 0;
    }
    return (rotary_count_end * EVOLUTION) - rotary_count;
}

double MosfetMotor::get_len() const {
    if (!rotary_count) {
        return 0;
    }

    const double evolution = rotary_count / (double) EVOLUTION;
    return 0.1502201 + 0.06363677 * evolution + 0.001051915 * evolution * evolution;
}

unsigned long *MosfetMotor::get_stop_evolution() const {
    return &rotary_count_end;
}

void MosfetMotor::decrease_stop_evolution() {
    rotary_count = 0;
    if (rotary_count_end > 0) {
        rotary_count_end = rotary_count_end - 1;
    }
}

void MosfetMotor::increase_stop_evolution() {
    if (rotary_count_end < ULONG_MAX) {
        rotary_count_end = rotary_count_end + 1;
    }
}

bool MosfetMotor::is_rotating() const {
    return !no_spin;
}

void MosfetMotor::spin_detect(unsigned long ms) {
    static unsigned long last_rotary_count = rotary_count, last_spin_tick = millis();
    const unsigned long diff = get_period(last_spin_tick, ms);

    if (diff >= SPIN_TIMEOUT) {
        if (last_rotary_count == rotary_count) {
            no_spin = true;
        }
        last_spin_tick = ms;
        last_rotary_count = rotary_count;
    }
}

void MosfetMotor::spin_down(unsigned long ms) {
    static unsigned long last_slow_tick = millis();
    const unsigned long diff = get_period(last_slow_tick, ms),
            remaining_count = get_remaining_evolutions();

    if (remaining_count <= EVOLUTION_OFFSET) {
        set_speed(0);
        rotary_count = rotary_count_end * EVOLUTION;
    } else if (remaining_count <= EVOLUTION) {
        set_speed(MIN_SPEED); // prevents from manual speed increase
    } else if (remaining_count <= 2 * EVOLUTION && diff >= 20) {
        if (speed - MIN_STEP > MIN_SPEED) {
            decrease_speed();
        }
        last_slow_tick = ms;
    }
}

void MosfetMotor::spin_up(unsigned long ms) {
    static unsigned long last_inc_tick = millis();
    const unsigned long diff = get_period(last_inc_tick, ms),
            remaining_count = get_remaining_evolutions();

    if (remaining_count > 2 * EVOLUTION && diff >= SPINUP_TIMEOUT) {
        increase_speed();
        last_inc_tick = ms;
    }

}

void MosfetMotor::cycle() {
    const unsigned long now = millis();

    if (get_state() == OFF) {
        spin_detect(now);
    } else if (get_state() == ON) {
        if (stall_detection) {
            stall_detect(now);
        }

        if (rotary_count_end) {
            switch (*profile) {
                case Auto:
                    spin_up(now);
                case Semi:
                    spin_down(now);
            }
        }
    }
}

void MosfetMotor::stall_detect(unsigned long ms) {
    static unsigned long last_rotary_count = rotary_count, last_spin_tick = millis();
    const unsigned long diff = get_period(last_spin_tick, ms);

    if (diff >= STALL_TIMEOUT) {
        if (last_rotary_count == rotary_count) {
            last_rotary_count = 0;
            set_speed(0);
        } else {
            last_rotary_count = rotary_count;
        }
        last_spin_tick = ms;
    }
}
