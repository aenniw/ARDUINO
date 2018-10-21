#ifndef ARDUINO_PROJECTS_ROOT_DISPLAY_H
#define ARDUINO_PROJECTS_ROOT_DISPLAY_H

#include <Arduino.h>
#include <Service.h>
#include <TM1637.h>

class Display : Service
{
private:
  TM1637 *display = nullptr;
  int8_t disp_buffer[4] = {0x00, 0x00, 0x00, 0x00};
  bool dirty = false, blink = false, clear = false;

public:
  Display(uint8_t _pin1, uint8_t _pin2);

  void set_blink(bool state);

  void display_print(unsigned int position);

  void display_print(const char *text);

  void cycle() override;

  ~Display()
  {
    delete display;
  }
};

#endif //ARDUINO_PROJECTS_ROOT_DISPLAY_H
