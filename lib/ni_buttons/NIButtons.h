#ifndef ARDUINO_PROJECTS_NIBUTTONS_H
#define ARDUINO_PROJECTS_NIBUTTONS_H

#include <vector>
#include <PMButton.h>
#include <Service.h>

class NIButton : public PMButton, Service {
private:
    void (*_on_pressed)() = nullptr;

public:
    explicit NIButton(int gpio) : PMButton(gpio) {
    }

    NIButton *on_pressed(void (*_on_pressed)()) {
        this->_on_pressed = _on_pressed;
        return this;
    }

    void cycle() override {
        checkSwitch();
        if (_on_pressed && this->pressed()) {
            this->_on_pressed();
        }
    }
};

class NIButtons : Service {
private:
    std::vector<NIButton *> buttons;

    NIButtons() = default;

public:

    static NIButtons *get_instance() {
        static auto *singleton = new NIButtons();
        return singleton;
    }

    NIButton *add_button(uint8_t gpio) {
        auto button = new NIButton(gpio);
        button->begin();
        buttons.push_back(button);
        return button;
    }

    void cycle() override {
        for (const auto &button: buttons) {
            button->cycle();
        }
    }

    ~NIButtons() {
        for (const auto &button: buttons) {
            delete button;
        }
    }

};


#endif //ARDUINO_PROJECTS_NIBUTTONS_H
