#include "WinderMenu.h"
#include <LocaleLabels.h>

static Menu *active_menu = nullptr;
static Motor *motor = nullptr;
static LOCALE locale = EN;

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

static void increase_speed() {
    if (motor) {
        motor->increase_speed();
    }
}

static void decrease_speed() {
    if (motor) {
        motor->decrease_speed();
    }
}

WinderMenu::WinderMenu(Display *display, Motor *m) {
    this->display = display;
    motor = m;

    active_menu = new Menu(new MonitorLabel(motor, &speed_label, &evol_label, &len_label), new Item *[5]{
            new MenuItem(new StatusLabel(motor, &start_label, &pause_label), toggle_motor),
            new MenuItem(&stop_label, reset_motor),
            new Menu(&profile_label, new Item *[3]{
                    new MenuItem(&auto_wind_label),
                    new MenuItem(&manual_wind_label),
                    new MenuItem(&back_label)
            }, 3, change_menu, restore_menu),
            new Menu(&language_label, new Item *[3]{
                    new MenuItem(&english_label, []() { locale = EN; }),
                    new MenuItem(&czech_label, []() { locale = CS; }),
                    new MenuItem(&back_label)
            }, 3, change_menu, restore_menu),
            new MenuItem(&back_label)
    }, 5, change_menu, restore_menu, decrease_speed, increase_speed);
}

void WinderMenu::cycle() {
    display->clear();
    print(locale, display, true);
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
    if (active_menu) {
        active_menu->print(locale, display, nl);
    }
}
