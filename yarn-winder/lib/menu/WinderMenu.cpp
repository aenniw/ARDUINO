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
    auto profile_labels = new Label *[3]{&manual_wind_label, &semi_wind_label, &auto_wind_label};
    auto locale_labels = new Label *[2]{&english_label, &czech_label};

    auto settings_menu = new Menu(&settings_label, new Item *[4]{
            new MenuValue(&profile_label, new SelectionLabel<PROFILE>(profile_labels, config->get_profile()),
                          change_menu, restore_menu,
                          []() { config->prev_profile(); },
                          []() { config->next_profile(); }),
            new MenuValue(&language_label, new SelectionLabel<LOCALE>(locale_labels, config->get_locale()),
                          change_menu, restore_menu,
                          []() { config->set_locale(EN); },
                          []() { config->set_locale(CS); }),
            new MenuValue(&brig_label, new ValueLabel<uint8_t>(display->get_backlight()),
                          change_menu, restore_menu,
                          []() { display->decrease_backlight(); },
                          []() { display->increase_backlight(); }),
            back_entry
    }, 4, change_menu, restore_menu);

    active_menu = new Menu(&menu_label, new Item *[5]{
                                   new MenuItem(new StatusLabel(motor, &start_label, &pause_label), toggle_motor),
                                   new MenuItem(&stop_label, reset_motor),
                                   new MenuValue(&evol_max_label, new ValueLabel<unsigned long>(motor->get_stop_evolution()),
                                                 change_menu, restore_menu, []() { motor->decrease_stop_evolution(); },
                                                 []() { motor->increase_stop_evolution(); },
                                                 []() { return *config->get_profile() != Manual; }),
                                   settings_menu,
                                   back_entry
                           }, 5, change_menu, restore_menu, []() { motor->decrease_speed(); },
                           []() { motor->increase_speed(); });
}

void WinderMenu::cycle() {
    _display->clear();
    print(*config->get_locale(), _display, true);
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
