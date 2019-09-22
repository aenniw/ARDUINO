#include "Labels.h"

static uint8_t read_vcc(uint16_t min, uint16_t max) {
    ADMUX = _BV(REFS0) | _BV(MUX3) | _BV(MUX2) | _BV(MUX1); // Read 1.1V reference against AVcc
    //delay(2); // Wait for Vref to settle
    ADCSRA |= _BV(ADSC); // Convert
    while (bit_is_set(ADCSRA, ADSC));
    long vcc = ADCL;
    vcc |= ADCH << 8;
    vcc = 1126400L / vcc; // Back-calculate AVcc in mV

    if (vcc >= max) {
        return 100;
    } else if (vcc <= min) {
        return 0;
    } else {
        return (uint8_t) ((vcc - min) / ((max - min) / 100.0));
    }
}

StaticLabel::StaticLabel(const char *label) {
    this->label = label;
}

StaticLabel::StaticLabel(const char *const labels[], uint8_t labels_count, LOCALE fallback) {
    this->labels = labels;
    this->labels_count = labels_count;
    this->fallback = fallback;
}

void StaticLabel::print(Display *display, bool nl) const {
    if (label) {
        print_progmem(display, label);
    } else if (labels && labels_count) {
        const LOCALE locale = *display->get_locale();
        if (locale < labels_count) {
            print_progmem(display, (char *) pgm_read_word(&(labels[locale])));
        } else {
            print_progmem(display, (char *) pgm_read_word(&(labels[fallback])));
        }
    }
    if (nl) {
        display->println();
    }
}

void StaticLabel::print_progmem(Display *display, const char *label) const {
    const uint8_t len = strlen_P(label);
    for (uint8_t i = 0; i < len; i++) {
        display->print((char) pgm_read_byte_near(label + i));
    }
}

StatusLabel::StatusLabel(Motor *motor, const Label *start, const Label *pause) {
    this->motor = motor;
    this->start = start;
    this->pause = pause;
}

void StatusLabel::print(Display *display, bool nl) const {
    if (!motor || !start || !pause) {
        return;
    }
    if (motor->get_state() == ON) {
        pause->print(display, nl);
    } else {
        start->print(display, nl);
    }
}

MonitorLabel::MonitorLabel(Motor *motor, const Label *speed, const Label *evol, const Label *len, const Label *bat) {
    this->motor = motor;
    this->speed = speed;
    this->evol = evol;
    this->len = len;
    this->bat = bat;
}

void MonitorLabel::print(Display *display, bool nl) const {
    if (!motor || !speed || !evol || !len) {
        return;
    }

    display->position(0, 10);
    speed->print(display, false);
    display->print(F(": "));
    display->print((int) motor->get_speed());
    display->println(F("%"));

    evol->print(display, false);
    display->print(F(": "));
    display->print((long) motor->get_evolution());
    unsigned long stop_evol = *motor->get_stop_evolution();
    if (stop_evol) {
        display->print(F("/"));
        display->print((long) stop_evol);
    }
    display->println();

    len->print(display, false);
    display->print(F(": "));
    display->print(motor->get_len(), 3);
    display->println(F("m"));

    bat->print(display, false);
    display->print(F(": "));
    display->print(read_vcc(3000, 4200));
    display->print(F("%"));
}