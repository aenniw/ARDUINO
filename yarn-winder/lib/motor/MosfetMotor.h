#ifndef ARDUINO_PROJECTS_MOTORMOSFET_H
#define ARDUINO_PROJECTS_MOTORMOSFET_H

#include <Arduino.h>
#include <Motor.h>

#define MIN_SPEED 20

class MosfetMotor : public Motor {
private:
    uint8_t pwm = 0, gate = 0, speed = 0;
    volatile unsigned long rotary_count = 0;

public:
    MosfetMotor(uint8_t pwm, uint8_t gate);

    void set_speed(uint8_t s) override;

    uint8_t get_speed() override;

    MotorState get_state() override;

    long get_evolution() override;

    void reset() override;

    void cycle() override;

    void toggle() override;

    void increase_speed() override;

    void decrease_speed() override;

    ~MosfetMotor();
};


#endif //ARDUINO_PROJECTS_MOTORMOSFET_H
