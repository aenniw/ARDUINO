#include <Arduino.h>
#include <vector>
#include <NIButtons.h>
#include <MosfetMotor.h>
#include <WinderMenu.h>

#define MOTOR_PWM 6
#define GATE_PIN 2

#define BUTTON_PLUS 8
#define BUTTON_OK 9
#define BUTTON_MINUS 10

static std::vector<Service *> services;
static WinderMenu *menu = nullptr;

void setup() {
    auto display = new Display();
    auto buttons = NIButtons::get_instance();
    auto config = Configuration::get_instance();
    auto motor = new MosfetMotor(MOTOR_PWM, GATE_PIN);
    menu = new WinderMenu(config, display, motor);

    services.push_back((Service *) motor);
    services.push_back((Service *) display);
    services.push_back((Service *) buttons);
    services.push_back((Service *) menu);

    buttons->add_button(BUTTON_PLUS)->on_pressed([]() { menu->prev(); });
    buttons->add_button(BUTTON_MINUS)->on_pressed([]() { menu->next(); });
    buttons->add_button(BUTTON_OK)->on_pressed([]() { menu->interact(); });
}


void loop() {
    for (const auto &service: services) {
        service->cycle();
    }
}
