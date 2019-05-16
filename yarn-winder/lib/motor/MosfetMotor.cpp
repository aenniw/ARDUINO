#include "MosfetMotor.h"

static MosfetMotor *motor;

MosfetMotor::MosfetMotor(uint8_t pwm, uint8_t gate) {
    motor = this;

    pinMode((this->pwm = pwm), OUTPUT);
    pinMode((this->gate = gate), INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(gate), []() {
        motor->rotary_count++;
    }, CHANGE);
}

uint8_t MosfetMotor::get_speed() {
    return (uint8_t) ((speed / 255.0) * 100);
}

void MosfetMotor::set_speed(uint8_t s) {
    analogWrite(pwm, (speed = s));
}

long MosfetMotor::get_evolution() {
    return rotary_count / EVOLUTION;
}

void MosfetMotor::reset() {
    set_speed(0);
    rotary_count = 0;
}

void MosfetMotor::cycle() {}

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
    if (speed - MIN_STEP >= MIN_SPEED && speed <= MAX_SPEED) {
        motor->set_speed(speed - MIN_STEP);
    }
}

MotorState MosfetMotor::get_state() {
    return speed ? ON : OFF;
}
