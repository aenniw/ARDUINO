#ifndef ARDUINO_PROJECTS_ROOT_MOTOR_BRIDGE_H
#define ARDUINO_PROJECTS_ROOT_MOTOR_BRIDGE_H

#include <Arduino.h>
#include <Motor.h>

#define MIN_SPEED 150
#define MAX_SPEED 255
#define SPEED_STEP_DURATION 15

class MotorBridge : public Motor {
private:
    uint8_t r_pwm = 0, l_pwm = 0, r_enable = 0, l_enable = 0;
    volatile uint8_t speed = 0;

protected:
    void _off() override;

    void _dir_cw() override;

    void _dir_ccw() override;

    void setSpeed(MotorState state, uint8_t speed);

    void enable();

public:
    MotorBridge(uint8_t _pin1, uint8_t _pin2,
                uint8_t _pin3, uint8_t _pin4,
                uint8_t _pin5, uint8_t _pin6);

    void cycle() override;
};

#endif //ARDUINO_PROJECTS_ROOT_MOTOR_BRIDGE_H
