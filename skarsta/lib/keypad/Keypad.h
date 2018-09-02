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

class Keypad : Service
{
private:
  bool manual_control_active = false;
  int goto_button_pressed = -1;
  unsigned long last_goto_interrupt_time = millis();

  Motor *motor;
  Display *display;

  void pciSetup(byte pin);

public:
  Keypad(Motor *_motor, Display *_display);

  void handle_manual_control();

  void handle_goto_control();

  void handle_calibration();

  void handle_interrupt();

  void cycle() override;
};

#endif //ARDUINO_PROJECTS_ROOT_KEYPAD_H
