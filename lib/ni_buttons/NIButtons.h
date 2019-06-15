#ifndef ARDUINO_PROJECTS_NIBUTTONS_H
#define ARDUINO_PROJECTS_NIBUTTONS_H

#include <vector>
#include <PMButton.h>   // https://github.com/JCWentzel/PolymorphicButtons.git#master
#include <Service.h>

#define DEBOUNCE 5

class NIButton : public Service {
private:
    int _gpio = 0;
    PMButton *_button = nullptr;
    bool _pressed = false, _held = false;

    void (*_on_short_press)() = nullptr;

    void (*_on_long_press)() = nullptr;

    void (*_on_press)() = nullptr;

    void (*_on_release)() = nullptr;

public:
    explicit NIButton(int gpio);

    NIButton *on_short_press(void (*_on_pressed)());

    NIButton *long_press(long delay);

    NIButton *on_long_press(void (*_on_pressed)());

    NIButton *on_press(void (*_on_press)());

    NIButton *on_release(void (*_on_release)());

    bool pressed();

    bool held();

    void cycle() override;
};

class NIButtons : Service {
private:
    std::vector<NIButton *> buttons;

    NIButtons() = default;

public:
    static NIButtons *get_instance();

    NIButton *add_button(uint8_t gpio);

    void cycle() override;

    ~NIButtons();

};


#endif //ARDUINO_PROJECTS_NIBUTTONS_H
