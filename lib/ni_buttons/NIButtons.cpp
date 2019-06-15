#include "NIButtons.h"

static NIButtons *NIButtons::get_instance() {
    static auto *singleton = new NIButtons();
    return singleton;
}

NIButton *NIButtons::add_button(uint8_t gpio) {
    auto button = new NIButton(gpio);
    buttons.push_back(button);
    return button;
}

void NIButtons::cycle() {
    for (const auto &button: buttons) {
        button->cycle();
    }
}

NIButtons::~NIButtons() {
    for (const auto &button: buttons) {
        delete button;
    }
}


NIButton::NIButton(int gpio) {
    this->_gpio = gpio;
    this->_button = new PMButton(gpio);
    this->_button->debounce(DEBOUNCE);
    this->_button->holdTime(500);
    this->_button->begin();
}

NIButton *NIButton::on_short_press(void (*_on_pressed)()) {
    this->_on_short_press = _on_pressed;
    return this;
}

NIButton *NIButton::long_press(long delay) {
    _button->holdTime(delay);
    return this;
}

NIButton *NIButton::on_long_press(void (*_on_pressed)()) {
    this->_on_long_press = _on_pressed;
    return this;
}

NIButton *NIButton::on_press(void (*_on_press)()) {
    this->_on_press = _on_press;
    return this;
}

NIButton *NIButton::on_release(void (*_on_release)()) {
    this->_on_release = _on_release;
    return this;
}

bool NIButton::pressed() {
    return _pressed;
}

bool NIButton::held() {
    return _held;
}

void NIButton::cycle() {
    _button->checkSwitch();
    if (!_pressed && _button->pressed()) {
        _pressed = true;
        if (_on_press) {
            this->_on_press();
        }
#ifdef __DEBUG__
        Serial.print("b: on: ");
        Serial.print(_gpio);
        Serial.println();
#endif
    } else if (digitalRead((uint8_t) _gpio) && _pressed) {
        if (_on_release) {
            this->_on_release();
        }
#ifdef __DEBUG__
        Serial.print("b: off: ");
        Serial.print(_gpio);
        Serial.println();
#endif
        _pressed = false;
        _held = false;
    }

    if (_on_short_press && _button->clicked()) {
        this->_on_short_press();
#ifdef __DEBUG__
        Serial.print("b: short: ");
        Serial.print(_gpio);
        Serial.println();
#endif
    }
    if (_button->held()) {
        if (_on_long_press) {
            this->_on_long_press();
        }
#ifdef __DEBUG__
        Serial.print("b: long: ");
        Serial.print(_gpio);
        Serial.println();
#endif
        _held = true;
    }
}