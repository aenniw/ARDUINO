#ifndef ARDUINO_PROJECTS_ROOT_MOTOR_RELAY_H
#define ARDUINO_PROJECTS_ROOT_MOTOR_RELAY_H

#include <Arduino.h>
#include <Motor.h>

class MotorRelay : public Motor {
private:
    uint8_t power_pin = 0, dir_pin = 0;
protected:

    void _off() override;

    void _dir_cw() override;

    void _dir_ccw() override;

public:
    MotorRelay(Stepper *stepper, uint8_t _pin1, uint8_t _pin2);
};

#endif //ARDUINO_PROJECTS_ROOT_MOTOR_RELAY_H
