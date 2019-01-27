#ifndef ARDUINO_PROJECTS_ROOT_MOTOR_H
#define ARDUINO_PROJECTS_ROOT_MOTOR_H

#include <Arduino.h>
#include <Service.h>

typedef enum {
    CCW,
    CW,
    OFF
} MotorState;

typedef enum {
    UNCALIBRATED,   // nothing calibrated
    SEMICALIBRATED, // bottom calibrated
    CALIBRATED      // bottom-top calibrated
} MotorMode;

#ifdef __EEPROM__
#define ADDRESS_POSITION 0
#define ADDRESS_END_STOP (ADDRESS_POSITION + sizeof(unsigned int))
#define ADDRESS_MODE (ADDRESS_END_STOP + sizeof(MotorMode))
#endif

#define MINIMUM_POS_CHANGE 8

class Motor : Service {
private:
    unsigned int end_stop = ~0u;
    MotorMode mode = UNCALIBRATED;
    long next_position = -1;
    bool disabled = false;

    volatile MotorState state = OFF;
    volatile unsigned int position = 0, position_change = 0;

protected:
    void update_position(unsigned char result);

    void initPin(uint8_t pin, uint8_t val = LOW);

    virtual void _off() = 0;

    virtual void _dir_cw() = 0;

    virtual void _dir_ccw() = 0;

public:
    Motor(uint8_t _pin1, uint8_t _pin2);

    void off();

    void dir_cw();

    void dir_ccw();

    void set_end_stop(unsigned int end_stop);

    unsigned int get_position();

    unsigned int get_position_change();

    void reset_position();

    void set_position(unsigned int pos);

    MotorState get_state();

    MotorMode get_mode();

    void set_mode(MotorMode state);

    void disable();;

    void cycle() override;

    ~Motor();
};

#endif //ARDUINO_PROJECTS_ROOT_MOTOR_H
