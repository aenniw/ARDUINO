#include "Keypad.h"

static Keypad *keypad = nullptr;
static Motor *motor = nullptr;
static Display *display = nullptr;
unsigned int preset_values[3] = {0u};

#define SET_PRESET_BUTTON(p, on) preset_buttons[p]->long_press(SAVE_BUTTON_TIMEOUT)\
                                    ->on_short_press([]() { if (!keypad->stop_motor()) keypad->goto_preset(p); })\
                                    ->on_long_press([]() { keypad->set_preset(p); })\
                                    ->on_press(on)

Keypad::Keypad(Motor *_motor, Display *_display,
               uint8_t down_pin, uint8_t up_pin, uint8_t rst_pin,
               uint8_t preset_0_pin, uint8_t preset_1_pin, uint8_t preset_2_pin) :
        down(down_pin), up(up_pin), rst(rst_pin),
        preset_0(preset_0_pin), preset_1(preset_1_pin), preset_2(preset_2_pin),
        preset_buttons({&preset_0, &preset_1, &preset_2}),
        buttons({&preset_0, &preset_1, &preset_2, &down, &up, &rst}) {
    motor = _motor;
    display = _display;

    keypad = this;
    for (uint8_t i = 0; i < 3; i++) {
#ifdef __EEPROM__
        EEPROM.get(ADDRESS_PRESETS[i], preset_values[i]);

#ifdef __DEBUG__
        Serial.print("preset ");
        Serial.print(i);
        Serial.print(" ");
        Serial.print(preset_values[i]);
        Serial.println();
#endif
#endif
    }
    auto motor_off = []() { motor->off(); };
    // up/down
    down.on_press([]() { if (!keypad->stop_motor()) motor->dir_ccw(); });
    down.on_release(motor_off);
    up.on_press([]() { if (!keypad->stop_motor()) motor->dir_cw(); });
    up.on_release(motor_off);
    // display light-up callback
    auto light_up = []() { display->light_up(); };
    // presets
    SET_PRESET_BUTTON(0, light_up);
    SET_PRESET_BUTTON(1, light_up);
    SET_PRESET_BUTTON(2, light_up);
    // calibration
    rst.long_press(RST_BUTTON_TIMEOUT);
    rst.on_short_press([]() { keypad->stop_motor(); });
    rst.on_long_press([]() {
        switch (motor->get_mode()) {
            case UNCALIBRATED:
                motor->set_mode(SEMICALIBRATED);
                motor->reset_position();
                break;
            case SEMICALIBRATED:
                motor->set_mode(CALIBRATED);
                motor->set_end_stop(motor->get_position());
                display->set_blink(false);
                break;
            default:
                for (int i = 0; i < 3; i++) {
                    preset_values[i] = 0u;
#ifdef __EEPROM__
                    updateEEPROM(ADDRESS_PRESETS[i], preset_values[i]);
#endif
                }
                motor->set_mode(UNCALIBRATED);
                motor->set_end_stop(~0u);
                display->set_blink(true);
                motor->reset_position();
                break;
        }
    });
    rst.on_press(light_up);
}

bool Keypad::begin() {
    for (auto &button : buttons)
        if (!button->begin())
            return false;
    return true;
}

void Keypad::cycle() {
    for (auto &button : buttons)
        button->cycle();

    if (rst.held()) {
        display->set_blink(false);
        display->print("-rst");
        return;
    }
    for (auto &preset_button : preset_buttons) {
        if (preset_button->held()) {
            display->set_blink(false);
            display->print("-set");
            return;
        }
    }

    static unsigned int pos_watch = 0;
    if (pos_watch != motor->get_position()) {
        pos_watch = motor->get_position();
        display->set_blink(false);
    } else if (motor->get_mode() == SEMICALIBRATED)
        display->set_blink(true);

    if (motor->get_mode() != UNCALIBRATED)
        display->print(motor->get_position());
    else if (motor->get_mode() == UNCALIBRATED) {
        display->set_blink(true);
        display->print(DISPLAY_NONE);
    }
}

void Keypad::set_preset(uint8_t i) {
    if (motor->get_mode() != CALIBRATED)
        return;
    preset_values[i] = motor->get_position();
#ifdef __EEPROM__
    updateEEPROM(ADDRESS_PRESETS[i], preset_values[i]);
#endif
#ifdef __DEBUG__
    Serial.print("set preset: ");
    Serial.print(i);
    Serial.print(" ");
    Serial.print(preset_values[i]);
    Serial.println();
#endif
}

void Keypad::goto_preset(uint8_t i) {
    if (motor->get_mode() != CALIBRATED)
        return;
#ifdef __DEBUG__
    Serial.print("goto preset: ");
    Serial.print(i);
    Serial.print(" ");
    Serial.print(preset_values[i]);
    Serial.println();
#endif
    motor->set_position(preset_values[i]);
}

bool Keypad::stop_motor() {
    if (motor->get_state() == OFF)
        return false;
    motor->off();
    return true;
}
