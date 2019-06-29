#ifndef ARDUINO_PROJECTS_OPTICALSTEPPER_H
#define ARDUINO_PROJECTS_OPTICALSTEPPER_H

#include <Stepper.h>

class OpticalStepper : public Stepper {
private:
    uint8_t pin1;
public:
    explicit OpticalStepper(uint8_t _pin1);

    ~OpticalStepper() override;;
};

#endif //ARDUINO_PROJECTS_OPTICALSTEPPER_H
