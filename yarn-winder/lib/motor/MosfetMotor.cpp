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
    return speed;
}

void MosfetMotor::set_speed(uint8_t s) {
    digitalWrite(pwm, (speed = s));
}

long MosfetMotor::get_evolution() {
    return rotary_count;
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
    if (speed >= MIN_SPEED && speed < 255) {
        motor->set_speed(speed + 1);
    }
}

void MosfetMotor::decrease_speed() {
    if (speed > MIN_SPEED && speed < 255) {
        motor->set_speed(speed - 1);
    }
}

MotorState MosfetMotor::get_state() {
    return speed ? ON : OFF;
}
