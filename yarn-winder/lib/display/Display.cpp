#include "Display.h"
#include "Logo.h"

Display::Display(uint8_t bck, uint8_t cs, uint8_t dc) :
        bck(bck), display(U8G2_R2, cs, dc) {
#ifdef __EEPROM__
    uint8_t data;
    EEPROM.get(ADDRESS_BRIGHTNESS, data);
    this->backlight = data > BACKLITE_MAX ? BACKLITE_MAX : data;
    EEPROM.get(ADDRESS_LOCALE, data);
    this->locale = data >= INVALID_LOCALE ? this->locale : (LOCALE) data;
#endif
}

bool Display::begin() {
    pinMode(bck, OUTPUT);
    set_backlight(backlight);
    display.begin();
    display.setContrast(150);
    display.setFont(u8g_font_7x13B);
    clear();
    return true;
}

void Display::clear() {
    line = display.getMaxCharHeight() - 4;
    display.setCursor(0, line);
}

void Display::set_backlight(uint8_t v) {
    analogWrite(bck, (uint8_t) (2.55 * (this->backlight = v)));
#ifdef __EEPROM__
    eeprom_set(ADDRESS_BRIGHTNESS, (uint8_t) this->backlight);
#endif
}

void Display::increase_backlight() {
    if (backlight < BACKLITE_MAX) {
        set_backlight(backlight + 1);
    }
}

void Display::decrease_backlight() {
    if (backlight > 0) {
        set_backlight(backlight - 1);
    }
}

uint8_t *Display::get_backlight() const {
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
    line += y;
    display.setCursor(x, line);
}

void Display::firstPage() {
    display.firstPage();
}

uint8_t Display::nextPage() {
    return display.nextPage();
}

void Display::cycle() {}

uint8_t Display::width() {
    return (display.getDisplayWidth() / display.getMaxCharWidth()) - 3;
}

uint8_t Display::height() {
    return (display.getDisplayHeight() / display.getMaxCharHeight()) - 3;
}

size_t Display::write(uint8_t v) {
    if (v == '\n') {
        line += display.getMaxCharHeight();
        display.setCursor(0, line);
    }
    return display.write(v);
}
