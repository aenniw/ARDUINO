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

static void print_progmem(Display &display, const char *l, bool center, const char sp, const char *p, const char *s) {
    const uint8_t len = strlen_P(l), padding_len = strlen(p) + strlen(s);
    const int offset = center ? display.width() - len - padding_len : 0,
            l_offset = offset % 2 ? (offset / 2) + 1 : offset / 2,
            r_offset = offset / 2;

    for (uint8_t i = 0; i < l_offset; i++)
        display.print(sp);

    if (p) display.print(p);
    for (uint8_t i = 0; i < len; i++)
        display.print((char) pgm_read_byte_near(l + i));
    if (s) display.print(s);

    for (uint8_t i = 0; i < r_offset; i++)
        display.print(sp);
}

StaticLabel::StaticLabel(const char *label) :
        label(label), labels_count(0) {}

StaticLabel::StaticLabel(const char *const labels[], uint8_t labels_count) :
        labels(labels), labels_count(labels_count) {}

void StaticLabel::print(Display &display, bool nl, bool center, const char sp, const char *p, const char *s) const {
    if (label) {
        print_progmem(display, label, center, sp, p, s);
    } else if (labels && labels_count) {
        print_progmem(display, (char *) pgm_read_word(&(labels[*display.get_locale()])), center, sp, p, s);
    }
    if (nl)
        display.println();
}

void StaticLabel::print(Display &display, bool nl) const {
    print(display, nl, false);
}

void StaticLabel::print(Display &display, char sp, const char *p, const char *s) const {
    print(display, true, true, sp, p, s);
}

StatusLabel::StatusLabel(Motor &motor, const Label &start, const Label &pause) :
        motor(motor), start(start), pause(pause) {}

void StatusLabel::print(Display &display, bool nl) const {
    if (motor.get_state() == ON) {
        pause.print(display, nl);
    } else {
        start.print(display, nl);
    }
}

MonitorLabel::MonitorLabel(Motor &motor, const Label &speed, const Label &evol, const Label &len, const Label &bat) :
        motor(motor), speed(speed), evol(evol), len(len), bat(bat) {}

void MonitorLabel::print(Display &display, bool nl) const {
    display.position(0, 8);
    speed.print(display, false);
    display.print(F(": "));
    display.print(motor.get_speed());
    display.println(F("%"));

    evol.print(display, false);
    display.print(F(": "));
    display.print(motor.get_evolution());
    auto stop_evol = *motor.get_stop_evolution();
    if (stop_evol) {
        display.print(F("/"));
        display.print(stop_evol);
    }
    display.println();

    len.print(display, false);
    display.print(F(": "));
    display.print(motor.get_len(), 3);
    display.println(F("m"));

    bat.print(display, false);
    display.print(F(": "));
    display.print(read_vcc(3000, 4200));
    display.print(F("%"));
}