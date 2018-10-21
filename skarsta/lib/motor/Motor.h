#ifndef ARDUINO_PROJECTS_ROOT_MOTOR_H
#define ARDUINO_PROJECTS_ROOT_MOTOR_H

#include <Arduino.h>
#include <Service.h>

#define DIRECTION_CHANGE_DELAY 200

typedef enum
{
  CCW,
  CW,
  OFF
} MotorState;

typedef enum
{
  UNCALIBRATED,   // nothing calibrated
  SEMICALIBRATED, // bottom calibrated
  CALIBRATED      // bottom-top calibrated
} MotorMode;

#ifdef __EEPROM__
#define ADDRESS_POSITION 0
#define ADDRESS_END_STOP (ADDRESS_POSITION + sizeof(unsigned int))
#define ADDRESS_MODE (ADDRESS_END_STOP + sizeof(MotorMode))
#endif

class Motor : Service
{
private:
  unsigned int end_stop = ~0u, position = 0;
  MotorMode mode = UNCALIBRATED;

  unsigned int next_position = -1;
  uint8_t power_pin, dir_pin = 0;
  MotorState state = OFF, next_state = OFF;

protected:
  void off();

  void dir_cw();

  void dir_ccw();

  void update_position(const unsigned char result);

public:
  Motor(uint8_t _pin1, uint8_t _pin2, uint8_t _pin3, uint8_t _pin4);

  void set_end_stop(unsigned int end_stop);

  unsigned int get_position();
  void reset_position();
  void set_position(unsigned int pos);

  MotorState get_state();
  void set_state(MotorState state);

  MotorMode get_mode();
  void set_mode(MotorMode state);

  void cycle() override;

  ~Motor();
};

#endif //ARDUINO_PROJECTS_ROOT_MOTOR_H
