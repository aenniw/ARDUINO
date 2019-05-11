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

    display->set_size(1);
    display->position(0, 4);
    speed->print(locale, display, false);
    display->print(": ");
    display->print(motor->get_speed());
    evol->print(locale, display, false);
    display->print(": ");
    display->print(static_cast<int>(motor->get_evolution() / 24));
    len->print(locale, display, false);
    display->print(": ");
    display->print(static_cast<int>(10 * motor->get_evolution() / 24));
}