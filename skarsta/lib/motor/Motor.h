#ifndef ARDUINO_PROJECTS_ROOT_MOTOR_H
#define ARDUINO_PROJECTS_ROOT_MOTOR_H

#include <Arduino.h>
#include <Service.h>

extern Table_Data table_data;

class Motor : Service {
private:
    uint8_t power_pin, dir_pin = 0;
public:
    Motor(uint8_t _pin1, uint8_t _pin2, uint8_t _pin3, uint8_t _pin4);

    void off() {
#ifdef __DEBUG__
        if (!digitalRead(power_pin)) {
            Serial.println("off");
        }
#endif
        digitalWrite(power_pin, HIGH);
    }

    void dir_cw() {
        if (table_data.calibration == CALIBRATED && table_data.position >= table_data.end_stop) {
            return;
        }
        digitalWrite(dir_pin, HIGH);
        delay(100);
        digitalWrite(power_pin, LOW);
#ifdef __DEBUG__
        Serial.println("ccw");
#endif
    }

    void dir_ccw() {
        if (table_data.calibration != UNCALIBRATED && table_data.position <= 0) {
            return;
        }
        digitalWrite(dir_pin, LOW);
        delay(100);
        digitalWrite(power_pin, LOW);
#ifdef __DEBUG__
        Serial.println("cw");
#endif
    }

    void cycle() {};

    ~Motor();
};


#endif //ARDUINO_PROJECTS_ROOT_MOTOR_H
