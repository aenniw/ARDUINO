#pragma once

#include <Arduino.h>
#include <Motor.h>

#ifndef __INVERSE_RELAYS__
    #define ENABLE LOW
    #define DISABLE HIGH
#else
    #define ENABLE HIGH
    #define DISABLE LOW
#endif

class MotorRelay : public Motor {
private:
    uint8_t power_pin = 0, dir_pin = 0;
protected:

    void _off() override;

    void _dir_cw() override;

    void _dir_ccw() override;

public:
    MotorRelay(uint8_t _pin1, uint8_t _pin2, uint8_t _pin3, uint8_t _pin4, uint8_t stop_diff, uint8_t min_change, bool reverse) :
            Motor(_pin1, _pin2, stop_diff, min_change, reverse), power_pin(_pin3), dir_pin(_pin4) {};

    bool begin() override;
};