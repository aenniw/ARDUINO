#ifndef ARDUINO_PROJECTS_MENU_H
#define ARDUINO_PROJECTS_MENU_H

#include <Arduino.h>
#include <Display.h>
#include <Labels.h>

class Item : public Label {
public:
    virtual void interact() = 0;
};

class Navigation : public Item {
public:
    virtual void next() =0;

    virtual void prev() =0;

    virtual void back() {};
};

class MenuItem : public Item {
private:
    const Label *label = nullptr;

    void (*callback)();

public:
    explicit MenuItem(const Label *label, void (*callback)() = nullptr);

    void print(LOCALE locale, Display *display, bool nl) const override;

    void interact() override;
};

class Menu : public Navigation {
private:
    const Label *_label;

    Item **items = nullptr;
    uint8_t item_count = 0;
    Menu *last_menu = nullptr;

    Menu *(*on_active)(Menu *) = nullptr;

    void (*on_inactive)(Menu *) = nullptr;

    void (*on_next)() = nullptr;

    void (*on_prev)() = nullptr;

    volatile boolean active = false;
    volatile uint8_t active_item = 0;
public:
    Menu(const Label *label, Item **items, uint8_t item_count, Menu *(*on_active)(Menu *) = nullptr,
         void(*on_inactive)(Menu *) = nullptr, void (*next)() = nullptr, void (*prev)() = nullptr);

    void next() override;

    void prev() override;

    void interact() override;

    void back() override;

    void print(LOCALE locale, Display *display, bool nl) const override;
};


#endif //ARDUINO_PROJECTS_MENU_H
