#ifndef ARDUINO_PROJECTS_DISPLAY_H
#define ARDUINO_PROJECTS_DISPLAY_H

#include <Wire.h>
#include <Adafruit_SSD1306.h>
#include <Service.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 32
#define OLED_RESET 4

class Display : Service {
private:
    Adafruit_SSD1306 *display = nullptr;
public:
    Display();

    void clear();

    void position(int16_t x, int16_t y);

    void set_size(uint8_t s);

    void print(const char *c);

    void println(const char *c);

    void print(int i, int b = 10);

    void cycle() override;

    ~Display();
};


#endif //ARDUINO_PROJECTS_DISPLAY_H
