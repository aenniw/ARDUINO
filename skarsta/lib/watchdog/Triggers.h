#pragma once

typedef enum {
    STOPPED = 1, STARTED = 2, INIT = 3
} ErrorType;

class Trigger {
public:
    virtual bool trip(ErrorType t) = 0;

    virtual void reset() = 0;
};