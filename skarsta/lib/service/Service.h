#ifndef ARDUINO_PROJECTS_ROOT_SERVICE_H
#define ARDUINO_PROJECTS_ROOT_SERVICE_H

#define DISPLAY_NONE "----"

class Service
{
public:
  virtual void cycle() = 0;
};

unsigned long get_period(const unsigned long last, const unsigned long next);

#endif //ARDUINO_PROJECTS_ROOT_SERVICE_H
