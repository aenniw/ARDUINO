#include "Display.h"

static int8_t codes_numbers[] = {0x3f, 0x06, 0x5b, 0x4f, 0x66,
                                 0x6d, 0x7d, 0x07, 0x7f, 0x6f};
static char error_msg[5] = {'e', 'r', ' ', '\0', '\0'};

static int8_t get_code_n(uint8_t n) {
    return n > 9 ? 0x00 : codes_numbers[n];
}

static int8_t get_code_c(char c) {
    if (c >= '0' && c <= '9') {
        return get_code_n((uint8_t) (c - '0'));
    }
    if (c >= 'A' && c <= 'Z') {
        c = static_cast<char>(c + 32);
    }
    switch (c) {
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

Display::Display(uint8_t _pin1, uint8_t _pin2, uint16_t _timeout) :
        display(_pin1, _pin2), timeout(_timeout) {
    display.set(brightness);
}

bool Display::begin() {
    display.clearDisplay();
    for (uint8_t i = 4; i > 0; i--) {
        display.display(i - 1, get_code_n(8), true);
        delay(200);
    }
    display.clearDisplay();
    return true;
}

void Display::set_blink(bool state) {
    if (disabled) return;
    if (blink != state) {
        LOG("d | blink=%d", state);
    }
    dirty = blink != state && !state;
    blink = state;
}

void Display::print(unsigned int position) {
    if (disabled) return;
    int8_t buffer[4] = {
            get_code_n(position / 1000),
            get_code_n((position / 100) % 10),
            get_code_n((position % 100) / 10),
            get_code_n(position % 10)};
    for (uint8_t i = 0u; i < 4; i++) {
        dirty = dirty || disp_buffer[i] != buffer[i];
        disp_buffer[i] = buffer[i];
    }

    if (dirty) {
        LOG("d | print:%d", position);
    }
}

void Display::print(const char *text) {
    if (disabled) return;
    const uint8_t len = strlen(text);
    int8_t buffer[4] = {
            len > 0 ? get_code_c(text[0]) : 0x00,
            len > 1 ? get_code_c(text[1]) : 0x00,
            len > 2 ? get_code_c(text[2]) : 0x00,
            len > 3 ? get_code_c(text[3]) : 0x00};
    for (uint8_t i = 0u; i < 4; i++) {
        dirty = dirty || disp_buffer[i] != buffer[i];
        disp_buffer[i] = buffer[i];
    }

    if (dirty) {
        LOG("d | print:%s", text);
    }
}

void Display::set_brightness(uint8_t b) {
    if (brightness == b)
        return;
    brightness = b;
    LOG("d | brightness:%d", brightness);
    display.set(b);
}

void Display::light_up() {
    if (!this->blink)
        this->dirty = true;
}

void Display::cycle() {
    if (dirty) {
        this->set_brightness(BRIGHT_HIGH);
        display.display(disp_buffer, true);
        dirty = false;
        elapsed = 0;
        LOG("d | redraw");
    } else if (blink) {
        if (elapsed >= 100) {
            if (!clear) {
                display.clearDisplay();
            } else {
                this->set_brightness(BRIGHT_HIGH);
                display.display(disp_buffer, true);
            }
            clear = !clear;
            elapsed = 0;
        }
    } else if (!disabled && brightness != 0 && elapsed >= timeout) {
        this->set_brightness((uint8_t) (8 - ((elapsed - timeout) / 10000)));
        if (brightness != 0)
            display.display(disp_buffer, true);
        else
            display.clearDisplay();
    }
}

void Display::disable(uint8_t cause) {
    error_msg[3] = (char) ('0' + (cause % 10));
    print(error_msg);
    disabled = true;
}
