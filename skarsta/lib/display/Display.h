#ifndef ARDUINO_PROJECTS_ROOT_DISPLAY_H
#define ARDUINO_PROJECTS_ROOT_DISPLAY_H

#include <Arduino.h>
#include <Service.h>
#include <SevenSegmentTM1637.h>

#define DISPLAY_FADEOUT_TIMEOUT 25000
#define DISPLAY_FADEOUT_DURATION 10000
#define DISPLAY_BLINK_TIMEOUT 500
#define ENCODER_FULL_ROTATION_STEPS (1)

class Display : Service {
private:
    const unsigned int fade_end_begin = (DISPLAY_FADEOUT_TIMEOUT) / (CYCLE_DELAY);
    const unsigned int fade_end_step = fade_end_begin + (DISPLAY_FADEOUT_DURATION) / (CYCLE_DELAY);
    const unsigned int fade_step = 100 / (fade_end_step - fade_end_begin);

    long displayed_counter = 0, text_cycle_counter = 0;
    unsigned int fadeout_cycle_counter = 0;
    SevenSegmentTM1637 *display = nullptr;

    void display_fading();

    void display_update();
public:
    Display(uint8_t _pin1, uint8_t _pin2);

    void display_light_up();

    void display_print(unsigned int position);

    void display_print(const char *text, unsigned int duration_ms = 1000);

    void cycle() override;

    ~Display() {
        delete display;
    }
};

#endif //ARDUINO_PROJECTS_ROOT_DISPLAY_H
