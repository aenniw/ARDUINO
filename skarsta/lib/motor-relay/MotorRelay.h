#ifndef ARDUINO_PROJECTS_ROOT_MOTOR_RELAY_H
#define ARDUINO_PROJECTS_ROOT_MOTOR_RELAY_H

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
    MotorRelay(uint8_t _pin1, uint8_t _pin2, uint8_t _pin3, uint8_t _pin4) :
            Motor(_pin1, _pin2), power_pin(_pin3), dir_pin(_pin4) {};

    bool begin() override;
};

#endif //ARDUINO_PROJECTS_ROOT_MOTOR_RELAY_H
