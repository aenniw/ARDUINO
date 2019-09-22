#ifndef ARDUINO_PROJECTS_LOCALES_H
#define ARDUINO_PROJECTS_LOCALES_H

#include <avr/pgmspace.h>

const uint8_t locale_count = 2;

const char menu_en[] PROGMEM = {"Main menu"};
const char start_en[] PROGMEM = {"Start"};
const char pause_en[] PROGMEM = {"Pause"};
const char reset_en[] PROGMEM = {"Reset"};
const char profile_en[] PROGMEM = {"Profile"};
const char stall_en[] PROGMEM = {"Stall senst."};
const char lang_en[] PROGMEM = {"Language"};
const char brig_en[] PROGMEM = {"Brightness"};
const char settings_en[] PROGMEM = {"Settings"};
const char back_en[] PROGMEM = {"Back"};
const char speed_wind_en[] PROGMEM = {"Speed"};
const char evol_wind_en[] PROGMEM = {"Evol"};
const char bat_en[] PROGMEM = {"Battery"};
const char evol_max_wind_en[] PROGMEM = {"Evol. max"};
const char len_wind_en[] PROGMEM = {"Length"};

const char menu_cs[] PROGMEM = {"Hlavni menu"};
const char start_cs[] PROGMEM = {"Zacit"};
const char pause_cs[] PROGMEM = {"Pauza"};
const char reset_cs[] PROGMEM = {"Vynulovat"};
const char profile_cs[] PROGMEM = {"Profil"};
const char stall_cs[] PROGMEM = {"Cit. zasek"};
const char lang_cs[] PROGMEM = {"Jazyk"};
const char brig_cs[] PROGMEM = {"Jas"};
const char settings_cs[] PROGMEM = {"Nastaveni"};
const char back_cs[] PROGMEM = {"Zpet"};
const char speed_wind_cs[] PROGMEM = {"Rychlost"};
const char evol_wind_cs[] PROGMEM = {"Ot"};
const char bat_cs[] PROGMEM = {"Baterie"};
const char evol_max_wind_cs[] PROGMEM = {"Ot. max"};
const char len_wind_cs[] PROGMEM = {"Delka"};

const char *const menu[] PROGMEM = {menu_en, menu_cs};
const char *const start[] PROGMEM = {start_en, start_cs};
const char *const pause[] PROGMEM = {pause_en, pause_cs};
const char *const reset[] PROGMEM = {reset_en, reset_cs};
const char *const profile[] PROGMEM = {profile_en, profile_cs};
const char *const stall[] PROGMEM = {stall_en, stall_cs};
const char *const lang[] PROGMEM = {lang_en, lang_cs};
const char *const brig[] PROGMEM = {brig_en, brig_cs};
const char *const settings[] PROGMEM = {settings_en, settings_cs};
const char *const back[] PROGMEM = {back_en, back_cs};
const char *const speed_wind[] PROGMEM = {speed_wind_en, speed_wind_cs};
const char *const evol_wind[] PROGMEM = {evol_wind_en, evol_wind_cs};
const char *const bat[] PROGMEM = {bat_en, bat_cs};
const char *const evol_max_wind[] PROGMEM = {evol_max_wind_en, evol_max_wind_cs};
const char *const len_wind[] PROGMEM = {len_wind_en, len_wind_cs};

const char english[] PROGMEM = {"ENGLISH"};
const char czech[] PROGMEM = {"CZECH"};

const char auto_wind[] PROGMEM = {"Auto"};
const char semi_wind[] PROGMEM = {"Semi"};
const char manual_wind[] PROGMEM = {"Manual"};

#endif //ARDUINO_PROJECTS_LOCALES_H
