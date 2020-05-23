#include "Display.h"
#include "Logo.h"

#ifdef __EEPROM__
#include <Configuration.h>
#endif

Display::Display(uint8_t bck, uint8_t sclk, uint8_t din, uint8_t dc, uint8_t cs, uint8_t rst) :
        display(sclk, din, dc, cs, rst) {
    pinMode((this->bck = bck), OUTPUT);
#ifdef __EEPROM__
    uint8_t data;
    EEPROM.get(ADDRESS_BRIGHTNESS, data);
    this->backlight = data;
    EEPROM.get(ADDRESS_LOCALE, data);
    this->locale = (LOCALE) data;
#endif
    this->set_backlight(backlight);
}

bool Display::begin() {
    display.begin();
    display.setRotation(2);
    display.setTextColor(BLACK);
    display.setContrast(50);
    display.setTextSize(1);

    display.clearDisplay();
    display.drawBitmap(0, 0, logo_bitmap, logo_width, logo_height, BLACK);
    display.display();
    delay(1500);

    display.clearDisplay();
    display.display();

    return true;
}

void Display::clear() {
    display.setCursor(0, 0);
    display.clearDisplay();
}

void Display::set_backlight(uint8_t v) {
    analogWrite(bck, 255 - (uint8_t) (2.55f * (this->backlight = v)));
#ifdef __EEPROM__
    eeprom_set(ADDRESS_BRIGHTNESS, (uint8_t) this->backlight);
#endif
}

void Display::increase_backlight() {
    if (backlight < 100) {
        set_backlight(backlight + 1);
    }
}

void Display::decrease_backlight() {
    if (backlight > 0) {
        set_backlight(backlight - 1);
    }
}

uint8_t *Display::get_backlight() {
    return &backlight;
}

LOCALE *Display::get_locale() const {
    return &locale;
}

void Display::set_locale(LOCALE locale) {
    this->locale = locale;
#ifdef __EEPROM__
    eeprom_set(ADDRESS_LOCALE, (uint8_t) this->locale);
#endif
}

void Display::position(int16_t x, int16_t y) {
    display.setCursor(x, y);
}

void Display::set_size(uint8_t s) {
    display.setTextSize(s);
}

void Display::print(const char c) {
    display.print(c);
}

void Display::print(const __FlashStringHelper *ifsh) {
    display.print(ifsh);
}

void Display::println() {
    display.println();
}

void Display::println(const __FlashStringHelper *ifsh) {
    display.println(ifsh);
}

void Display::print(int i, int b) {
    display.print(i, b);
}

void Display::print(long i, int b) {
    display.print(i, b);
}

void Display::print(unsigned int i, int b) {
    display.print(i, b);
}

void Display::print(unsigned long i, int b) {
    display.print(i, b);
}

void Display::print(double i, int d) {
    display.print(i, d);
}

void Display::cycle() {
    display.display();
}
