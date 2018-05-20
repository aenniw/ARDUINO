#ifdef __EEPROM__
#include <EEPROM.h>
#endif

#include <Motor.h>
#include <Rotary.h>
#include "Motor.h"
#include "../service/Service.h"

extern Table_Data table_data;

static Rotary *encoder = nullptr;
static uint8_t power = 0, dir = 0;
static long next_position = -1;

static void encoder_rotate() {
    unsigned char result = encoder->process();

    if (table_data.calibration == UNCALIBRATED)
        return;
    if (result == DIR_CW) {
        table_data.position++;
    } else if (result == DIR_CCW && table_data.position != 0) {
        table_data.position--;
    }
#ifdef __DEBUG__
    Serial.println(table_data.position);
#endif
#ifdef __EEPROM__
    EEPROM.put(EEPROM.begin(), table_data);
#endif
    if (table_data.position == 0 || table_data.position >= table_data.end_stop) {
        if (table_data.position == 0 && digitalRead(dir) == LOW) {
            digitalWrite(power, HIGH);
        } else if (table_data.position >= table_data.end_stop && digitalRead(dir) == HIGH) {
            digitalWrite(power, HIGH);
        }
    }
    if (next_position >= 0 && next_position == table_data.position) {
        digitalWrite(power, HIGH);
        next_position = -1;
    }
}


Motor::Motor(uint8_t _pin1, uint8_t _pin2, uint8_t _pin3, uint8_t _pin4) {
    encoder = new Rotary(_pin1, _pin2);
    power_pin = _pin3;
    power = _pin3;
    dir_pin = _pin4;
    dir = _pin4;

    attachInterrupt((uint8_t) digitalPinToInterrupt(_pin1), encoder_rotate, CHANGE); // set interrupt
    attachInterrupt((uint8_t) digitalPinToInterrupt(_pin2), encoder_rotate, CHANGE); // set interrupt
    pinMode(dir_pin, OUTPUT);
    pinMode(power_pin, OUTPUT);
    digitalWrite(dir_pin, LOW);
    digitalWrite(power_pin, HIGH);
}

void Motor::goto_position(unsigned int pos) {
    if (table_data.calibration != CALIBRATED || table_data.end_stop < pos) {
#ifdef __DEBUG__
        Serial.println("Cannot go to pos.");
#endif
        return;
    }
    next_position = pos;
    if (next_position < table_data.position) {
        dir_ccw();
    } else {
        dir_cw();
    }
}

void Motor::off() {
#ifdef __DEBUG__
    if (!digitalRead(power_pin)) {
            Serial.println("off");
        }
#endif
    digitalWrite(power_pin, HIGH);
}

void Motor::dir_cw() {
    if (table_data.calibration == CALIBRATED && table_data.position >= table_data.end_stop) {
#ifdef __DEBUG__
        Serial.println("Cannot cw");
#endif
        return;
    }
    digitalWrite(dir_pin, HIGH);
    delay(100);
    digitalWrite(power_pin, LOW);
#ifdef __DEBUG__
    Serial.println("cw");
#endif
}

void Motor::dir_ccw() {
    if (table_data.calibration != UNCALIBRATED && table_data.position <= 0) {
#ifdef __DEBUG__
        Serial.println("Cannot ccw");
#endif
        return;
    }
    digitalWrite(dir_pin, LOW);
    delay(100);
    digitalWrite(power_pin, LOW);
#ifdef __DEBUG__
    Serial.println("ccw");
#endif
}

Motor::~Motor() {
    delete encoder;
}
