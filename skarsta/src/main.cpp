#include <vector>
#include <Arduino.h>
#include <Display.h>
#include <Keypad.h>
#include <configuration.h>
#ifdef __WATCHDOG__
#include <Watchdog.h>
#endif
#ifdef __H_BRIDGE_MOTOR__
#include <MotorBridge.h>
#else
#include <MotorRelay.h>
#endif

static std::vector<Service *> services;

#ifdef __EEPROM__

bool eeprom_valid() {
    for (unsigned int i = ADDRESS_MODE + 4 * sizeof(unsigned int); i < EEPROM.length(); ++i) {
        if (EEPROM[i] != 0) {
            return false;
        }
    }
    return true;
}

void eeprom_reset() {
#ifdef __DEBUG__
    Serial.println("reset eeprom");
#endif
    for (unsigned int i = 0; i < EEPROM.length(); ++i)
        updateEEPROM(i, 0);
}

#endif

#ifdef __H_BRIDGE_MOTOR__
MotorBridge motor(SENSOR_PIN0, SENSOR_PIN1, R_EN, L_EN, R_PWM, L_PWM);
#else
MotorRelay motor(SENSOR_PIN0, SENSOR_PIN1, POWER_RELAY, DIRECTION_RELAY);
#endif
Display display(DISPLAY_PIN_CLK, DISPLAY_PIN_DIO, FADE_TIMEOUT);
#ifdef __WATCHDOG__
Watchdog watchdog(&motor, &display, WATCHDOG_TIMEOUT, WATCHDOG_DEADLOCK_CHANGE, WATCHDOG_OTHER_CHANGE,
                  WATCHDOG_TOLERANCE);
#endif
Keypad keypad(&motor, &display, BUTTON_DOWN, BUTTON_UP, BUTTON_RST, BUTTON_P0, BUTTON_P1, BUTTON_P2);

void setup() {
#ifdef __DEBUG__
    Serial.begin(9600);
#endif
#ifdef __EEPROM__
    if (!eeprom_valid())
        eeprom_reset();
#endif

    delay(1000);

#ifdef __WATCHDOG__
    services.push_back((Service *) &watchdog);
#endif
    services.push_back((Service *) &keypad);
    services.push_back((Service *) &motor);
    services.push_back((Service *) &display);

#ifdef __DEBUG__
    Serial.println("starting");
#endif

    bool failed = false;
    for (const auto &service: services)
        failed |= !service->begin();

    if (failed) {
        for (const auto &service: services)
            service->disable(ERROR_INIT);
    }
}

void loop() {
    for (const auto &service: services)
        service->cycle();
}