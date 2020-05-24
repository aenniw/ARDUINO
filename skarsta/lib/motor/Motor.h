#ifndef ARDUINO_PROJECTS_ROOT_MOTOR_H
#define ARDUINO_PROJECTS_ROOT_MOTOR_H

#include <Arduino.h>
#include <Service.h>

#ifdef __USENSOR__
#include <HCSR04.h>
#else
#include <Rotary.h>
#endif

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
#define ADDRESS_END_STOP_0 (ADDRESS_POSITION + sizeof(unsigned int))
#define ADDRESS_END_STOP_1 (ADDRESS_END_STOP_0 + sizeof(unsigned int))
#define ADDRESS_MODE (ADDRESS_END_STOP_1 + sizeof(MotorMode))
#endif

#define STOP_POS_DIFF 1
#define MINIMUM_POS_CHANGE 8

class Motor : public TimedService {
private:
#ifndef __USENSOR__
    Rotary sensor;
#else
    UltraSonicDistanceSensor sensor;
#endif
    const uint8_t sensor_pin_1 = 0, sensor_pin_2 = 0;

    bool disabled = false;
    long next_position = -1;
    unsigned int end_stop[2] = {0u, ~0u};

    MotorState state = OFF;
    MotorMode mode = UNCALIBRATED;
    volatile unsigned int position = 0u, position_change = 0u;

protected:
#ifndef __USENSOR__

    void update_position(unsigned char result);

#endif

    void initPin(uint8_t pin, uint8_t val = LOW);

    virtual void _off() = 0;

    virtual void _dir_cw() = 0;

    virtual void _dir_ccw() = 0;

public:
    Motor(uint8_t _pin1, uint8_t _pin2);

    bool begin() override;

    void off();

    void dir_cw();

    void dir_ccw();

    void set_end_stop(unsigned int end_stop);

    unsigned int get_position() const;

    unsigned int get_position_change();

    void reset_position();

    void set_position(unsigned int pos);

    MotorState get_state();

    MotorMode get_mode();

    void set_mode(MotorMode state);

    void disable() override;

    void cycle() override;
};

#endif //ARDUINO_PROJECTS_ROOT_MOTOR_H
