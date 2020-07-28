#ifndef ARDUINO_PROJECTS_NIBUTTONS_H
#define ARDUINO_PROJECTS_NIBUTTONS_H

#include <vector>
#include <PMButton.h>
#include <Service.h>

class NIButton : public TimedService {
private:
    const int _gpio;
    PMButton _button;
    const uint16_t debounce;
    bool _pressed = false, _held = false, _held_long = false;

    void (*_on_short_press)() = nullptr;

    void (*_on_long_press)() = nullptr;

    void (*_on_llong_press)() = nullptr;

    void (*_on_press)() = nullptr;

    void (*_on_release)() = nullptr;

public:
    explicit NIButton(int gpio, uint16_t debounce = 20);

    bool begin() override;

    NIButton &on_short_press(void (*_on_pressed)());

    NIButton &long_press(long delay);

    NIButton &llong_press(long delay);

    NIButton &on_llong_press(void (*_on_pressed)());

    NIButton &on_long_press(void (*_on_pressed)());

    NIButton &on_press(void (*_on_press)());

    NIButton &on_release(void (*_on_release)());

    bool pressed();

    bool held();

    bool held_long();

    void cycle() override;
};


#endif //ARDUINO_PROJECTS_NIBUTTONS_H
