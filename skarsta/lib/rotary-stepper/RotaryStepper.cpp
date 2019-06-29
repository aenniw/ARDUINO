#include "RotaryStepper.h"

static Stepper *stepper = nullptr;
static Rotary *encoder= nullptr;

RotaryStepper::RotaryStepper(uint8_t _pin1, uint8_t _pin2) {
    this->pin1 = _pin1;
    this->pin2 = _pin2;

    stepper = this;

    encoder = new Rotary((char) (pin1), (char) pin2);
    auto interrupt_routine = []() {
        unsigned char step = encoder->process();
        if (stepper != nullptr && (step == DIR_CW || step == DIR_CCW)) {
            stepper->step();
        }
    };
    attachInterrupt((uint8_t) digitalPinToInterrupt(pin1), interrupt_routine, CHANGE);
    attachInterrupt((uint8_t) digitalPinToInterrupt(pin2), interrupt_routine, CHANGE);
}

RotaryStepper::~RotaryStepper() {
    detachInterrupt((uint8_t) digitalPinToInterrupt(pin1));
    detachInterrupt((uint8_t) digitalPinToInterrupt(pin2));
    delete encoder;
}
