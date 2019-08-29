#include "Display.h"
#include "Logo.h"

#ifdef __EEPROM__
#include <Configuration.h>
#endif

Display::Display(uint8_t bck, uint8_t sclk, uint8_t din, uint8_t dc, uint8_t cs, uint8_t rst) {
    display = new Adafruit_PCD8544(sclk, din, dc, cs, rst);
    display->begin();
    display->setRotation(2);
    display->setTextColor(BLACK);
    display->setContrast(50);
    display->setTextSize(1);
    pinMode((this->bck = bck), OUTPUT);
#ifdef __EEPROM__
    EEPROM.get(ADDRESS_BRIGHTNESS, _backlight);
#endif
    this->set_backlight(_backlight);

    display->clearDisplay();
    display->drawBitmap(0, 0, logo_bitmap, logo_width, logo_height, BLACK);
    display->display();
    delay(1500);

    display->clearDisplay();
    display->display();
}

void Display::clear() {
    display->setCursor(0, 0);
    display->clearDisplay();
}

void Display::set_backlight(uint8_t v) {
    analogWrite(bck, 255 - (uint8_t) (2.55f * (this->_backlight = v)));
#ifdef __EEPROM__
    eeprom_set(ADDRESS_BRIGHTNESS, (uint8_t) this->_backlight);
#endif
}

void Display::increase_backlight() {
    if (_backlight < 100) {
        set_backlight(_backlight + 1);
    }
}

void Display::decrease_backlight() {
    if (_backlight > 0) {
        set_backlight(_backlight - 1);
    }
}

uint8_t *Display::get_backlight() {
    return &_backlight;
}

void Display::position(int16_t x, int16_t y) {
    display->setCursor(x, y);
}

void Display::set_size(uint8_t s) {
    display->setTextSize(s);
}

void Display::print(const char c) {
    display->print(c);
}

void Display::print(const __FlashStringHelper *ifsh) {
    display->print(ifsh);
}

void Display::println() {
    display->println();
}

void Display::println(const __FlashStringHelper *ifsh) {
    display->println(ifsh);
}

void Display::print(int i, int b) {
    display->print(i, b);
}

void Display::print(long i, int b) {
    display->print(i, b);
}

void Display::print(unsigned long i, int b) {
    display->print(i, b);
}

void Display::print(double i, int d) {
    display->print(i, d);
}

void Display::cycle() {
    display->display();
}

Display::~Display() {
    delete display;
}
