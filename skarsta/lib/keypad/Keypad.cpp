#include "Keypad.h"

extern Table_Data table_data;
static Keypad *static_keypad;

ISR(PCINT1_vect) // handle pin change interrupt for A0 to A5 here
{
    static_keypad->handle_interrupt();
}

Keypad::Keypad(Motor *_motor, Display *_display)
{
    for (uint8_t i = A0; i <= A5; i++)
    {
        digitalWrite(i, HIGH); // set pull-up
        pciSetup(i);           // set interrupt
    }
    motor = _motor;
    display = _display;
    static_keypad = this;
}

void Keypad::pciSetup(byte pin)
{
    *digitalPinToPCMSK(pin) |= bit(digitalPinToPCMSKbit(pin)); // enable pin
    PCIFR |= bit(digitalPinToPCICRbit(pin));                   // clear any outstanding interrupt
    PCICR |= bit(digitalPinToPCICRbit(pin));                   // enable interrupt for the group
}

void Keypad::handle_manual_control()
{
    if (digitalRead(A0) == LOW)
    {
        motor->dir_ccw();
        manual_control_active = true;
    }
    else if (digitalRead(A1) == LOW)
    {
        motor->dir_cw();
        manual_control_active = true;
    }
    else if (manual_control_active && digitalRead(A0) && digitalRead(A1))
    {
        motor->off();
        manual_control_active = false;
    }
}

void Keypad::handle_goto_control()
{
    if (table_data.calibration != CALIBRATED)
    {
        return;
    }
    bool states[] = {(bool)digitalRead(A2), (bool)digitalRead(A3), (bool)digitalRead(A4)},
         allHigh = states[0] && states[1] && states[2];

    unsigned long interrupt_time = millis();

    if (allHigh && goto_button_pressed >= 0)
    {
        if (interrupt_time - last_goto_interrupt_time < SAVE_BUTTON_TIMEOUT)
        {
#ifdef __DEBUG__
            Serial.print("Pos ");
            Serial.print(goto_button_pressed);
            Serial.println(" goto.");
#endif
            switch (goto_button_pressed)
            {
            case 0:
                motor->goto_position(table_data.preset_1);
                break;
            case 1:
                motor->goto_position(table_data.preset_2);
                break;
            case 2:
                motor->goto_position(table_data.preset_3);
                break;
            default:
#ifdef __DEBUG__
                Serial.println("Unknown button pressed");
#endif
                break;
            }
        }
        else
        {
#ifdef __DEBUG__
            Serial.print("Pos ");
            Serial.print(goto_button_pressed);
            Serial.println(" set.");
#endif
            switch (goto_button_pressed)
            {
            case 0:
                table_data.preset_1 = table_data.position;
                break;
            case 1:
                table_data.preset_2 = table_data.position;
                break;
            case 2:
                table_data.preset_3 = table_data.position;
                break;
            default:
#ifdef __DEBUG__
                Serial.println("Unknown button pressed");
#endif
                break;
            }
#ifdef __EEPROM__
            EEPROM.put(EEPROM.begin(), table_data);
#endif
            display->display_print(" SET");
        }
        goto_button_pressed = -1;
    }
    else
    {
        for (uint8_t i = 0; i < 3; i++)
        {
            if (!states[i])
                goto_button_pressed = i;
        }
    }
    last_goto_interrupt_time = interrupt_time;
}

void Keypad::handle_calibration()
{
    if (digitalRead(A5) == LOW)
    {
        switch (table_data.calibration)
        {
        case UNCALIBRATED:
            table_data.calibration = SEMICALIBRATED;
            table_data.position = 0;
#ifdef __DEBUG__
            Serial.println("Calibration set start.");
#endif
            break;
        case SEMICALIBRATED:
            table_data.calibration = CALIBRATED;
            table_data.end_stop = table_data.position;
#ifdef __EEPROM__
            EEPROM.put(EEPROM.begin(), table_data);
#endif
#ifdef __DEBUG__
            Serial.print("Calibration end-stop ");
            Serial.println(table_data.position);
#endif
            break;
        default:
            table_data.calibration = UNCALIBRATED;
            table_data.end_stop = ~0u;
            table_data.preset_1 = 0;
            table_data.preset_2 = 0;
            table_data.preset_3 = 0;
#ifdef __DEBUG__
            Serial.println("Calibration clear.");
#endif
#ifdef __EEPROM__
            EEPROM.put(EEPROM.begin(), table_data);
#endif
            break;
        }
    }
}

void Keypad::handle_interrupt()
{
    {
        display->display_light_up();
        handle_goto_control();
        handle_manual_control();
        static unsigned long last_interrupt_time = 0;
        unsigned long interrupt_time = millis();
        // If interrupts come faster than 200ms, assume it's a bounce and ignore
        if (interrupt_time - last_interrupt_time > 200)
        {
            handle_calibration();
        }
        last_interrupt_time = interrupt_time;
    };
}

void Keypad::cycle()
{
    if (goto_button_pressed >= 0 && millis() - last_goto_interrupt_time >= SAVE_BUTTON_TIMEOUT)
    {
        display->display_print(" SET");
    }
}