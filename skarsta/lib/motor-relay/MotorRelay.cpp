#include <MotorRelay.h>

MotorRelay::MotorRelay(Stepper *stepper, uint8_t _pin1, uint8_t _pin2) : Motor(stepper) {
    initPin((power_pin = _pin1), HIGH);
    initPin((dir_pin = _pin2));
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
