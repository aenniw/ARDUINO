#ifndef ARDUINO_PROJECTS_MENU_H
#define ARDUINO_PROJECTS_MENU_H

#include <Arduino.h>
#include <Display.h>
#include <Labels.h>

#define MAX_MENU_ITEMS 4

class Item : public Label {
protected:
    bool (*_enabled)() = nullptr;
public:
    virtual void interact() = 0;

    virtual boolean enabled();;
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

    void print(Display *display, bool nl) const override;

    void interact() override;
};

class Menu : public Navigation {
protected:
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

    virtual void print_item(uint8_t i, Display *display, bool nl) const;

public:
    Menu(const Label *label, Item **items, uint8_t item_count, Menu *(*on_active)(Menu *) = nullptr,
         void(*on_inactive)(Menu *) = nullptr, void (*next)() = nullptr, void (*prev)() = nullptr,
         bool (*_enabled)() = nullptr);

    boolean is_active() const;

    void next() override;

    void prev() override;

    void interact() override;

    void back() override;

    void print(Display *display, bool nl) const override;
};

class MenuValue : public Menu {
public:
    MenuValue(const Label *label, Label *value, Menu *(*on_active)(Menu *) = nullptr,
              void(*on_inactive)(Menu *) = nullptr, void (*next)() = nullptr, void (*prev)() = nullptr,
              bool (*_enabled)() = nullptr) :
            Menu(label, new Item *[1]{new MenuItem(value)}, 1, on_active, on_inactive, next, prev, _enabled) {}

    void print_item(uint8_t i, Display *display, bool nl) const override;

    void next() override;

    void prev() override;
};

#endif //ARDUINO_PROJECTS_MENU_H
