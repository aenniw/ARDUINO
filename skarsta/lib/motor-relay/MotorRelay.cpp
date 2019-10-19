#include <MotorRelay.h>

void MotorRelay::begin() {
    Motor::begin();
    initPin(power_pin, HIGH);
    initPin(dir_pin);
}

void MotorRelay::_off() {
    digitalWrite(power_pin, HIGH);
}

void MotorRelay::_dir_cw() {
    _off();
    digitalWrite(dir_pin, HIGH);
    digitalWrite(power_pin, LOW);
}

void MotorRelay::_dir_ccw() {
    _off();
    digitalWrite(dir_pin, LOW);
    digitalWrite(power_pin, LOW);
}