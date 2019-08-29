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
           void(*on_inactive)(Menu *) = nullptr, void (*on_next)() = nullptr, void (*on_prev)() = nullptr,
           bool (*_enabled)() = nullptr) {
    this->_enabled = _enabled;
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
    } else if (active) {
        for (uint8_t i = active_item + 1; i < item_count; i++) {
            if (items[i]->enabled()) {
                active_item = i;
                break;
            }
        }
    }
};

void Menu::prev() {
    if (!active && on_prev) {
        on_prev();
    } else if (active) {
        for (int16_t i = active_item - 1; i >= 0; i--) {
            if (items[i]->enabled()) {
                active_item = (uint8_t) i;
                break;
            }
        }
    }
};

void Menu::interact() {
    if (!active) {
        active = true;
        if (on_active) {
            last_menu = on_active(this);
        }
    } else if (active_item == item_count - 1) {
        back();
    } else if (items) {
        items[active_item]->interact();
    }
}

void Menu::back() {
    if (active) {
        active = false;
        active_item = 0;
        if (on_inactive) {
            on_inactive(last_menu);
            last_menu = nullptr;
        }
    }
}

boolean Menu::is_active() const {
    return active;
}

void Menu::print_item(uint8_t i, LOCALE locale, Display *display, bool nl) const {
    if (i >= 0 && i < item_count) {
        if (item_count > 1) {
            if (i == active_item)
                display->print(F("> "));
            else if (!items[i]->enabled())
                display->print(F("- "));
        }
        items[i]->print(locale, display, nl);
    }
}

void Menu::print(LOCALE locale, Display *display, bool nl) const {
    if (active && items) {
        _label->print(locale, display, true);
        display->println(F("--------------"));

        const uint8_t last_entry = active_item > MAX_MENU_ITEMS - 1 ? active_item : MAX_MENU_ITEMS - 1;
        const uint8_t first_entry = active_item >= MAX_MENU_ITEMS ? 1 + active_item - MAX_MENU_ITEMS : 0;
        for (uint8_t i = first_entry; i <= last_entry; i++) {
            print_item(i, locale, display, nl);
        }

    } else {
        _label->print(locale, display, nl);
    }
}

void MenuValue::print_item(uint8_t i, LOCALE locale, Display *display, bool nl) const {
    if (i == 0) {
        items[0]->print(locale, display, nl);
    }
}

void MenuValue::next() {
    if (on_next) {
        on_next();
    }
}

void MenuValue::prev() {
    if (on_prev) {
        on_prev();
    }
}

boolean Item::enabled() {
    if (_enabled) {
        return _enabled();
    }
    return true;
}
