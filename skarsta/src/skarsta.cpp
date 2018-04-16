#include <Arduino.h>
#include <EEPROM.h>
#include <Rotary.h>
#include <SevenSegmentTM1637.h>

#define POWER_RELAY 6
#define DIRECTION_RELAY 7
#define ENCODER_PIN_CLK 2
#define ENCODER_PIN_DIO 3
#define DISPLAY_PIN_CLK 4
#define DISPLAY_PIN_DIO 5

#define CYCLE_DELAY 100
#define DISPLAY_FADEOUT_TIMEOUT 25000
#define DISPLAY_FADEOUT_DURATION 10000
#define ENCODER_FULL_ROTATION_STEPS (1)

#define MAX(x, y) ((x) >= (y) ? (x) : (y))
#define __DEBUG__

static Rotary encoder = Rotary(ENCODER_PIN_CLK, ENCODER_PIN_DIO);
static SevenSegmentTM1637 display(DISPLAY_PIN_CLK, DISPLAY_PIN_DIO);

// EEPROM data
static bool encoder_uncalibrated = false;
static int encoder_uncalibrated_address = EEPROM.begin();
static volatile unsigned int encoder_counter = 0;
static int encoder_counter_address = encoder_uncalibrated_address + sizeof(bool);

// variables
static unsigned int fadeout_cycle_counter = 0;
const unsigned int fade_end_begin = (DISPLAY_FADEOUT_TIMEOUT) / (CYCLE_DELAY);
const unsigned int fade_end_step = fade_end_begin + (DISPLAY_FADEOUT_DURATION) / (CYCLE_DELAY);
const unsigned int fade_step = 100 / (fade_end_step - fade_end_begin);

void pciSetup(byte pin);

void encoder_rotate();

void display_light_up();

void display_fading();

void display_update();

bool any_button_pressed();

ISR (PCINT1_vect) // handle pin change interrupt for A0 to A5 here
{
    if (any_button_pressed()) {
        display_light_up();
    }

    if (digitalRead(A0) == LOW) {
        digitalWrite(DIRECTION_RELAY, HIGH);
        delay(100);
        digitalWrite(POWER_RELAY, LOW);
#ifdef __DEBUG__
        Serial.println("up");
#endif
    } else if (digitalRead(A1) == LOW) {
        digitalWrite(DIRECTION_RELAY, LOW);
        delay(100);
        digitalWrite(POWER_RELAY, LOW);
#ifdef __DEBUG__
        Serial.println("down");
#endif
    } else {
        digitalWrite(DIRECTION_RELAY, LOW);
        delay(100);
        digitalWrite(POWER_RELAY, HIGH);
#ifdef __DEBUG__
        Serial.println("off");
#endif
    }
}

void setup() {
#ifdef __DEBUG__
    Serial.begin(9600);
#endif
    attachInterrupt((uint8_t) digitalPinToInterrupt(ENCODER_PIN_CLK), encoder_rotate, CHANGE); // set interrupt
    attachInterrupt((uint8_t) digitalPinToInterrupt(ENCODER_PIN_DIO), encoder_rotate, CHANGE); // set interrupt
    for (uint8_t i = A0; i <= A5; i++) {
        digitalWrite(i, HIGH); // set pull-up
        pciSetup(i); // set interrupt
    }
    pinMode(DIRECTION_RELAY, OUTPUT);
    pinMode(POWER_RELAY, OUTPUT);
    digitalWrite(DIRECTION_RELAY, LOW);
    digitalWrite(POWER_RELAY, HIGH);

    display.begin();
    display_light_up();

    encoder_uncalibrated = EEPROM.get(encoder_uncalibrated_address, encoder_uncalibrated);
    if (!encoder_uncalibrated) {
        encoder_counter = EEPROM.get(encoder_counter_address, encoder_counter);
    } else {
        display.print("----");
    }
}

void loop() {
    display_update();
    display_fading();
    delay(CYCLE_DELAY);
}

void pciSetup(byte pin) {
    *digitalPinToPCMSK(pin) |= bit (digitalPinToPCMSKbit(pin));  // enable pin
    PCIFR |= bit (digitalPinToPCICRbit(pin)); // clear any outstanding interrupt
    PCICR |= bit (digitalPinToPCICRbit(pin)); // enable interrupt for the group
}

void encoder_rotate() {
    unsigned char result = encoder.process();

    if (encoder_uncalibrated)
        return;
    if (result == DIR_CW) {
        encoder_counter++;
    } else if (result == DIR_CCW) {
        encoder_counter--;
    }
    EEPROM.put(encoder_counter_address, encoder_counter);
}

bool any_button_pressed() {
    bool anyPressed = false;
    for (uint8_t i = A0; i <= A5 && !anyPressed; i++) {
        anyPressed = (bool) digitalRead(i) == LOW;
    }
    return anyPressed;
}

void display_light_up() {
    display.setBacklight(100);
    fadeout_cycle_counter = 0;
}

void display_fading() {
    if (fadeout_cycle_counter >= fade_end_step)
        return;

    fadeout_cycle_counter++;
    if (fadeout_cycle_counter >= fade_end_begin) {
        const uint8_t brightness = 100 - ((fadeout_cycle_counter - fade_end_begin) * fade_step);
#ifdef __DEBUG__
        Serial.print("Backlight: ");
        Serial.print(fade_end_begin);
        Serial.print(" ");
        Serial.print(fade_end_step);
        Serial.print(" ");
        Serial.print(fadeout_cycle_counter);
        Serial.print(" ");
        Serial.println(brightness);
#endif
        display.setBacklight(brightness);
    }
}

void display_update() {
    if (encoder_uncalibrated)
        return;
    static unsigned int displayed_counter = 0;
    if (displayed_counter != encoder_counter) {
#ifdef __DEBUG__
        Serial.print("Position: ");
        Serial.println(encoder_counter);
#endif
        display.println(MAX(encoder_counter - (encoder_counter % ENCODER_FULL_ROTATION_STEPS), 0) /
                        ENCODER_FULL_ROTATION_STEPS);
        displayed_counter = encoder_counter;
    }
}