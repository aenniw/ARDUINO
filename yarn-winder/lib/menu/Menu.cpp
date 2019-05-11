#include "Menu.h"


MenuItem::MenuItem(const Label *label, void (*callback)()) {
    this->label = label;
    this->callback = callback;
}

void MenuItem::interact() {
    if (callback)
        this->callback();
}

void MenuItem::print(LOCALE locale, Display *display, bool nl) const {
    if (label)
        label->print(locale, display, nl);
}

Menu::Menu(const Label *label, Item **items, uint8_t item_count, Menu *(*on_active)(Menu *) = nullptr,
           void(*on_inactive)(Menu *) = nullptr, void (*on_next)() = nullptr, void (*on_prev)() = nullptr) {
    this->_label = label;
    this->items = items;
    this->item_count = item_count;
    this->on_active = on_active;
    this->on_inactive = on_inactive;
    this->on_next = on_next;
    this->on_prev = on_prev;
}

void Menu::next() {
    if (!active && on_next) {
        on_next();
    } else if (active && active_item + 1 < item_count) {
        active_item++;
    }
};

void Menu::prev() {
    if (!active && on_prev) {
        on_prev();
    } else if (active && active_item > 0) {
        active_item--;
    }
};

void Menu::interact() {
    if (!active) {
        active = true;
        if (on_active) {
            last_menu = on_active(this);
        }
    } else if (active_item == item_count - 1) {
        active = false;
        active_item = 0;
        if (on_inactive) {
            on_inactive(last_menu);
            last_menu = nullptr;
        }
    } else if (items) {
        items[active_item]->interact();
    }
}

void Menu::print(LOCALE locale, Display *display, bool nl) const {
    if (active && items) {
        display->set_size(2);
        if (active_item == 0)
            display->position(0, 8);
        else
            display->position(0, -8);
        if (active_item - 1 >= 0)
            items[active_item - 1]->print(locale, display, nl);
        display->print("> ");
        items[active_item]->print(locale, display, nl);
        if (active_item + 1 < item_count)
            items[active_item + 1]->print(locale, display, nl);
    } else {
        _label->print(locale, display, nl);
    }
}