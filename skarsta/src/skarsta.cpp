#include <Arduino.h>
#include <Rotary.h>
#include <SevenSegmentTM1637.h>

#define CYCLE (1)
#define POWER_RELAY 6
#define DIRECTION_RELAY 7
#define ENCODER_PIN_CLK 2
#define ENCODER_PIN_DIO 3
#define DISPLAY_PIN_CLK 4
#define DISPLAY_PIN_DIO 5

#define MAX(x, y) ((x) >= (y) ? (x) : (y))
#define __DEBUG__

static Rotary encoder = Rotary(ENCODER_PIN_CLK, ENCODER_PIN_DIO);
static SevenSegmentTM1637 display(DISPLAY_PIN_CLK, DISPLAY_PIN_DIO);

static volatile unsigned int counter = 0;

void pciSetup(byte pin);

void rotate();

ISR (PCINT1_vect) // handle pin change interrupt for A0 to A5 here
{
    if (digitalRead(A0) == LOW) {
        digitalWrite(DIRECTION_RELAY, HIGH);
        delay(100);
        digitalWrite(POWER_RELAY, LOW);
#ifdef __DEBUG__
        Serial.println("left");
#endif
    } else if (digitalRead(A1) == LOW) {
        digitalWrite(DIRECTION_RELAY, LOW);
        delay(100);
        digitalWrite(POWER_RELAY, LOW);
#ifdef __DEBUG__
        Serial.println("right");
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
    attachInterrupt((uint8_t) digitalPinToInterrupt(ENCODER_PIN_CLK), rotate, CHANGE); // set interrupt
    attachInterrupt((uint8_t) digitalPinToInterrupt(ENCODER_PIN_DIO), rotate, CHANGE); // set interrupt
    for (uint8_t i = A0; i <= A5; i++) {
        digitalWrite(i, HIGH); // set pull-up
        pciSetup(i); // set interrupt
    }
    pinMode(DIRECTION_RELAY, OUTPUT);
    pinMode(POWER_RELAY, OUTPUT);
    digitalWrite(DIRECTION_RELAY, LOW);
    digitalWrite(POWER_RELAY, HIGH);

    display.begin(); // initializes the display
    display.setBacklight(100); // set the brightness to 100 %
    //display.clear(); // display INIT on the display
}

void pciSetup(byte pin) {
    *digitalPinToPCMSK(pin) |= bit (digitalPinToPCMSKbit(pin));  // enable pin
    PCIFR |= bit (digitalPinToPCICRbit(pin)); // clear any outstanding interrupt
    PCICR |= bit (digitalPinToPCICRbit(pin)); // enable interrupt for the group
}

void rotate() {
    unsigned char result = encoder.process();
    if (result == DIR_CW) {
        counter++;
    } else if (result == DIR_CCW) {
        counter--;
    }
}

void loop() {
    static int pos = 0;

    if (pos != counter) {
#ifdef __DEBUG__
        Serial.print("Position: ");
        Serial.println(counter);
#endif
        display.print(MAX(counter - (counter % CYCLE), 0) / CYCLE);
        pos = counter;
    }
    delay(100);
}

