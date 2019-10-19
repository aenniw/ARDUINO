#include <vector>
#include <Arduino.h>
#include <Display.h>
#include <Keypad.h>
#include <Watchdog.h>

#ifdef __H_BRIDGE_MOTOR__
#include <MotorBridge.h>
#else
#include <MotorRelay.h>
#endif

#define ENCODER_PIN_CLK 2
#define ENCODER_PIN_DIO 3
#define DISPLAY_PIN_CLK 4
#define DISPLAY_PIN_DIO 5

#define DIRECTION_RELAY 6
#define POWER_RELAY 7

#define R_EN 7
#define L_EN 8
#define R_PWM 9
#define L_PWM 10

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
MotorBridge motor(ENCODER_PIN_CLK, ENCODER_PIN_DIO, R_EN, L_EN, R_PWM, L_PWM);
#else
MotorRelay motor(ENCODER_PIN_CLK, ENCODER_PIN_DIO, POWER_RELAY, DIRECTION_RELAY);
#endif
Display display(DISPLAY_PIN_CLK, DISPLAY_PIN_DIO);
Watchdog watchdog(&motor, &display);
Keypad keypad(&motor, &display);

void setup() {
#ifdef __DEBUG__
    Serial.begin(9600);
#endif
#ifdef __EEPROM__
    if (!eeprom_valid())
        eeprom_reset();
#endif

    delay(1000);

    services.push_back((Service *) &watchdog);
    services.push_back((Service *) &keypad);
    services.push_back((Service *) &motor);
    services.push_back((Service *) &display);

#ifdef __DEBUG__
    Serial.println("starting");
#endif

    for (const auto &service: services)
        service->begin();
}

void loop() {
    unsigned long now = millis();
    for (const auto &service: services)
        service->cycle(now);
}