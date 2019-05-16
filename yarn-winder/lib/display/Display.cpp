#include "Display.h"

Display::Display() {
    display = new Adafruit_SSD1306(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
    display->begin(SSD1306_SWITCHCAPVCC, 0x3C);
    display->clearDisplay();
    display->display();
    display->setTextColor(WHITE);
    display->setTextSize(1);
}

void Display::clear() {
    display->setCursor(0, 0);
    display->clearDisplay();
}

void Display::position(int16_t x, int16_t y) {
    display->setCursor(x, y);
}

void Display::set_size(uint8_t s) {
    display->setTextSize(s);
}

void Display::print(const char *c) {
    display->print(c);
}

void Display::println(const char *c) {
    display->println(c);
}

void Display::print(int i, int b) {
    display->print(i, b);
}

void Display::print(long i, int b) {
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
