#include <MotorRelay.h>

void MotorRelay::begin() {
    Motor::begin();
    initPin(power_pin, DISABLE);
    initPin(dir_pin, DISABLE);
}

void MotorRelay::_off() {
    digitalWrite(power_pin, DISABLE);
}

void MotorRelay::_dir_cw() {
    _off();
    digitalWrite(dir_pin, DISABLE);
    digitalWrite(power_pin, ENABLE);
}

void MotorRelay::_dir_ccw() {
    _off();
    digitalWrite(dir_pin, ENABLE);
    digitalWrite(power_pin, ENABLE);
}