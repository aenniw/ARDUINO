#ifndef ARDUINO_PROJECTS_ROOT_MOTOR_H
#define ARDUINO_PROJECTS_ROOT_MOTOR_H

#include <Arduino.h>
#include <Service.h>

class Motor : Service {
private:
    uint8_t power_pin, dir_pin = 0;
public:
    Motor(uint8_t _pin1, uint8_t _pin2, uint8_t _pin3, uint8_t _pin4);

    void off();

    void dir_cw();

    void dir_ccw();

    void goto_position(unsigned int pos);

    void cycle() override {};

    ~Motor();
};


#endif //ARDUINO_PROJECTS_ROOT_MOTOR_H
