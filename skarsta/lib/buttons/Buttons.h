#ifndef ARDUINO_PROJECTS_ROOT_BUTTONS_H
#define ARDUINO_PROJECTS_ROOT_BUTTONS_H

#include <Arduino.h>

#define MAX_BUTTONS 10
#define DEBOUNCE 5

class Button {
protected:
    volatile bool state = false;
    uint8_t button, isrIndex = MAX_BUTTONS;

    virtual void _isr(bool state) = 0;

public:
    explicit Button(uint8_t button);

    virtual void isr();

    virtual ~Button();
};

class ToggleButton : Button {
private:
    void (*on)();

    void (*off)();

protected:
    void _isr(bool state) override;

public:
    ToggleButton(uint8_t button, void (*on)(), void (*off)());
};

class TimedButton : Button {
private:
    volatile unsigned long msg_time = 0;
    unsigned int delay;

    void (*on)();

    void (*short_press)();

    void (*long_press)();

protected:
    void _isr(bool state) override;

    bool debounce();

public:
    TimedButton(uint8_t button, unsigned int delay, void (*short_press)(), void (*long_press)())
            : TimedButton(button, delay, short_press, long_press, nullptr) {}

    TimedButton(uint8_t button, unsigned int delay, void (*short_press)(), void (*long_press)(), void (*on)());

    bool get_state();

    bool is_short();
};

#endif //ARDUINO_PROJECTS_ROOT_BUTTONS_H
