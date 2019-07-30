#include "Labels.h"

StaticLabel::StaticLabel(const char *label) {
    this->label = label;
}

StaticLabel::StaticLabel(const char *const labels[], uint8_t labels_count, LOCALE fallback) {
    this->labels = labels;
    this->labels_count = labels_count;
    this->fallback = fallback;
}

void StaticLabel::print(LOCALE locale, Display *display, bool nl) const {
    if (this->label) {
        display->print(this->label);
    } else if (labels && labels_count) {
        if (locale < labels_count) {
            display->print(labels[locale]);
        } else {
            display->print(labels[fallback]);
        }
    }
    if (nl) {
        display->println("");
    }
}

StatusLabel::StatusLabel(Motor *motor, const Label *start, const Label *pause) {
    this->motor = motor;
    this->start = start;
    this->pause = pause;
}

void StatusLabel::print(LOCALE locale, Display *display, bool nl) const {
    if (!motor || !start || !pause) {
        return;
    }
    if (motor->get_state() == ON) {
        pause->print(locale, display, nl);
    } else {
        start->print(locale, display, nl);
    }
}

MonitorLabel::MonitorLabel(Motor *motor, const Label *speed, const Label *evol, const Label *len) {
    this->motor = motor;
    this->speed = speed;
    this->evol = evol;
    this->len = len;
}

void MonitorLabel::print(LOCALE locale, Display *display, bool nl) const {
    if (!motor || !speed || !evol || !len) {
        return;
    }

    display->position(0, 10);
    speed->print(locale, display, false);
    display->print(": ");
    display->print((int) motor->get_speed());
    display->println("%");

    evol->print(locale, display, false);
    display->print(": ");
    display->print((long) motor->get_evolution());
    unsigned long stop_evol = *motor->get_stop_evolution();
    if (stop_evol) {
        display->print("/");
        display->print((long) stop_evol);
    }
    display->println();

    len->print(locale, display, false);
    display->print(": ");
    display->print(motor->get_len(), 3);
    display->print("m");
}
