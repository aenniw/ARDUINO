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

static NIButton button_ok(ROTARY_OK);
static MosfetMotor motor(MOTOR_PWM, GATE_PIN);
static RotaryEncoder encoder(ROTARY_CLK, ROTARY_DT);
static Display display(DISPLAY_BCK, DISPLAY_SCLK, DISPLAY_DIN, DISPLAY_DC, DISPLAY_CS, DISPLAY_RST);
static WinderMenu winder_menu(&display, &motor);

static std::vector<Service *> services;

void setup() {
    services.push_back((Service *) &motor);
    services.push_back((Service *) &display);
    services.push_back((Service *) &button_ok);
    services.push_back((Service *) &winder_menu);

    PCICR |= (1 << PCIE1);
    PCMSK1 |= (1 << PCINT10) | (1 << PCINT11);
    button_ok.on_short_press([]() { winder_menu.interact(); });

    for (const auto &service: services)
        service->begin();
}

ISR(PCINT1_vect) {
    encoder.tick();
}

void loop() {
    for (const auto &service: services) {
        service->cycle();
    }

    static long last_position = 0;
    long current_position = encoder.getPosition();
    if (current_position > last_position) {
        winder_menu.next();
    } else if (current_position < last_position) {
        winder_menu.prev();
    }
    last_position = current_position;
}
