#include "NIButtons.h"

NIButton::NIButton(int gpio, uint16_t db) : _gpio(gpio), _button(gpio), debounce(db) {
    this->_button.debounce(debounce);
    this->_button.holdTime(500);
    this->_button.begin();
}

bool NIButton::begin() {
    this->_button.begin();
    return true;
}

NIButton &NIButton::on_short_press(void (*_on_pressed)()) {
    this->_on_short_press = _on_pressed;
    return *this;
}

NIButton &NIButton::long_press(long delay) {
    _button.holdTime(delay);
    return *this;
}

NIButton &NIButton::llong_press(long delay) {
    _button.longHoldTime(delay);
    return *this;
}

NIButton &NIButton::on_long_press(void (*_on_pressed)()) {
    this->_on_long_press = _on_pressed;
    return *this;
}

NIButton &NIButton::on_llong_press(void (*_on_pressed)()) {
    this->_on_llong_press = _on_pressed;
    return *this;
}

NIButton &NIButton::on_press(void (*_on_press)()) {
    this->_on_press = _on_press;
    return *this;
}

NIButton &NIButton::on_release(void (*_on_release)()) {
    this->_on_release = _on_release;
    return *this;
}

bool NIButton::pressed() {
    return _pressed;
}

bool NIButton::held() {
    return _held;
}

bool NIButton::held_long() {
    return _held_long;
}

void NIButton::cycle() {
    _button.checkSwitch();

    if (!_pressed && _button.pressed()) {
        elapsed = 0;
        _pressed = true;
        if (_on_press) {
            this->_on_press();
        }
        LOG("b | on:%d", _gpio);
    } else if (digitalRead((uint8_t) _gpio) && _pressed && elapsed > debounce) {
        if (_on_release) {
            this->_on_release();
        }
        LOG("b | off:%d", _gpio);
        _pressed = false;
        _held_long = false;
        _held = false;
        elapsed = 0;
    }

    if (_on_short_press && _button.clicked()) {
        this->_on_short_press();
        LOG("b | short:%d", _gpio);
    }
    if (_button.held()) {
        if (_on_long_press) {
            this->_on_long_press();
        }
        LOG("b | long:%d", _gpio);
        _held = true;
    }
    if (_button.heldLong()) {
        if (_on_llong_press) {
            this->_on_llong_press();
        }
        LOG("b | llong:%d", _gpio);
        _held_long = true;
    }
}