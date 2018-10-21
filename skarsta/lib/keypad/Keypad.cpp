#include "Keypad.h"

static Keypad *static_keypad = nullptr;

ISR(PCINT1_vect) // handle pin change interrupt for A0 to A5 here
{
    if (static_keypad)
        static_keypad->handle_interrupt();
}

Keypad::Keypad(Motor *_motor, Display *_display)
{
#ifdef __EEPROM__
    for (int i = 0; i < 3; i++)
        EEPROM.get(ADDRESS_PRESETS[i], presets[i]);
#endif

    motor = _motor;
    display = _display;

    for (uint8_t i = A0; i <= A5; i++)
    {
        digitalWrite(i, HIGH); // set pull-up
        pciSetup(i);           // set interrupt
    }
    // Needs to be last as interupt setup may triger isr
    static_keypad = this;
}

void Keypad::pciSetup(byte pin)
{
    *digitalPinToPCMSK(pin) |= bit(digitalPinToPCMSKbit(pin)); // enable pin
    PCIFR |= bit(digitalPinToPCICRbit(pin));                   // clear any outstanding interrupt
    PCICR |= bit(digitalPinToPCICRbit(pin));                   // enable interrupt for the group
}

bool Keypad::handle_manual_control(bool down, bool up)
{
    static bool local_control = false;

    if (down)
    {
        motor->set_state(CCW);
    }
    else if (up)
    {
        motor->set_state(CW);
    }
    else if (local_control && !down && !up)
    {
        motor->set_state(OFF);
        local_control = down || up;
        return true;
    }
    local_control = down || up;
    return local_control;
}

bool Keypad::handle_goto_control(bool p_1, bool p_2, bool p_3)
{
    static int8_t goto_button_pressed = -1;
    static long last_goto_interrupt_time = millis();
    unsigned long interrupt_time = millis(),
                  interrupt_diff = ABSD(interrupt_time, last_goto_interrupt_time);

    bool none_pressed = !p_1 && !p_2 && !p_3;
    tst_ = !none_pressed;

    if (none_pressed && goto_button_pressed >= 0)
    {
        if (interrupt_diff < SAVE_BUTTON_TIMEOUT)
        {
            motor->set_position(presets[goto_button_pressed]);
        }
        else
        {
            presets[goto_button_pressed] = motor->get_position();
#ifdef __EEPROM__
            EEPROM.update(ADDRESS_PRESETS[goto_button_pressed], presets[goto_button_pressed]);
#endif
        }
        goto_button_pressed = -1;
    }
    else
    {
        if (goto_button_pressed < 0)
            last_goto_interrupt_time = interrupt_time;
        if (p_1)
            goto_button_pressed = 0;
        if (p_2)
            goto_button_pressed = 1;
        if (p_3)
            goto_button_pressed = 2;
    }

    return !none_pressed;
}

bool Keypad::handle_calibration(bool b)
{
    if (!b)
        return false;

    switch (motor->get_mode())
    {
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
        for (int i = 0; i < 3; i++)
        {
            presets[i] = 0u;
#ifdef __EEPROM__
            EEPROM.update(ADDRESS_PRESETS[i], presets[i]);
#endif
        }
        motor->set_mode(UNCALIBRATED);
        motor->set_end_stop(~0u);
        display->set_blink(true);
        motor->reset_position();
        break;
    }

    return true;
}

void Keypad::handle_interrupt()
{
    static unsigned long last_interrupt_time = 0;
    unsigned long interrupt_time = millis(),
                  interrupt_diff = ABSD(interrupt_time, last_interrupt_time);
    last_interrupt_time = interrupt_time;

    if (handle_manual_control(digitalRead(A0) == LOW,
                              digitalRead(A1) == LOW))
        return;

    if (motor->get_mode() == CALIBRATED &&
        handle_goto_control(digitalRead(A2) == LOW,
                            digitalRead(A3) == LOW,
                            digitalRead(A4) == LOW))
        return;

    if (interrupt_diff > 200)
    {
        handle_calibration(digitalRead(A5) == LOW);
    }
}

void Keypad::cycle()
{
    static unsigned int pos_watch = motor->get_position();
    static unsigned long time_watch = 0;
    if (tst_ && time_watch == 0)
    {
        time_watch = millis();
    }
    if (!tst_)
    {
        time_watch = 0;
    }

    if (time_watch > 0 && ABSD(millis(), time_watch) > SAVE_BUTTON_TIMEOUT)
    {
        display->display_print("-set");
        return;
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
        display->display_print(DISPLAY_NONE);
}