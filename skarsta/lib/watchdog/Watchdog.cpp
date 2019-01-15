#include "Watchdog.h"

static unsigned int position_diff(unsigned int a, unsigned int b) {
    return a > b ? a - b : b - a;
}

Watchdog::Watchdog(Motor *m, Display *d) {
    this->motor = m;
    this->display = d;
}

void Watchdog::cycle() {
    static unsigned long last_tick = millis();
    static unsigned int last_position = motor->get_position();
    unsigned long now = millis(), diff = get_period(last_tick, now);

    if (diff >= WATCHDOG_TIMEOUT) {

        if (position_diff(motor->get_position(), last_position) <= 4) {
            motor->off();
            display->print("-err");

#ifdef __DEBUG__
            Serial.print("w stop l:");
            Serial.print(last_position);
            Serial.print(" c:");
            Serial.print(motor->get_position());
            Serial.println();
#endif
        }

        last_tick = now;
        last_position = motor->get_position();
    }
}