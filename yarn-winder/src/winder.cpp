#include <Arduino.h>
#include <vector>
#include <NIButtons.h>
#include <MosfetMotor.h>
#include <WinderMenu.h>
#include <RotaryEncoder.h>

#define GATE_PIN        2
#define DISPLAY_BCK     3
#define DISPLAY_SCLK    4
#define DISPLAY_DIN     5
#define DISPLAY_DC      6
#define DISPLAY_CS      7
#define DISPLAY_RST     8
#define MOTOR_PWM       9
#define ROTARY_CLK      A2
#define ROTARY_DT       A3
#define ROTARY_OK       A4

static std::vector<Service *> services;
static RotaryEncoder *encoder = nullptr;
static WinderMenu *menu = nullptr;

void setup() {
    auto display = new Display(DISPLAY_BCK, DISPLAY_SCLK, DISPLAY_DIN, DISPLAY_DC, DISPLAY_CS, DISPLAY_RST);
    auto buttons = NIButtons::get_instance();
    auto config = Configuration::get_instance();
    auto motor = new MosfetMotor(MOTOR_PWM, GATE_PIN);

    menu = new WinderMenu(config, display, motor);
    encoder = new RotaryEncoder(ROTARY_CLK, ROTARY_DT);

    services.push_back((Service *) motor);
    services.push_back((Service *) display);
    services.push_back((Service *) buttons);
    services.push_back((Service *) menu);

    PCICR |= (1 << PCIE1);
    PCMSK1 |= (1 << PCINT10) | (1 << PCINT11);
    buttons->add_button(ROTARY_OK)->on_short_press([]() { menu->interact(); });
}

ISR(PCINT1_vect) {
    encoder->tick();
}

void loop() {
    for (const auto &service: services) {
        service->cycle();
    }

    static long last_position = 0;
    long current_position = encoder->getPosition();
    if (current_position > last_position) {
        menu->next();
    } else if (current_position < last_position) {
        menu->prev();
    }
    last_position = current_position;
}
