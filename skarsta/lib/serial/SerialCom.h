#pragma once

#include <Arduino.h>
#include <Service.h>
#include <Motor.h>
#include <Display.h>
#include <Calibrator.h>

#include <SoftwareSerial.h>

const byte ixPin = 11;
const byte txPin = 12;

class SerialCom : public Service {
private:
    bool disabled = false;
    unsigned int pos_watch = 0;
    unsigned long last_update = 0;
    String readString = "";
    Motor *motor = nullptr;
    Display *display = nullptr;
    Calibrator *calibrator = nullptr;
    uint8_t ixPin;
    uint8_t txPin;


    SoftwareSerial *mySerial;

    void goto_preset(unsigned int preset);

    void send_motor_position();
    void SerialCom::set_motor(unsigned int pos);

    String read_line();
protected:


public:
    SerialCom(Motor *motor, Calibrator *calibrator, uint8_t ixPin, uint8_t txPin);

    bool begin() override;

    void disable() override;

    void cycle() override;
};