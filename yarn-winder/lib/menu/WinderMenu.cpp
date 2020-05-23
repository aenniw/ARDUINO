#include "WinderMenu.h"

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

WinderMenu::WinderMenu(Display *d, Motor *m) :
        back_entry(&back_label),
        status(m, &speed_label, &evol_label, &len_label, &bat_label),
        evol_value_label(m->get_stop_evolution()),
        stall_value_label(m->get_stall_timeout()),
        brig_value_label(d->get_backlight()),
        profile_selection_label(profile_labels, m->get_profile()),
        locale_selection_label(locale_labels, d->get_locale()),
        status_label(m, &start_label, &pause_label),
        status_menu_item(&status_label, toggle_motor),
        stop_menu_item(&stop_label, reset_motor),
        profile_menu_value(&profile_label, &profile_selection_label, change_menu, restore_menu,
                           []() { motor->prev_profile(); },
                           []() { motor->next_profile(); }),
        stall_menu_value(&stall_label, &stall_value_label, change_menu, restore_menu,
                         []() { motor->decrease_stall_timeout(); },
                         []() { motor->increase_stall_timeout(); }),
        brig_menu_value(&brig_label, &brig_value_label, change_menu, restore_menu,
                        []() { display->decrease_backlight(); },
                        []() { display->increase_backlight(); }),
        locale_menu_value(&language_label, &locale_selection_label, change_menu, restore_menu,
                          []() { display->set_locale(EN); },
                          []() { display->set_locale(CS); }),
        evol_menu_value(&evol_max_label, &evol_value_label, change_menu, restore_menu,
                        []() { motor->decrease_stop_evolution(); },
                        []() { motor->increase_stop_evolution(); },
                        []() { return *motor->get_profile() != Manual; }),
        settings_menu_items(
                {&profile_menu_value, &stall_menu_value, &brig_menu_value, &locale_menu_value, &back_entry}),
        main_menu_items({&status_menu_item, &stop_menu_item, &evol_menu_value, &settings_menu, &back_entry}),
        settings_menu(&settings_label, settings_menu_items, settings_menu_items_len, change_menu, restore_menu),
        main_menu(&menu_label, main_menu_items, main_menu_items_len, change_menu, restore_menu,
                  []() { motor->decrease_speed(); },
                  []() { motor->increase_speed(); }) {
    this->_display = d;

    display = d;
    motor = m;
    active_menu = (Menu *) &main_menu;
}

void WinderMenu::cycle() {
    if (elapsed <= 100) {
        return;
    }

    _display->clear();
    print(_display, true);
    elapsed = 0;
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

void WinderMenu::print(Display *display, bool nl) const {
    if (active_menu && active_menu->is_active()) {
        active_menu->print(display, nl);
    } else {
        status.print(display, nl);
    }
}
