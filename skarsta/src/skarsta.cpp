#include <Arduino.h>
#include <Display.h>
#include <Keypad.h>

#define ENCODER_PIN_CLK 2
#define ENCODER_PIN_DIO 3
#define DISPLAY_PIN_CLK 4
#define DISPLAY_PIN_DIO 5
#define DIRECTION_RELAY 6
#define POWER_RELAY 7

static Service **services;
static const uint8_t services_count = 3;
void setup()
{
    auto motor = new Motor(ENCODER_PIN_CLK, ENCODER_PIN_DIO, POWER_RELAY, DIRECTION_RELAY);

    auto display = new Display(DISPLAY_PIN_CLK, DISPLAY_PIN_DIO);
    if (motor->get_mode() == UNCALIBRATED)
    {
        display->set_blink(true);
        display->display_print(DISPLAY_NONE);
    }

    services = new Service *[services_count] { (Service *)new Keypad(motor, display),
                                                   (Service *)motor,
                                                   (Service *)display };
}

void loop()
{
    for (uint8_t i = 0; i < services_count; i++)
    {
        services[i]->cycle();
    }
}