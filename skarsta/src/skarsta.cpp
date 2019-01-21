#include <Arduino.h>
#include <Display.h>
#include <Keypad.h>

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

static Service **services;
static const uint8_t services_count = 3;

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

void setup() {
#ifdef __DEBUG__
    Serial.begin(9600);
#endif
#ifdef __EEPROM__
    if (!eeprom_valid())
        eeprom_reset();
#endif

    delay(1000);
#ifdef __H_BRIDGE_MOTOR__
    auto motor = new MotorBridge(ENCODER_PIN_CLK, ENCODER_PIN_DIO, R_EN, L_EN, R_PWM, L_PWM);
#else
    auto motor = new MotorRelay(ENCODER_PIN_CLK, ENCODER_PIN_DIO, POWER_RELAY, DIRECTION_RELAY);
#endif
    auto display = new Display(DISPLAY_PIN_CLK, DISPLAY_PIN_DIO);
    services = new Service *[services_count]{(Service *) new Keypad(motor, display),
                                             (Service *) motor,
                                             (Service *) display};

#ifdef __DEBUG__
    Serial.println("starting");
#endif
}

void loop() {
    for (uint8_t i = 0; i < services_count; i++) {
        services[i]->cycle();
    }
}