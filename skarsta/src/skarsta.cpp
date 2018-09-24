#ifdef __EEPROM__
#include <EEPROM.h>
#endif

#include <Arduino.h>
#include <Display.h>
#include <Keypad.h>

#define ENCODER_PIN_CLK 2
#define ENCODER_PIN_DIO 3
#define DISPLAY_PIN_CLK 4
#define DISPLAY_PIN_DIO 5
#define DIRECTION_RELAY 6
#define POWER_RELAY 7

Table_Data table_data = {
    .calibration = UNCALIBRATED,
    .position = 0u,
    .preset_1 = 0u,
    .preset_2 = 0u,
    .preset_3 = 0u,
    .end_stop = ~0u};
static Service **services;
static const uint8_t services_count = 3;

void setup()
{
#ifdef __EEPROM__
    table_data = EEPROM.get(EEPROM.begin(), table_data);
#endif
    if (table_data.calibration > CALIBRATED)
    {
        table_data.calibration = UNCALIBRATED;
    }
#ifdef __DEBUG__
    Serial.begin(9600);
    Serial.print("Position: ");
    Serial.println(table_data.position);
    Serial.print("Calibration: ");
    Serial.println(table_data.calibration);
    Serial.print("Preset 1: ");
    Serial.println(table_data.preset_1);
    Serial.print("Preset 2: ");
    Serial.println(table_data.preset_2);
    Serial.print("Preset 3: ");
    Serial.println(table_data.preset_3);
    Serial.print("End-stop: ");
    Serial.println(table_data.end_stop);
#endif

    auto motor = new Motor(ENCODER_PIN_CLK, ENCODER_PIN_DIO, POWER_RELAY, DIRECTION_RELAY);
    auto display = new Display(DISPLAY_PIN_CLK, DISPLAY_PIN_DIO, motor);
    services = new Service *[services_count] { (Service *)new Keypad(motor, display),
                                                   (Service *)motor,
                                                   (Service *)display };
}

void loop()
{
    for (uint8_t i = 0; i < services_count; i++)
    {
        services[i]->cycle();
    }
    delay(CYCLE_DELAY);
}