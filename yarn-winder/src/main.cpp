#include <configuration.h>

#include <Arduino.h>
#include <Wire.h>
#include <vector>
#include <NIButtons.h>
#include <MosfetMotor.h>
#include <WinderMenu.h>
#include <Rotary.h>

static NIButton button_ok(ROTARY_OK);
static MosfetMotor motor(MOTOR_PWM, GATE_PIN);
static Rotary encoder(ROTARY_CLK, ROTARY_DT);
static Display display(DISPLAY_BCK, DISPLAY_CS, DISPLAY_DC);
static WinderMenu winder_menu(display, motor);

static std::vector<Service *> services{&motor, &display, &button_ok, &winder_menu};

void setup() {
    PCICR |= (1 << PCIE1);
    PCMSK1 |= (1 << PCINT9) | (1 << PCINT10);
    button_ok.on_short_press([]() { winder_menu.interact(); });

    for (const auto &service: services)
        service->begin();
}

ISR(PCINT1_vect) {
    const unsigned char result = encoder.process();
    if (result == DIR_CW) {
        winder_menu.next();
    } else if (result == DIR_CCW) {
        winder_menu.prev();
    }
}

void loop() {
    for (const auto &service: services) {
        service->cycle();
    }
}
