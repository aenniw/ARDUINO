#include "Keypad.h"

static Keypad *keypad = nullptr;
static Motor *motor = nullptr;
static Display *display = nullptr;
unsigned int preset_values[3] = {0u};

#define PRESET_BUTTON(pin, p) new TimedButton(pin, SAVE_BUTTON_TIMEOUT, []() { if (!keypad->stop_motor()) keypad->goto_preset(p); }, []() { keypad->set_preset(p); })
#define SET_PRESET_BUTTON(pin, p) preset_buttons[p] = PRESET_BUTTON(pin, p)

Keypad::Keypad(Motor *_motor, Display *_display)
{
    motor = _motor;
    display = _display;

    keypad = this;
    for (uint8_t i = 0; i < 3; i++)
    {
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
    // up/down
    down = new ToggleButton(BUTTON_DOWN, []() { if (!keypad->stop_motor()) motor->dir_ccw(); }, []() { motor->off(); });
    up = new ToggleButton(BUTTON_UP, []() { if (!keypad->stop_motor()) motor->dir_cw(); }, []() { motor->off(); });
    // presets
    SET_PRESET_BUTTON(BUTTON_P0, 0);
    SET_PRESET_BUTTON(BUTTON_P1, 1);
    SET_PRESET_BUTTON(BUTTON_P2, 2);
    // calibration
    rst = new TimedButton(BUTTON_RST, RST_BUTTON_TIMEOUT, []() { keypad->stop_motor(); }, []() {
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
        } });
}

void Keypad::cycle()
{
    static unsigned int pos_watch = 0;

    if (rst->get_state() && !rst->is_short())
    {
        display->set_blink(false);
        display->display_print("-rst");
        return;
    }
    for (auto &preset_button : preset_buttons)
    {
        if (preset_button->get_state() && !preset_button->is_short())
        {
            display->set_blink(false);
            display->display_print("-set");
            return;
        }
    }

    if (pos_watch != motor->get_position())
    {
        pos_watch = motor->get_position();
        display->set_blink(false);
    }
    else if (motor->get_mode() == SEMICALIBRATED)
        display->set_blink(true);

    if (motor->get_mode() != UNCALIBRATED)
        display->display_print(motor->get_position());
    else if (motor->get_mode() == UNCALIBRATED)
    {
        display->set_blink(true);
        display->display_print(DISPLAY_NONE);
    }
}

void Keypad::set_preset(uint8_t i)
{
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

void Keypad::goto_preset(uint8_t i)
{
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

Keypad::~Keypad()
{
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
