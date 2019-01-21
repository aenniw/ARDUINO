#include "Buttons.h"
#include <PinChangeInt.h>
#include <Service.h>

#define BUTTON_ISR(i) []() { if (buttons[i]) { buttons[i]->isr(); } }
static Button *buttons[MAX_BUTTONS] = {nullptr};
static const PCIntvoidFuncPtr buttonsIsr[MAX_BUTTONS] = {
        BUTTON_ISR(0), BUTTON_ISR(1), BUTTON_ISR(2), BUTTON_ISR(3), BUTTON_ISR(4),
        BUTTON_ISR(5), BUTTON_ISR(6), BUTTON_ISR(7), BUTTON_ISR(8), BUTTON_ISR(9)};

static void registerIsr(uint8_t button, Button *buttonI) {
    PCIntvoidFuncPtr isr = nullptr;
    for (uint8_t i = 0; i < MAX_BUTTONS; i++) {
        if (buttons[i] == nullptr) {
            buttons[i] = buttonI;
            isr = buttonsIsr[i];

#ifdef __DEBUG__
            Serial.print("register isr: ");
            Serial.print(i);
            Serial.println();
#endif
            break;
        }
    }
    pinMode(button, INPUT_PULLUP);
    PCintPort::attachInterrupt(button, isr, CHANGE);
}

ToggleButton::ToggleButton(uint8_t button, void (*on)(), void (*off)()) {
    this->button = button;
    this->on = on;
    this->off = off;
    registerIsr(button, this);
}

TimedButton::TimedButton(uint8_t button, unsigned int delay, void (*short_press)(), void (*long_press)(),
                         void (*on)()) {
    this->button = button;
    this->delay = delay;
    this->long_press = long_press;
    this->short_press = short_press;
    this->on = on;
    registerIsr(button, this);
}

void ToggleButton::isr() {
    bool state = digitalRead(button) == LOW;

    if (state) {
        this->on();
    } else {
        this->off();
    }
}

bool TimedButton::get_state() {
    return button_state;
}

bool TimedButton::is_short() {
    return get_period(msg_time, millis()) < delay;
}

void TimedButton::isr() {
    // TODO: move to class def
    static bool last_state = false;
    button_state = digitalRead(button) == LOW;
    if (button_state) {
        if (this->on) this->on();
        msg_time = millis();
        last_state = true;
        return;
    } else if (!last_state) {
        button_state = false;
        return;
    }

    if (is_short()) {
        this->short_press();
    } else {
        this->long_press();
    }
    last_state = false;
    msg_time = millis();
}
