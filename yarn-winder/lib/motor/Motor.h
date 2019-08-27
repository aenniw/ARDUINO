#ifndef ARDUINO_PROJECTS_MOTOR_H
#define ARDUINO_PROJECTS_MOTOR_H

#include <Service.h>

typedef enum {
    OFF,
    ON
} MotorState;

class Motor : Service {
public:
    virtual void set_speed(uint8_t s)=0;

    virtual uint8_t get_speed() =0;

    virtual MotorState get_state() =0;

    virtual void reset()=0;

    virtual unsigned long get_evolution() const =0;

    virtual double get_len() =0;

    virtual void toggle()=0;

    virtual void increase_speed() =0;

    virtual void decrease_speed() =0;

    virtual void increase_stop_evolution() =0;

    virtual void decrease_stop_evolution() =0;

    virtual unsigned long *get_stop_evolution() =0;
};


#endif //ARDUINO_PROJECTS_MOTOR_H
