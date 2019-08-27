#include <climits>
#include "MosfetMotor.h"

static MosfetMotor *motor;

static unsigned long get_period(const unsigned long last, const unsigned long next) {
    if (last > next) {
        return (ULONG_MAX - last) + next;
    }
    return next - last;
}

MosfetMotor::MosfetMotor(uint8_t pwm, uint8_t gate) {
    motor = this;

    pinMode((this->pwm = pwm), OUTPUT);
    pinMode((this->gate = gate), INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(gate), []() {
        if (motor->speed)
            motor->rotary_count++;
    }, IR_TRIGGER);
}

uint8_t MosfetMotor::get_speed() {
    if (!speed) {
        return 0;
    }
    const auto min = (float) (MIN_SPEED - MIN_STEP);
    return (uint8_t) (((speed - min) / (MAX_SPEED - min)) * 100);
}

void MosfetMotor::set_speed(uint8_t s) {
    analogWrite(pwm, (speed = s));
}

void MosfetMotor::reset() {
    set_speed(0);
    rotary_count = 0;
}

void MosfetMotor::cycle() {
    if (speed && rotary_count_end) {
        static unsigned long last_tick = millis();
        const unsigned long now = millis(), diff = get_period(last_tick, now),
                remaining_count = (rotary_count_end * EVOLUTION) - rotary_count;

        if (remaining_count <= EVOLUTION_OFFSET) {
            set_speed(0);
            rotary_count = rotary_count_end * EVOLUTION;
        } else if (remaining_count <= EVOLUTION) {
            set_speed(MIN_SPEED);
        } else if (remaining_count <= 2 * EVOLUTION && diff >= 25) {
            if (speed - MIN_STEP > MIN_SPEED)
                decrease_speed();
            last_tick = now;
        }
    }
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

MotorState MosfetMotor::get_state() {
    return speed ? ON : OFF;
}

unsigned long MosfetMotor::get_evolution() const {
    return rotary_count / EVOLUTION;
}

double MosfetMotor::get_len() {
    // TODO: update based on fitting curve
    return get_evolution() * 0.0965;
}

unsigned long *MosfetMotor::get_stop_evolution() {
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
