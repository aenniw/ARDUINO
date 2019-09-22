#ifndef ARDUINO_PROJECTS_MOTORMOSFET_H
#define ARDUINO_PROJECTS_MOTORMOSFET_H

#include <Arduino.h>
#include <Motor.h>
#include <Configuration.h>

#define MAX_SPEED           255
#define MIN_SPEED           30
#define STEP_SPEED          5

#define MAX_STALL           1000
#define MIN_STALL           100
#define STEP_STALL          50

#define SPIN_TIMEOUT        500
#define SPINUP_TIMEOUT      750

#define IR_TRIGGER          RISING
#define EVOLUTION_OFFSET    4
#define EVOLUTION           44


class MosfetMotor : public Motor {
private:
    PROFILE profile = Manual;

    uint8_t pwm = 0, gate = 0, speed = 0;
    uint16_t stall_timeout = 250;
    volatile unsigned long rotary_count = 0;
    unsigned long rotary_count_end = 0;
    bool no_spin = true;
private:
    void stall_detect(unsigned long ms);

    void spin_detect(unsigned long ms);

    void spin_down(unsigned long ms);

    void spin_up(unsigned long ms);

protected:
    unsigned long get_remaining_evolutions() const;

    bool is_rotating() const;

    void set_speed(uint8_t s);

    void set_stall_timeout(uint16_t t);

public:
    MosfetMotor(uint8_t pwm, uint8_t gate);

    uint8_t get_speed() const override;

    MotorState get_state() const override;

    unsigned long get_evolution() const override;

    double get_len() const override;

    void reset() override;

    void cycle() override;

    void toggle() override;

    void increase_speed() override;

    void decrease_speed() override;

    void increase_stop_evolution() override;

    void decrease_stop_evolution() override;

    unsigned long *get_stop_evolution() const override;

    void increase_stall_timeout() override;

    void decrease_stall_timeout() override;

    uint16_t *get_stall_timeout() const override;

    PROFILE *get_profile() const override;

    void next_profile() override;

    void prev_profile() override;

    ~MosfetMotor();
};


#endif //ARDUINO_PROJECTS_MOTORMOSFET_H
