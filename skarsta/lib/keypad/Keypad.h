#ifndef ARDUINO_PROJECTS_ROOT_KEYPAD_H
#define ARDUINO_PROJECTS_ROOT_KEYPAD_H

#include <Arduino.h>
#include <Service.h>
#include <Motor.h>
#include <Display.h>
#include <NIButtons.h>

#ifdef __EEPROM__
#include <EEPROM.h>
#endif

#define SAVE_BUTTON_TIMEOUT 1500
#define RST_BUTTON_TIMEOUT 1500

#ifdef __EEPROM__
const size_t ADDRESS_PRESETS[3] = {
        (ADDRESS_MODE + sizeof(unsigned int)),
        (ADDRESS_MODE + 2 * sizeof(unsigned int)),
        (ADDRESS_MODE + 3 * sizeof(unsigned int))};
#endif

class Keypad : Service {
private:
    NIButton down, up, rst, preset_0, preset_1, preset_2;
    NIButton *preset_buttons[3], *buttons[6];
protected:
    bool stop_motor();

public:
    Keypad(Motor *_motor, Display *_display,
           uint8_t down_pin, uint8_t up_pin, uint8_t rst_pin,
           uint8_t preset_0_pin, uint8_t preset_1_pin, uint8_t preset_2_pin);

    bool begin() override;

    void set_preset(uint8_t i);

    void goto_preset(uint8_t i);

    void cycle() override;
};

#endif //ARDUINO_PROJECTS_ROOT_KEYPAD_H
