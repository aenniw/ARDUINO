#pragma once

#include <Display.h>

class Label {
public:
    virtual void print(Display &display, bool nl) const = 0;
    virtual void print(Display &display, char sp, const char *p, const char *s) const {
        if (p)display.print(p);
        print(display, s == nullptr);
        if (s)display.println(s);
    };
};