#include <Service.h>
#include "OpticalStepper.h"

static Stepper *stepper = nullptr;

OpticalStepper::OpticalStepper(uint8_t _pin1) {
    this->pin1 = _pin1;

    stepper = this;
    attachInterrupt((uint8_t) digitalPinToInterrupt(pin1), []() {
        static unsigned long msg_time = 0;
        if (get_period(msg_time, millis()) >= 5) {
            stepper->step();
        }
        msg_time = millis();
    }, FALLING);
}

OpticalStepper::~OpticalStepper() {
    detachInterrupt((uint8_t) digitalPinToInterrupt(pin1));
}
