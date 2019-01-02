#include "MotorBridge.h"

void MotorBridge::_off() {
    setSpeed(0);
}

void MotorBridge::_dir_cw() {
    _off();
    digitalWrite(r_enable, HIGH);
    digitalWrite(l_enable, HIGH);
    setSpeed(INITIAL_SPEED);
}

void MotorBridge::_dir_ccw() {
    _off();
    digitalWrite(r_enable, LOW);
    digitalWrite(l_enable, LOW);
    setSpeed(INITIAL_SPEED);
}

void MotorBridge::setSpeed(uint8_t speed) {
    if (get_state() == OFF) return;
    this->speed = speed;
#ifdef __DEBUG__
    Serial.print("m speed ");
    Serial.print(speed);
    Serial.println();
#endif
    digitalWrite(r_pwm, this->speed);
    digitalWrite(l_pwm, this->speed);
}

void MotorBridge::cycle() {
    static unsigned long last_tick = millis();
    unsigned long now = millis(), diff = get_period(last_tick, now);

    if (diff >= 125 && speed < 255) {
        setSpeed(speed + 5 % 256);
        last_tick = now;
    }
}