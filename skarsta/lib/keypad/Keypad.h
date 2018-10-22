#ifndef ARDUINO_PROJECTS_ROOT_KEYPAD_H
#define ARDUINO_PROJECTS_ROOT_KEYPAD_H

#include <Arduino.h>
#include <Service.h>
#include <Motor.h>
#include <Display.h>

#ifdef __EEPROM__
#include <EEPROM.h>
#endif

#define SAVE_BUTTON_TIMEOUT 1500
#define DISPLAY_TRIGER_DELAY 100

#ifdef __EEPROM__
const size_t ADDRESS_PRESETS[3] = {
    (ADDRESS_MODE + sizeof(unsigned int)),
    (ADDRESS_MODE + 2 * sizeof(unsigned int)),
    (ADDRESS_MODE + 3 * sizeof(unsigned int))};
#endif

class Keypad : Service
{
private:
  unsigned int presets[3] = {0u};
  Motor *motor;
  Display *display;
  bool tst_ = false;

protected:
  void pciSetup(byte pin);

  bool handle_manual_control(bool down, bool up);

  bool handle_goto_control(bool p_1, bool p_2, bool p_3);

  bool handle_calibration(bool b);

public:
  Keypad(Motor *_motor, Display *_display);

  void handle_interrupt();

  void cycle() override;
};

#endif //ARDUINO_PROJECTS_ROOT_KEYPAD_H
