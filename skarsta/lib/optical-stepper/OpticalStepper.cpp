#include "OpticalStepper.h"

static Stepper *stepper = nullptr;

OpticalStepper::OpticalStepper(uint8_t _pin1) {
    this->pin1 = _pin1;

    stepper = this;

    attachInterrupt((uint8_t) digitalPinToInterrupt(pin1), []() {
        stepper->step();
    }, CHANGE);
}

OpticalStepper::~OpticalStepper() {
    detachInterrupt((uint8_t) digitalPinToInterrupt(pin1));
}
