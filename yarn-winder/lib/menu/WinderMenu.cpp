#include "WinderMenu.h"
#include <LocaleLabels.h>

static Configuration *config = nullptr;
static Menu *active_menu = nullptr;
static Motor *motor = nullptr;
static Display *display = nullptr;

static void restore_menu(Menu *m) {
    active_menu = m;
}

static Menu *change_menu(Menu *m) {
    Menu *last_root = active_menu;
    active_menu = m;
    return last_root;
}

static void back_menu() {
    if (active_menu) {
        active_menu->back();
    }
}

static void reset_motor() {
    if (motor) {
        motor->reset();
        back_menu();
    }
}

static void toggle_motor() {
    if (motor) {
        motor->toggle();
        back_menu();
    }
}

WinderMenu::WinderMenu(Configuration *c, Display *d, Motor *m) {
    this->_display = d;

    display = d;
    motor = m;
    config = c;

    _status = new MonitorLabel(motor, &speed_label, &evol_label, &len_label);

    auto back_entry = new MenuItem(&back_label);
    auto settings_menu = new Menu(&settings_label, new Item *[4]{
            new Menu(&profile_label, new Item *[3]{
                    new MenuItem(&auto_wind_label, []() { if (config) config->set_profile(Auto); }),
                    new MenuItem(&manual_wind_label, []() { if (config) config->set_profile(Manual); }),
                    back_entry
            }, 3, change_menu, restore_menu),
            new Menu(&language_label, new Item *[3]{
                    new MenuItem(&english_label, []() { if (config) config->set_locale(EN); }),
                    new MenuItem(&czech_label, []() { if (config) config->set_locale(CS); }),
                    back_entry
            }, 3, change_menu, restore_menu),
            new MenuValue(&brig_label, new ValueLabel<uint8_t>(display->get_backlight()), change_menu, restore_menu,
                          []() { if (display) display->decrease_backlight(); },
                          []() { if (display) display->increase_backlight(); }),
            back_entry
    }, 4, change_menu, restore_menu);

    active_menu = new Menu(&menu_label, new Item *[5]{
                                   new MenuItem(new StatusLabel(motor, &start_label, &pause_label), toggle_motor),
                                   new MenuItem(&stop_label, reset_motor),
                                   new MenuValue(&evol_max_label, new ValueLabel<unsigned long>(motor->get_stop_evolution()),
                                                 change_menu, restore_menu, []() { if (motor) motor->decrease_stop_evolution(); },
                                                                            []() { if (motor) motor->increase_stop_evolution(); }),
                                   settings_menu,
                                   back_entry
                           }, 5, change_menu, restore_menu, []() { if (motor) motor->decrease_speed(); },
                                                            []() { if (motor) motor->increase_speed(); });
}

void WinderMenu::cycle() {
    _display->clear();
    print(config->get_locale(), _display, true);
}

void WinderMenu::interact() {
    if (active_menu) {
        active_menu->interact();
    }
}

void WinderMenu::next() {
    if (active_menu) {
        active_menu->next();
    }
}

void WinderMenu::prev() {
    if (active_menu) {
        active_menu->prev();
    }
}

void WinderMenu::print(LOCALE locale, Display *display, bool nl) const {
    if (active_menu && active_menu->is_active()) {
        active_menu->print(locale, display, nl);
    } else if (_status) {
        _status->print(locale, display, nl);
    }
}
