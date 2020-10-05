#pragma once

#include <Service.h>
#include <Motor.h>
#include <Triggers.h>

#define END_STOP_OFFSET 100
#define SPIN_DELAY 750

#ifdef __EEPROM__
const size_t ADDRESS_PRESETS[3] = {
        (ADDRESS_MODE + sizeof(unsigned int)),
        (ADDRESS_MODE + 2 * sizeof(unsigned int)),
        (ADDRESS_MODE + 3 * sizeof(unsigned int))};
#endif

class Calibrator : public TimedService, public Trigger {
private:
    Motor *motor = nullptr;
    bool auto_calibrating = false;
    unsigned long spin_up = 0;
protected:

    void uncalibrated();

    void semi_calibrated();

    void calibrated(unsigned int offset = 0);

public:
    explicit Calibrator(Motor *motor);

    void calibrate();

    void auto_calibrate();

    void set_preset(uint8_t i);

    unsigned int get_preset(uint8_t i);

    void cycle() override;

    bool trip(ErrorType t) override;

    void reset() override;
};