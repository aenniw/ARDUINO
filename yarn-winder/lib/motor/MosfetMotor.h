#ifndef ARDUINO_PROJECTS_MOTORMOSFET_H
#define ARDUINO_PROJECTS_MOTORMOSFET_H

#include <Arduino.h>
#include <Motor.h>

#define MAX_SPEED           255
#define MIN_SPEED           30
#define MIN_STEP            5

#define IR_TRIGGER          RISING
#define EVOLUTION_OFFSET    4
#define EVOLUTION           44


class MosfetMotor : public Motor {
private:
    uint8_t pwm = 0, gate = 0, speed = 0;
    volatile unsigned long rotary_count = 0;
    unsigned long rotary_count_end = 8;

protected:
    void set_speed(uint8_t s) override;

public:
    MosfetMotor(uint8_t pwm, uint8_t gate);

    uint8_t get_speed() override;

    MotorState get_state() override;

    unsigned long get_evolution() const override;

    double get_len() override;

    void reset() override;

    void cycle() override;

    void toggle() override;

    void increase_speed() override;

    void decrease_speed() override;

    void increase_stop_evolution() override;

    void decrease_stop_evolution() override;

    unsigned long *get_stop_evolution() override;

    ~MosfetMotor();
};


#endif //ARDUINO_PROJECTS_MOTORMOSFET_H
