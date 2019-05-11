#include "Buttons.h"
#include <limits.h>

#define NO_PIN_NUMBER
#define DISABLE_PCINT_MULTI_SERVICE

#include <PinChangeInt.h>

#define BUTTON_ISR(i) []() { if (buttons[i]) { buttons[i]->isr(); } }
static Button *buttons[MAX_BUTTONS] = {nullptr};
static const PCIntvoidFuncPtr buttonsIsr[MAX_BUTTONS] = {
        BUTTON_ISR(0), BUTTON_ISR(1), BUTTON_ISR(2), BUTTON_ISR(3), BUTTON_ISR(4),
        BUTTON_ISR(5), BUTTON_ISR(6), BUTTON_ISR(7), BUTTON_ISR(8), BUTTON_ISR(9)};

static uint8_t registerIsr(uint8_t button, Button *buttonI) {
    for (uint8_t i = 0; i < MAX_BUTTONS; i++) {
        if (buttons[i] == nullptr) {
            buttons[i] = buttonI;
            pinMode(button, INPUT_PULLUP);
            PCintPort::attachInterrupt(button, buttonsIsr[i], CHANGE);
#ifdef __DEBUG__
            Serial.print("register isr: ");
            Serial.print(i);
            Serial.println();
#endif
            return i;
        }
    }
    return MAX_BUTTONS;
}

static unsigned long get_period(const unsigned long last, const unsigned long next) {
    if (last > next) //overflow
    {
        return (ULONG_MAX - last) + next;
    }
    return next - last;
}

Button::Button(uint8_t button) {
    this->button = button;
    this->isrIndex = registerIsr(button, this);
}

void Button::isr() {
    bool state = PCintPort::pinState == 0;
    _isr(state);
    this->state = state;
}

Button::~Button() {
    if (this->isrIndex == MAX_BUTTONS) {
        return;
    }

    PCintPort::detachInterrupt(button);
    buttons[this->isrIndex] = nullptr;
}

ToggleButton::ToggleButton(uint8_t button, void (*on)(), void (*off)()) : Button(button) {
    this->on = on;
    this->off = off;
}

TimedButton::TimedButton(uint8_t button, unsigned int delay, void (*short_press)(),
                         void (*long_press)(), void (*on)()) : Button(button) {
    this->delay = delay;
    this->long_press = long_press;
    this->short_press = short_press;
    this->on = on;
}

void ToggleButton::_isr(bool state) {
    if (this->state == state) return;
    if (state) {
#ifdef __DEBUG__
        Serial.print("b: on: ");
        Serial.print(button);
        Serial.println();
#endif
        this->on();
    } else {
#ifdef __DEBUG__
        Serial.print("b: off: ");
        Serial.print(button);
        Serial.println();
#endif
        this->off();
    }
}

bool TimedButton::get_state() {
    return state;
}

bool TimedButton::is_short() {
    return get_period(msg_time, millis()) < delay;
}

bool TimedButton::debounce() {
    return get_period(msg_time, millis()) < DEBOUNCE;
}

void TimedButton::_isr(bool state) {
    if (state) {
        if (this->on) this->on();
        msg_time = millis();
        this->state = true;
        return;
    } else if (!this->state) {
        return;
    }

    if (!debounce()) {
        if (is_short()) {
#ifdef __DEBUG__
            Serial.print("b: short: ");
            Serial.print(button);
            Serial.println();
#endif
            this->short_press();
        } else {
#ifdef __DEBUG__
            Serial.print("b: long: ");
            Serial.print(button);
            Serial.println();
#endif
            this->long_press();
        }
    } else {
#ifdef __DEBUG__
        Serial.print("b: debouncing: ");
        Serial.print(button);
        Serial.println();
#endif
    }
    msg_time = millis();
}