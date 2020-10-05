#pragma once

#include <Arduino.h>
#include <Service.h>
#include <TM1637.h>

class Display : public TimedService {
private:
    TM1637 display;
    uint8_t brightness = BRIGHT_HIGH;
    int8_t disp_buffer[4] = {0x00, 0x00, 0x00, 0x00};
    bool dirty = false, blink = false, clear = false, disabled = false;

    const uint16_t timeout;
protected:
    void set_brightness(uint8_t b);

public:
    Display(uint8_t _pin1, uint8_t _pin2, uint16_t _timeout);

    bool begin() override;

    void set_blink(bool state);

    void light_up();

    void print(unsigned int position);

    void print(const char *text);

    void cycle() override;

    void disable(uint8_t cause) override;
};