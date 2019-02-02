#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#include <Buttons.h>
#include <limits.h>

#define LOGO_HEIGHT 16
#define LOGO_WIDTH 16
static const unsigned char PROGMEM logo_bmp[] =
        {B00000000, B11000000,
         B00000001, B11000000,
         B00000001, B11000000,
         B00000011, B11100000,
         B11110011, B11100000,
         B11111110, B11111000,
         B01111110, B11111111,
         B00110011, B10011111,
         B00011111, B11111100,
         B00001101, B01110000,
         B00011011, B10100000,
         B00111111, B11100000,
         B00111111, B11110000,
         B01111100, B11110000,
         B01110000, B01110000,
         B00000000, B00110000};


#define MOTOR_PWM 6
#define MIN_SPEED 100
#define MAX_SPEED 255
#define SPEED_STEP_DURATION 50

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET 4
static Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

#define BUTTON_PLUS 8
#define BUTTON_MINUS 9
static ToggleButton *down = nullptr, *up = nullptr;
volatile static uint8_t speed = 0;
typedef enum {
    NONE,
    PLUS,
    MINUS
} ButtonState;
volatile ButtonState state = NONE;

unsigned long get_period(const unsigned long last, const unsigned long next) {
    if (last > next) //overflow
    {
        return (ULONG_MAX - last) + next;
    }
    return next - last;
}

void print(int speed) {
    display.clearDisplay();

    display.setTextSize(3);
    display.setTextColor(WHITE);
    display.setCursor(48, 24);
    display.println(speed);
    display.display();
}

void testdrawbitmap(void) {
    display.clearDisplay();

    display.drawBitmap(
            (display.width() - LOGO_WIDTH) / 2,
            (display.height() - LOGO_HEIGHT) / 2,
            logo_bmp, LOGO_WIDTH, LOGO_HEIGHT, 1);
    display.display();
    delay(1000);
}

void setup() {
    Serial.begin(9600);

    // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
    if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3C for 128x32
        Serial.println(F("SSD1306 allocation failed"));
        for (;;); // Don't proceed, loop forever
    }
    // Clear the buffer
    display.clearDisplay();
    display.display();
    testdrawbitmap(); // Draw a small bitmap image

    pinMode(MOTOR_PWM, OUTPUT);
    down = new ToggleButton(BUTTON_PLUS, []() { state = PLUS; }, []() { state = NONE; });
    up = new ToggleButton(BUTTON_MINUS, []() { state = MINUS; }, []() { state = NONE; });

    delay(1500);
    display.clearDisplay();
    display.display();
}

void loop() {
    static unsigned long last_tick = millis();
    unsigned long now = millis(), diff = get_period(last_tick, now);

    if (diff >= SPEED_STEP_DURATION) {
        if (state == MINUS && speed > 0) {
            speed = speed <= MIN_SPEED ? 0 : speed - 1;
            analogWrite(MOTOR_PWM, speed);
        } else if (state == PLUS && speed < MAX_SPEED) {
            speed = speed < MIN_SPEED ? MIN_SPEED : (speed + 1) % (MAX_SPEED + 1);
            analogWrite(MOTOR_PWM, speed);
        }

        last_tick = now;
    }

    print(speed < MIN_SPEED ? 0 : (uint8_t) ((1 + speed - MIN_SPEED) / (float) (1 + MAX_SPEED - MIN_SPEED) * 100));
}
