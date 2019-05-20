#ifndef ARDUINO_PROJECTS_MOTORMOSFET_H
#define ARDUINO_PROJECTS_MOTORMOSFET_H

#include <Arduino.h>
#include <Motor.h>

#define MAX_SPEED 255
#define MIN_SPEED 30
#define MIN_STEP 5
#define EVOLUTION 216
#define SPOOL_RADIUS 1.5

class MosfetMotor : public Motor {
private:
    uint8_t pwm = 0, gate = 0, speed = 0;
    volatile unsigned long rotary_count = 0;

public:
    MosfetMotor(uint8_t pwm, uint8_t gate);

    void set_speed(uint8_t s) override;

    uint8_t get_speed() override;

    MotorState get_state() override;

    unsigned long get_evolution() override;

    double get_len() override;

    void reset() override;

    void cycle() override;

    void toggle() override;

    void increase_speed() override;

    void decrease_speed() override;

    ~MosfetMotor();
};


#endif //ARDUINO_PROJECTS_MOTORMOSFET_H
