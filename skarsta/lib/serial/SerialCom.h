#pragma once

#include <Arduino.h>
#include <Service.h>
#include <Calibrator.h>
#ifndef __SERIAL_COM_HW__
#include <SoftwareSerial.h>
#endif

class SerialCom : public TimedService {
private:
#ifdef __SERIAL_COM_HW__
    HardwareSerial &serial;
#else
    SoftwareSerial serial;
#endif
    Motor *motor = nullptr;
    Calibrator *calibrator = nullptr;
    unsigned long baud;
protected:
    unsigned int send_motor_position();

    String get_command_line();

public:
    SerialCom(Motor *motor, Calibrator *calibrator, unsigned long baud, uint8_t rx, uint8_t tx) :
#ifdef __SERIAL_COM_HW__
        serial(Serial), motor(motor), calibrator(calibrator), baud(baud) {}
#else
        serial(rx, tx), motor(motor), calibrator(calibrator), baud(baud) {};
#endif

    bool begin() override;

    void cycle() override;
};