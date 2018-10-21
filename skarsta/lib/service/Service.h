#ifndef ARDUINO_PROJECTS_ROOT_SERVICE_H
#define ARDUINO_PROJECTS_ROOT_SERVICE_H

#define ABSD(a, b) ((a > b) ? (a - b) : (b - a))

#define DISPLAY_NONE "----"

class Service
{
public:
  virtual void cycle() = 0;
};

#endif //ARDUINO_PROJECTS_ROOT_SERVICE_H
