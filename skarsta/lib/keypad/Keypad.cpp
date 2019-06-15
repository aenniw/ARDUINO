#include "Keypad.h"

static Keypad *keypad = nullptr;
static Motor *motor = nullptr;
static Display *display = nullptr;
unsigned int preset_values[3] = {0u};

#define PRESET_BUTTON(pin, p, on) buttons->add_button(pin)\
                                            ->long_press(SAVE_BUTTON_TIMEOUT)\
                                            ->on_short_press([]() { if (!keypad->stop_motor()) keypad->goto_preset(p); })\
                                            ->on_long_press([]() { keypad->set_preset(p); })\
                                            ->on_press(on);
#define SET_PRESET_BUTTON(pin, p, on) preset_buttons[p] = PRESET_BUTTON(pin, p,on)

Keypad::Keypad(Motor *_motor, Display *_display) {
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
    auto buttons = NIButtons::get_instance();
    // up/down
    down = buttons->add_button(BUTTON_DOWN)
            ->on_press([]() { if (!keypad->stop_motor()) motor->dir_ccw(); })
            ->on_release(motor_off);
    up = buttons->add_button(BUTTON_UP)
            ->on_press([]() { if (!keypad->stop_motor()) motor->dir_cw(); })
            ->on_release(motor_off);
    // display light-up callback
    auto light_up = []() { display->light_up(); };
    // presets
    SET_PRESET_BUTTON(BUTTON_P0, 0, light_up);
    SET_PRESET_BUTTON(BUTTON_P1, 1, light_up);
    SET_PRESET_BUTTON(BUTTON_P2, 2, light_up);
    // calibration
    rst = buttons->add_button(BUTTON_RST)
            ->long_press(RST_BUTTON_TIMEOUT)
            ->on_short_press([]() { keypad->stop_motor(); })
            ->on_long_press([]() {
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
            })
            ->on_press(light_up);
}

void Keypad::cycle() {
    static unsigned int pos_watch = 0;

    if (rst->held()) {
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

Keypad::~Keypad() {
    delete down;
    delete up;
    delete rst;
    for (auto &preset_button : preset_buttons)
        delete preset_button;
}

bool Keypad::stop_motor() {
    if (motor->get_state() == OFF)
        return false;
    motor->off();
    return true;
}
