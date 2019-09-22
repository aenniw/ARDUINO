#ifndef ARDUINO_PROJECTS_LABELS_H
#define ARDUINO_PROJECTS_LABELS_H

#include <Arduino.h>
#include <Motor.h>
#include <Label.h>

class StaticLabel : public Label {
private:
    const char *const *labels = nullptr;
    const char *label = nullptr;
    uint8_t labels_count = 0;
    LOCALE fallback = EN;
protected:
    void print_progmem(Display *display, const char *label) const;
public:
    explicit StaticLabel(const char *label);

    StaticLabel(const char *const labels[], uint8_t labels_count, LOCALE fallback = EN);

    void print(Display *display, bool nl) const override;
};

template<class T>
class ValueLabel : public Label {
private:
    T *value = nullptr;
public:
    explicit ValueLabel(T *value) {
        this->value = value;
    }

    void print(Display *display, bool nl) const override {
        display->println();
        display->print(F("     "));
        display->print(*value);
        display->println();
    }
};

class StatusLabel : public Label {
private:
    Motor *motor = nullptr;
    const Label *start = nullptr, *pause = nullptr;
public:
    StatusLabel(Motor *motor, const Label *start, const Label *pause);

    void print(Display *display, bool nl) const override;
};


template<class T>
class SelectionLabel : public Label {
private:
    T *selector = nullptr;
    const Label **labels = nullptr;
public:
    SelectionLabel(const Label **labels, T *profile) {
        this->labels = labels;
        this->selector = profile;
    }

    void print(Display *display, bool nl) const override {
        display->println();
        display->print(F("  "));
        labels[(uint8_t) (*selector)]->print(display, true);
    }
};

class MonitorLabel : public Label {
private:
    Motor *motor = nullptr;
    const Label *speed = nullptr, *evol = nullptr, *len = nullptr, *bat = nullptr;
public:
    MonitorLabel(Motor *motor, const Label *speed, const Label *evol, const Label *len, const Label *bat);

    void print(Display *display, bool nl) const override;
};


#endif //ARDUINO_PROJECTS_LABELS_H
