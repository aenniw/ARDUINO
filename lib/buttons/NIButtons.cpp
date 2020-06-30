#include "NIButtons.h"

NIButton::NIButton(int gpio) : _gpio(gpio), _button(gpio) {
    this->_button.debounce(DEBOUNCE);
    this->_button.holdTime(500);
    this->_button.begin();
}

bool NIButton::begin() {
    this->_button.begin();
    return true;
}

NIButton *NIButton::on_short_press(void (*_on_pressed)()) {
    this->_on_short_press = _on_pressed;
    return this;
}

NIButton *NIButton::long_press(long delay) {
    _button.holdTime(delay);
    return this;
}

NIButton *NIButton::llong_press(long delay) {
    _button.longHoldTime(delay);
    return this;
}

NIButton *NIButton::on_long_press(void (*_on_pressed)()) {
    this->_on_long_press = _on_pressed;
    return this;
}

NIButton *NIButton::on_llong_press(void (*_on_pressed)()) {
    this->_on_llong_press = _on_pressed;
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
    _button.checkSwitch();
    if (!_pressed && _button.pressed()) {
        _pressed = true;
        if (_on_press) {
            this->_on_press();
        }
#ifdef __DEBUG__
        Serial.print(millis());
        Serial.print(F("\t| b |: on: "));
        Serial.println(_gpio);
#endif
    } else if (digitalRead((uint8_t) _gpio) && _pressed) {
        if (_on_release) {
            this->_on_release();
        }
#ifdef __DEBUG__
        Serial.print(millis());
        Serial.print(F("\t| b |: off: "));
        Serial.println(_gpio);
#endif
        _pressed = false;
        _held = false;
    }

    if (_on_short_press && _button.clicked()) {
        this->_on_short_press();
#ifdef __DEBUG__
        Serial.print(millis());
        Serial.print(F("\t| b |: short: "));
        Serial.println(_gpio);
#endif
    }
    if (_button.held()) {
        if (_on_long_press) {
            this->_on_long_press();
        }
#ifdef __DEBUG__
        Serial.print(millis());
        Serial.print(F("\t| b |: long: "));
        Serial.println(_gpio);
#endif
        _held = true;
    }
    if (_button.heldLong()) {
        if (_on_llong_press) {
            this->_on_llong_press();
        }
#ifdef __DEBUG__
        Serial.print(millis());
        Serial.print(F("\t| b |: llong: "));
        Serial.println(_gpio);
#endif
        _held = true;
    }
}