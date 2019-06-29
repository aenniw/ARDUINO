#ifndef ARDUINO_PROJECTS_STEPPER_H
#define ARDUINO_PROJECTS_STEPPER_H

#include <Arduino.h>

class Stepper {
protected:
    void (*_on_step)() = nullptr;

public:

    void step() {
        if (_on_step != nullptr) {
            _on_step();
        }
    }

    void on_step(void (*_on_step())) {
        this->_on_step = _on_step;
    }

    virtual ~Stepper() = default;
};

#endif //ARDUINO_PROJECTS_STEPPER_H
