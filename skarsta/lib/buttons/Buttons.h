#ifndef ARDUINO_PROJECTS_ROOT_BUTTONS_H
#define ARDUINO_PROJECTS_ROOT_BUTTONS_H

#include <Arduino.h>

#define MAX_BUTTONS 10

class Button {
public:
    virtual void isr() = 0;

    virtual ~Button() = default;
};

class ToggleButton : Button {
private:
    uint8_t button;

    void (*on)();

    void (*off)();

public:
    ToggleButton(uint8_t button, void (*on)(), void (*off)());

    void isr() override;
};

class TimedButton : Button {
private:
    volatile unsigned long msg_time = 0;
    volatile bool button_state = false;
    unsigned int delay;
    uint8_t button;

    void (*on)();

    void (*short_press)();

    void (*long_press)();

public:
    TimedButton(uint8_t button, unsigned int delay, void (*short_press)(), void (*long_press)())
            : TimedButton(button, delay, short_press, long_press, nullptr) {}

    TimedButton(uint8_t button, unsigned int delay, void (*short_press)(), void (*long_press)(), void (*on)());

    bool get_state();

    bool is_short();

    void isr() override;
};

#endif //ARDUINO_PROJECTS_ROOT_BUTTONS_H
