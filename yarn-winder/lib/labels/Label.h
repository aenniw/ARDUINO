#ifndef ARDUINO_PROJECTS_LABEL_H
#define ARDUINO_PROJECTS_LABEL_H

#include <Display.h>

class Label {
public:
    virtual void print(Display *display, bool nl) const =0;
};

#endif //ARDUINO_PROJECTS_LABEL_H
