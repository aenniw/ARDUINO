#ifndef ARDUINO_PROJECTS_ROOT_DISPLAY_H
#define ARDUINO_PROJECTS_ROOT_DISPLAY_H

#include <Arduino.h>
#include <Service.h>
#include <SevenSegmentTM1637.h>

#define CYCLE_DELAY 100
#define DISPLAY_FADEOUT_TIMEOUT 25000
#define DISPLAY_FADEOUT_DURATION 10000
#define ENCODER_FULL_ROTATION_STEPS (1)

#define MAX(x, y) ((x) >= (y) ? (x) : (y))

extern Table_Data table_data;

class Display : Service {
private:
    const unsigned int fade_end_begin = (DISPLAY_FADEOUT_TIMEOUT) / (CYCLE_DELAY);
    const unsigned int fade_end_step = fade_end_begin + (DISPLAY_FADEOUT_DURATION) / (CYCLE_DELAY);
    const unsigned int fade_step = 100 / (fade_end_step - fade_end_begin);

    unsigned int fadeout_cycle_counter = 0;
    SevenSegmentTM1637 *display = nullptr;


    void display_fading() {
        if (fadeout_cycle_counter >= fade_end_step)
            return;

        fadeout_cycle_counter++;
        if (fadeout_cycle_counter >= fade_end_begin) {
            const uint8_t brightness = 100 - ((fadeout_cycle_counter - fade_end_begin) * fade_step);
#ifdef __DEBUG__
            Serial.print("Backlight: ");
            Serial.print(fade_end_begin);
            Serial.print(" ");
            Serial.print(fade_end_step);
            Serial.print(" ");
            Serial.print(fadeout_cycle_counter);
            Serial.print(" ");
            Serial.println(brightness);
#endif
            display->setBacklight(brightness);
        }
    }

    void display_update() {
        if (table_data.calibration != CALIBRATED) {
            display_light_up();
            if (table_data.calibration == SEMICALIBRATED) {
                display_print(table_data.position);
            } else if (table_data.calibration == UNCALIBRATED) {
                display->print("----");
            }
            display->blink(CYCLE_DELAY, 1);
            return;
        }

        static unsigned int displayed_counter = 0;
        if (displayed_counter != table_data.position) {
#ifdef __DEBUG__
            Serial.print("Position: ");
            Serial.println(table_data.position);
#endif
            display_print(MAX(table_data.position - (table_data.position % ENCODER_FULL_ROTATION_STEPS), 0) /
                          ENCODER_FULL_ROTATION_STEPS);
            displayed_counter = table_data.position;
        }
    }

public:
    Display(uint8_t _pin1, uint8_t _pin2) {
        display = new SevenSegmentTM1637(_pin1, _pin2);
        display->begin();
        display_light_up();

        if (table_data.calibration != CALIBRATED) {
            display->print("----");
        }
    }

    void display_light_up() {
        display->setBacklight(100);
        fadeout_cycle_counter = 0;
    }

    void display_print(unsigned int position) {
        char buffer[5] = {'\0'};
        sprintf(buffer, "%04d", position);
        display->print(buffer);
    }

    void cycle() override {
        display_update();
        //display_fading();
        delay(CYCLE_DELAY);
    }

    ~Display() {
        delete display;
    }
};

#endif //ARDUINO_PROJECTS_ROOT_DISPLAY_H
