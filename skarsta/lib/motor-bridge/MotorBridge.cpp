#include "MotorBridge.h"

void MotorBridge::_off() {
    this->speed = 0;
    analogWrite(r_pwm, 0);
    analogWrite(l_pwm, 0);
}

void MotorBridge::_dir_cw() {
    setSpeed(CW, MIN_SPEED);
}

void MotorBridge::_dir_ccw() {
    setSpeed(CCW, MIN_SPEED);
}

void MotorBridge::setSpeed(MotorState state, uint8_t speed) {
    if (state == OFF) {
#ifdef __DEBUG__
        Serial.print("m speed - off ");
        Serial.print(speed);
        Serial.println();
#endif
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
        analogWrite(r_pwm, this->speed);
    } else if (state == CW) {
        analogWrite(r_pwm, 0);
        analogWrite(l_pwm, this->speed);
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