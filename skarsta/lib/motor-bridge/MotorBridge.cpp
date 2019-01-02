#include "MotorBridge.h"

void MotorBridge::_off() {
    setSpeed(0);
}

void MotorBridge::_dir_cw() {
    _off();
    digitalWrite(r_enable, HIGH);
    digitalWrite(l_enable, HIGH);
    setSpeed(255);
}

void MotorBridge::_dir_ccw() {
    _off();
    digitalWrite(r_enable, LOW);
    digitalWrite(l_enable, LOW);
    setSpeed(255);
}