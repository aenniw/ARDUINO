#pragma once

#include <Arduino.h>
#include <Service.h>
#include <Motor.h>
#include <Display.h>
#include <NIButtons.h>
#include <Calibrator.h>

#ifdef __EEPROM__
#include <EEPROM.h>
#endif

#define SAVE_BUTTON_TIMEOUT 1500
#define RST_BUTTON_TIMEOUT 1500

class Keypad : public Service {
private:
    NIButton down, up, rst, preset_0, preset_1, preset_2;
    NIButton *preset_buttons[3], *buttons[6];
protected:
    bool stop_motor();

public:
    Keypad(Motor *_motor, Display *_display, Calibrator *_calibrator,
           uint8_t down_pin, uint8_t up_pin, uint8_t rst_pin,
           uint8_t preset_0_pin, uint8_t preset_1_pin, uint8_t preset_2_pin);

    bool begin() override;

    void goto_preset(uint8_t i, unsigned int pos);

    void cycle() override;
};