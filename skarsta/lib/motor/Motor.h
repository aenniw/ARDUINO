#pragma once

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

#ifdef __USENSOR__
#define SENSOR_TIMEOUT          125      // USensor sample rate in milliseconds
#define SENSOR_DELTA_ON     10       // USensor min change when ON in millimeters
#define SENSOR_DELTA_OFF    75       // USensor min change when OFF in millimeters
#endif

class Motor : public TimedService {
private:
#ifndef __USENSOR__
    Rotary sensor;
#else
    UltraSonicDistanceSensor sensor;
#endif
    const uint8_t sensor_pin_1 = 0, sensor_pin_2 = 0;
    const uint8_t pos_diff = 0, min_change = 0;

    bool disabled = false;
    long next_position = -1;
    unsigned int end_stop[2] = {0u, ~0u};

    MotorState state = OFF;
    MotorMode mode = UNCALIBRATED;
    volatile unsigned int position = 0u, position_change = 0u;

protected:
    bool reverse = false;

#ifndef __USENSOR__
    void update_position(unsigned char result);
#endif

    void initPin(uint8_t pin, uint8_t val = LOW);

    bool check_end_stops(const unsigned int end_stop_down, const unsigned int end_stop_up) const;

    virtual void _off() = 0;

    virtual void _dir_cw() = 0;

    virtual void _dir_ccw() = 0;

public:
    Motor(uint8_t _pin1, uint8_t _pin2, uint8_t stop_diff, uint8_t min_change, bool reverse);

    bool begin() override;

    void off();

    void dir_cw();

    void dir_ccw();

    void set_end_stop(unsigned int end_stop, unsigned int offset = 0);

    unsigned int get_position() const;

    unsigned int get_position_change();

    void reset_position();

    void set_position(unsigned int pos);

    MotorState get_state() const;

    MotorMode get_mode() const;

    unsigned int get_end_stop_low();

    unsigned int get_end_stop_high();

    void set_mode(MotorMode state);

    void disable() override;

    void cycle() override;
};