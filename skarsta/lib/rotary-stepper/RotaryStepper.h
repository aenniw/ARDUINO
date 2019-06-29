#ifndef ARDUINO_PROJECTS_ROTARYSTEPPER_H
#define ARDUINO_PROJECTS_ROTARYSTEPPER_H

#include <Stepper.h>
#include <Rotary.h>

class RotaryStepper : public Stepper {
private:
    uint8_t pin1, pin2;
public:
    RotaryStepper(uint8_t _pin1, uint8_t _pin2);

    ~RotaryStepper() override;
};

#endif //ARDUINO_PROJECTS_ROTARYSTEPPER_H
