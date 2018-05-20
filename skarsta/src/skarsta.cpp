#ifdef __EEPROM__
#include <EEPROM.h>
#endif

#include <Arduino.h>
#include <Display.h>
#include <Motor.h>
#include <Keypad.h>

#define POWER_RELAY 6
#define DIRECTION_RELAY 7
#define ENCODER_PIN_CLK 2
#define ENCODER_PIN_DIO 3
#define DISPLAY_PIN_CLK 4
#define DISPLAY_PIN_DIO 5

Table_Data table_data = {
        .calibration=UNCALIBRATED,
        .position=0,
        .preset_1=0,
        .preset_2=0,
        .preset_3=0,
        .end_stop=~0u
};
static Motor *motor;
static Display *display;
static Keypad *keypad;

ISR (PCINT1_vect) // handle pin change interrupt for A0 to A5 here
{
    if (keypad->any_button_pressed()) {
        display->display_light_up();
    }
    keypad->handle_interrupt();
}

void setup() {
#ifdef __EEPROM__
    table_data = EEPROM.get(EEPROM.begin(), table_data);
#endif
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

    display = new Display(DISPLAY_PIN_CLK, DISPLAY_PIN_DIO);
    motor = new Motor(ENCODER_PIN_CLK, ENCODER_PIN_DIO, POWER_RELAY, DIRECTION_RELAY);
    keypad = new Keypad(motor, display);
}

void loop() {
    motor->cycle();
    display->cycle();
    keypad->cycle();
    delay(CYCLE_DELAY);
}