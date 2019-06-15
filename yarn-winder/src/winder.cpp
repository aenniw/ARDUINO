#include <Arduino.h>
#include <vector>
#include <NIButtons.h>
#include <MosfetMotor.h>
#include <WinderMenu.h>

#define GATE_PIN        2
#define BUTTON_PLUS     3
#define BUTTON_OK       4
#define BUTTON_MINUS    5
#define MOTOR_PWM       6

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

    buttons->add_button(BUTTON_PLUS)->on_short_press([]() { menu->prev(); });
    buttons->add_button(BUTTON_MINUS)->on_short_press([]() { menu->next(); });
    buttons->add_button(BUTTON_OK)->on_short_press([]() { menu->interact(); });
}


void loop() {
    for (const auto &service: services) {
        service->cycle();
    }
}
