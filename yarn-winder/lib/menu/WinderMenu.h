#ifndef ARDUINO_PROJECTS_DISPLAYMENU_H
#define ARDUINO_PROJECTS_DISPLAYMENU_H

#include <Service.h>
#include <Display.h>
#include <Motor.h>
#include <Menu.h>
#include <LocaleLabels.h>

class WinderMenu : public TimedService, public Navigation {
private:
    const Label *profile_labels[3] = {&manual_wind_label, &semi_wind_label, &auto_wind_label};
    const Label *locale_labels[2] = {&english_label, &czech_label};
    const MonitorLabel status;
    const StatusLabel status_label;
    const ValueLabel<unsigned long> evol_value_label;
    const ValueLabel<uint16_t> stall_value_label;
    const ValueLabel<uint8_t> brig_value_label;
    const SelectionLabel<PROFILE> profile_selection_label;
    const SelectionLabel<LOCALE> locale_selection_label;

    const MenuItem status_menu_item, stop_menu_item, back_entry;
    const MenuValue profile_menu_value, stall_menu_value, brig_menu_value,
            locale_menu_value, evol_menu_value;

    const static uint8_t settings_menu_items_len = 5, main_menu_items_len = 5;
    const Item *settings_menu_items[settings_menu_items_len],
            *main_menu_items[main_menu_items_len];

    const Menu settings_menu, main_menu;
    Display *_display = nullptr;
protected:

    void print(Display *display, bool nl) const override;

public:
    WinderMenu(Display *display, Motor *motor);

    void interact() override;

    void next() override;

    void prev() override;

    void cycle() override;
};

#endif //ARDUINO_PROJECTS_DISPLAYMENU_H
