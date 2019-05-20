#ifndef ARDUINO_PROJECTS_LABEL_H
#define ARDUINO_PROJECTS_LABEL_H

#include <Display.h>
#include <Configuration.h>

class Label {
public:
    virtual void print(LOCALE locale, Display *display, bool nl) const =0;
};

#endif //ARDUINO_PROJECTS_LABEL_H
