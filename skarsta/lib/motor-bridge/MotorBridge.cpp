#include "MotorBridge.h"

MotorBridge::MotorBridge(uint8_t _pin1, uint8_t _pin2, uint8_t _pin3, uint8_t _pin4, uint8_t _pin5, uint8_t _pin6)
        : Motor(_pin1, _pin2) {
    initPin((r_enable = _pin3));
    initPin((l_enable = _pin4));
    initPin((r_pwm = _pin5));
    initPin((l_pwm = _pin6));
    this->enable();
}

void MotorBridge::_off() {
    digitalWrite(r_enable, LOW);
    digitalWrite(l_enable, LOW);
    this->speed = 0;
    analogWrite(r_pwm, 0);
    analogWrite(l_pwm, 0);
}

void MotorBridge::enable() {
    digitalWrite(r_enable, HIGH);
    digitalWrite(l_enable, HIGH);
}

void MotorBridge::_dir_cw() {
    setSpeed(CW, MIN_SPEED);
    this->enable();
}

void MotorBridge::_dir_ccw() {
    setSpeed(CCW, MIN_SPEED);
    this->enable();
}

void MotorBridge::setSpeed(MotorState state, uint8_t speed) {
    if (state == OFF) {
        return;
    }
    this->speed = speed;

#ifdef __DEBUG__
    Serial.print("m speed ");
    Serial.print(speed);
    Serial.println();
#endif

    if (state == CCW) {
        analogWrite(l_pwm, 0);
        analogWrite(r_pwm, speed);
    } else if (state == CW) {
        analogWrite(r_pwm, 0);
        analogWrite(l_pwm, speed);
    }
}

void MotorBridge::cycle() {
    static unsigned long last_tick = millis();
    unsigned long now = millis(), diff = get_period(last_tick, now);

    if (diff >= SPEED_STEP_DURATION && speed < MAX_SPEED && speed >= MIN_SPEED) {
        setSpeed(get_state(), speed + 5 % (MAX_SPEED + 1));
        last_tick = now;
    }
}