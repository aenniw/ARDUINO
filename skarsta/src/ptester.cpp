#include <vector>
#include <Arduino.h>
#include <Display.h>
#include <NIButtons.h>
#include <configuration.h>

Display display(DISPLAY_PIN_CLK, DISPLAY_PIN_DIO, FADE_TIMEOUT);
NIButton down(BUTTON_DOWN), up(BUTTON_UP), rst(BUTTON_RST),
        preset_0(BUTTON_P0), preset_1(BUTTON_P1), preset_2(BUTTON_P2);

static std::vector<Service *> services = {&display};
static std::vector<NIButton *> buttons = {
        &down, &up, &preset_0, &preset_1, &preset_2, &rst
};

void setup() {
    for (const auto &button: buttons)
        button->begin();
    for (const auto &service: services)
        service->begin();
}

void loop() {
    for (const auto &service: services)
        service->cycle();

    uint8_t i = 1;
    for (const auto &button: buttons) {
        button->cycle();
        if (button->pressed()) {
            display.print(i);
        }
        i++;
    }
}