#ifndef ARDUINO_PROJECTS_ROOT_KEYPAD_H
#define ARDUINO_PROJECTS_ROOT_KEYPAD_H

#include <Arduino.h>
#include <Service.h>
#include <Motor.h>
#include <Display.h>
#include <EEPROM.h>

class Keypad {
private:
    Motor *motor;

    void pciSetup(byte pin) {
        *digitalPinToPCMSK(pin) |= bit(digitalPinToPCMSKbit(pin));  // enable pin
        PCIFR |= bit(digitalPinToPCICRbit(pin)); // clear any outstanding interrupt
        PCICR |= bit(digitalPinToPCICRbit(pin)); // enable interrupt for the group
    }

public:
    Keypad(Motor *_motor) {
        for (uint8_t i = A0; i <= A5; i++) {
            digitalWrite(i, HIGH); // set pull-up
            pciSetup(i); // set interrupt
        }
        motor = _motor;
    }

    bool any_button_pressed() {
        bool anyPressed = false;
        for (uint8_t i = A0; i <= A5 && !anyPressed; i++) {
            anyPressed = (bool) digitalRead(i) == LOW;
        }
        return anyPressed;
    }

    void handle_manual_control() {
        if (digitalRead(A0) == LOW) {
            motor->dir_ccw();
        } else if (digitalRead(A1) == LOW) {
            motor->dir_cw();
        } else if (digitalRead(A0) && digitalRead(A1)) {
            motor->off();
        }
    }

    void handle_calibration() {
        if (digitalRead(A5) == LOW) {
            switch (table_data.calibration) {
                case UNCALIBRATED:
                    table_data.calibration = SEMICALIBRATED;
                    table_data.position = 0;
#ifdef __DEBUG__
                    Serial.println("Calibration set start.");
#endif
                    break;
                case SEMICALIBRATED:
                    table_data.calibration = CALIBRATED;
                    table_data.end_stop = table_data.position;
                    EEPROM.put(EEPROM.begin(), table_data);
#ifdef __DEBUG__
                    Serial.print("Calibration end-stop");
                    Serial.println(table_data.position);
#endif
                    break;
                default:
                    table_data.calibration = UNCALIBRATED;
                    table_data.end_stop = (unsigned int) (-1);
                    table_data.preset_1 = 0;
                    table_data.preset_2 = 0;
                    table_data.preset_3 = 0;
#ifdef __DEBUG__
                    Serial.println("Calibration clear.");
#endif
                    EEPROM.put(EEPROM.begin(), table_data);
                    break;
            }
        }
    }

    void handle_interrupt() {
        handle_manual_control();
        static unsigned long last_interrupt_time = 0;
        unsigned long interrupt_time = millis();
        // If interrupts come faster than 200ms, assume it's a bounce and ignore
        if (interrupt_time - last_interrupt_time > 200) {
            handle_calibration();
        }
        last_interrupt_time = interrupt_time;
    };
};

#endif //ARDUINO_PROJECTS_ROOT_KEYPAD_H
