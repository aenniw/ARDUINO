#pragma once

#include <SPI.h>
#include <U8g2lib.h>
#include <Service.h>
#include <Configuration.h>

#define BACKLITE_MAX 100

class Display : public Service, public Print {
private:
    LOCALE locale = EN;
    uint8_t bck, backlight = BACKLITE_MAX, line = 0;
    U8G2_ST7565_NHD_C12864_1_4W_HW_SPI display;
private:
    size_t write(uint8_t uint8) override;
    void set_backlight(uint8_t v);
public:
    Display(uint8_t bck, uint8_t cs, uint8_t dc);

    uint8_t *get_backlight() const;
    void increase_backlight();
    void decrease_backlight();

    LOCALE *get_locale() const;
    void set_locale(LOCALE locale);
    void position(int16_t x, int16_t y);

    void clear();
    uint8_t width();
    uint8_t height();

    void firstPage();
    uint8_t nextPage();

    bool begin() override;
    void cycle() override;
};