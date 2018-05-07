#include <Motor.h>
#include <EEPROM.h>
#include <Rotary.h>
#include "Motor.h"

static Rotary *encoder = nullptr;
static uint8_t power = 0, dir = 0;

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
    EEPROM.put(EEPROM.begin(), table_data);

    if (table_data.position <= 0 || table_data.position >= table_data.end_stop) {
        if (table_data.position <= 0 && digitalRead(dir) == LOW) {
            digitalWrite(power, HIGH);
        } else if (table_data.position >= table_data.end_stop && digitalRead(dir) == HIGH) {
            digitalWrite(power, HIGH);
        }
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

Motor::~Motor() {
    delete encoder;
}
