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
    this->blink = state;
    this->dirty = true;
}

void Display::display_print(unsigned int position)
{
    disp_buffer[3] = get_code_n(position % 10);
    disp_buffer[2] = get_code_n((position % 100) / 10);
    disp_buffer[1] = get_code_n((position / 100) % 10);
    disp_buffer[0] = get_code_n(position / 1000);
    dirty = true;
}

void Display::display_print(const char *text)
{
    const uint8_t len = strlen(text);
    disp_buffer[0] = len > 0 ? get_code_c(text[0]) : 0x00;
    disp_buffer[1] = len > 1 ? get_code_c(text[1]) : 0x00;
    disp_buffer[2] = len > 2 ? get_code_c(text[2]) : 0x00;
    disp_buffer[3] = len > 3 ? get_code_c(text[3]) : 0x00;
    dirty = true;
}

void Display::cycle()
{
    static unsigned long last_tick = millis();

    if (!blink && dirty)
    {
        display->display(disp_buffer, true);
        dirty = false;
    }

    if (blink && ABSD(millis(), last_tick) >= 500)
    {
        if (!clear)
        {
            display->clearDisplay();
        }
        else
        {
            display->display(disp_buffer, true);
        }
        clear = !clear;
        last_tick = millis();
    }
}