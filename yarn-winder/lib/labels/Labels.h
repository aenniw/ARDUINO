#pragma once

#include <Arduino.h>
#include <Motor.h>
#include <Label.h>

#define OPTION_SPACER   ' '
#define OPTION_PREFIX   "> "
#define OPTION_SUFFIX   " <"

class StaticLabel : public Label {
private:
    const char *const *labels = nullptr;
    const char *label = nullptr;
    const uint8_t labels_count;
protected:
    void print(Display &display, bool nl, bool center = false,
               char sp = ' ', const char *p = "", const char *s = "") const;
public:
    explicit StaticLabel(const char *label);
    StaticLabel(const char *const labels[], uint8_t labels_count);

    void print(Display &display, bool nl) const override;
    void print(Display &display, char sp, const char *p, const char *s) const override;;
};

template<class T>
class ValueLabel : public Label {
private:
    T *value = nullptr;
protected:
    uint8_t val_len() const {
        auto val = *value;
        uint8_t len = 0;
        do {
            val /= 10;
            len++;
        } while (val > 0);
        return len;
    }

public:
    explicit ValueLabel(T *value) : value(value) {}

    void print(Display &display, bool nl) const override {
        const uint8_t len = val_len() + 4;
        const int offset = display.width() - len,
                l_offset = offset / 2,
                r_offset = offset % 2 ? (offset / 2) + 1 : offset / 2;

        for (uint8_t i = 0; i < l_offset; i++)
            display.print(OPTION_SPACER);

        display.print(OPTION_PREFIX);
        display.print(*value);
        display.print(OPTION_SUFFIX);

        for (uint8_t i = 0; i < r_offset; i++)
            display.print(OPTION_SPACER);

        if (nl)
            display.println();
    }
};

class StatusLabel : public Label {
private:
    Motor &motor;
    const Label &start, &pause;
public:
    StatusLabel(Motor &motor, const Label &start, const Label &pause);

    void print(Display &display, bool nl) const override;
};


template<class T>
class SelectionLabel : public Label {
private:
    T *selector = nullptr;
    const Label **labels = nullptr;
public:
    SelectionLabel(const Label **labels, T *selector) : labels(labels), selector(selector) {}

    void print(Display &display, bool nl) const override {
        labels[(uint8_t) (*selector)]->print(display, OPTION_SPACER, OPTION_PREFIX, OPTION_SUFFIX);
    }
};

class MonitorLabel : public Label {
private:
    Motor &motor;
    const Label &speed, &evol, &len, &bat;
public:
    MonitorLabel(Motor &motor, const Label &speed, const Label &evol, const Label &len, const Label &bat);

    void print(Display &display, bool nl) const override;
};