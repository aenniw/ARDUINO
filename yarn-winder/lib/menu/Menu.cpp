#include "Menu.h"

MenuItem::MenuItem(const Label &label, void (*callback)()) :
        label(label), callback(callback) {}

void MenuItem::interact() {
    if (callback)
        this->callback();
}

void MenuItem::print(Display &display, bool nl) const {
    label.print(display, nl);
}

Menu::Menu(const Label &label, Item **items, uint8_t item_count, Menu *(*on_active)(Menu *),
           void(*on_inactive)(Menu *), void (*on_next)(), void (*on_prev)(), bool (*_enabled)()) :
        _label(label), items(items), item_count(item_count), on_active(on_active), on_inactive(on_inactive),
        on_next(on_next), on_prev(on_prev) {
    this->_enabled = _enabled;
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

void Menu::print_item(uint8_t i, Display &display, bool nl) const {
    if (i >= 0 && i < item_count) {
        if (item_count > 1) {
            if (i == active_item)
                display.print(F("> "));
            else if (!items[i]->enabled())
                display.print(F("- "));
            else
                display.print(F("  "));
        }
        items[i]->print(display, nl);
    }
}

void Menu::print(Display &display, bool nl) const {
    if (active && items) {
        _label.print(display, '-', nullptr, nullptr);

        const uint8_t last_entry = active_item > MAX_MENU_ITEMS - 1 ? active_item : MAX_MENU_ITEMS - 1;
        const uint8_t first_entry = active_item >= MAX_MENU_ITEMS ? 1 + active_item - MAX_MENU_ITEMS : 0;
        for (uint8_t i = first_entry; i <= last_entry; i++) {
            print_item(i, display, nl);
        }

    } else {
        _label.print(display, nl);
    }
}

void MenuValue::print_item(uint8_t i, Display &display, bool nl) const {
    if (i == 0) {
        display.println();
        items[0]->print(display, ' ', nullptr, nullptr);
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

boolean Item::enabled() const {
    if (_enabled) {
        return _enabled();
    }
    return true;
}
