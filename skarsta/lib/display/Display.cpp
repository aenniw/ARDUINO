#include "Display.h"

static int8_t codes_numbers[] = {0x3f, 0x06, 0x5b, 0x4f, 0x66,
                                 0x6d, 0x7d, 0x07, 0x7f, 0x6f};

static int8_t get_code_n(uint8_t n)
{
    return n > 9 ? 0x00 : codes_numbers[n];
}

static int8_t get_code_c(char c)
{
    if (c >= 'A' && c <= 'Z')
    {
        c = c + 32;
    }
    switch (c)
    {
    case '-':
        return B01000000;
    case 's':
        return B01101101;
    case 'e':
        return B01111001;
    case 'r':
        return B01010000;
    case 't':
        return B01111000;
    default:
        return 0x00;
    }
}

Display::Display(uint8_t _pin1, uint8_t _pin2)
{
    display = new TM1637(_pin1, _pin2);
    display->set();
}

void Display::set_blink(bool state)
{
#ifdef __DEBUG__
    if (blink != state)
    {
        Serial.print("blink=");
        Serial.print(state);
        Serial.println();
    }
#endif
    dirty = blink != state && !state;
    blink = state;
}

void Display::display_print(unsigned int position)
{
    int8_t buffer[4] = {
        get_code_n(position / 1000),
        get_code_n((position / 100) % 10),
        get_code_n((position % 100) / 10),
        get_code_n(position % 10)};
    for (uint8_t i = 0u; i < 4; i++)
    {
        dirty = dirty || disp_buffer[i] != buffer[i];
        disp_buffer[i] = buffer[i];
    }

#ifdef __DEBUG__
    if (dirty)
    {
        Serial.print("print: ");
        Serial.println(position);
    }
#endif
}

void Display::display_print(const char *text)
{
    const uint8_t len = strlen(text);
    int8_t buffer[4] = {
        len > 0 ? get_code_c(text[0]) : 0x00,
        len > 1 ? get_code_c(text[1]) : 0x00,
        len > 2 ? get_code_c(text[2]) : 0x00,
        len > 3 ? get_code_c(text[3]) : 0x00};
    for (uint8_t i = 0u; i < 4; i++)
    {
        dirty = dirty || disp_buffer[i] != buffer[i];
        disp_buffer[i] = buffer[i];
    }

#ifdef __DEBUG__
    if (dirty)
    {
        Serial.print("print: ");
        Serial.println(text);
    }
#endif
}

void Display::cycle()
{
    static unsigned long last_tick = millis();

    if (dirty)
    {
        display->display(disp_buffer, true);
        dirty = false;

#ifdef __DEBUG__
        Serial.println("redraw");
#endif
    }
    else if (blink)
    {
        unsigned long now = millis(),
                      diff = get_period(last_tick, now);

        if (diff >= 500)
        {
#ifdef __DEBUG__
            Serial.print("blink toggle: ");
            Serial.print(last_tick);
            Serial.print(", ");
            Serial.print(now);
            Serial.print(", ");
            Serial.print(diff);
            Serial.println();
#endif
            if (!clear)
            {
                display->clearDisplay();
            }
            else
            {
                display->display(disp_buffer, true);
            }
            clear = !clear;
            last_tick = now;
        }
    }
}