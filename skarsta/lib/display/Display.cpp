#include "Display.h"

#define MAX(x, y) ((x) >= (y) ? (x) : (y))
extern Table_Data table_data;

Display::Display(uint8_t _pin1, uint8_t _pin2) {
    display = new SevenSegmentTM1637(_pin1, _pin2);
    display->begin();
    display_light_up();

    if (table_data.calibration != CALIBRATED) {
        display->print("----");
    }
}

void Display::display_fading() {
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

void Display::display_update() {
    if (table_data.calibration != CALIBRATED) {
        static unsigned int blink_counter = 0;
        if (table_data.calibration == SEMICALIBRATED) {
            display_print(table_data.position);
        } else if (table_data.calibration == UNCALIBRATED) {
            display->print("----");
        }
        if (blink_counter % DISPLAY_BLINK_TIMEOUT == 0) {
            display_light_up();
        } else if (blink_counter % DISPLAY_BLINK_TIMEOUT == DISPLAY_BLINK_TIMEOUT * 0.5) {
            display->setBacklight(0);
        }
        blink_counter++;
        return;
    }

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

void Display::display_light_up() {
    display->setBacklight(100);
    fadeout_cycle_counter = 0;
}

void Display::display_print(unsigned int position) {
    char buffer[5] = {'\0'};
    sprintf(buffer, "%04d", position);
    display->print(buffer);
}

void Display::display_print(const char *text, const unsigned int duration_ms) {
#ifdef __DEBUG__
    Serial.print("Display: ");
        Serial.println(text);
#endif
    display->print(text);
    text_cycle_counter = duration_ms;
    displayed_counter = -1;
}

void Display::cycle() {
    if (!text_cycle_counter) {
        display_update();
        display_fading();
    } else {
        text_cycle_counter -= CYCLE_DELAY;
    }
}