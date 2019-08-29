#ifndef ARDUINO_PROJECTS_DISPLAY_H
#define ARDUINO_PROJECTS_DISPLAY_H

#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_PCD8544.h>
#include <Service.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 32

class Display : Service {
private:
    uint8_t bck, _backlight = 255;
    Adafruit_PCD8544 *display = nullptr;
protected:
    void set_backlight(uint8_t v);

public:
    Display(uint8_t bck, uint8_t sclk, uint8_t din, uint8_t dc, uint8_t cs, uint8_t rst);

    void clear();

    uint8_t *get_backlight();

    void increase_backlight();

    void decrease_backlight();

    void position(int16_t x, int16_t y);

    void set_size(uint8_t s);

    void print(char c);

    void print(const __FlashStringHelper *ifsh);

    void println();

    void println(const __FlashStringHelper *ifsh);

    void print(int i, int b = 10);

    void print(long i, int b = 10);

    void print(unsigned long i, int b = 10);

    void print(double i, int d = 2);

    void cycle() override;

    ~Display();
};


#endif //ARDUINO_PROJECTS_DISPLAY_H
