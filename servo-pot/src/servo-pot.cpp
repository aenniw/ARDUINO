#include <Arduino.h>
#include <Servo.h>

#define SERVO_MIN_ANGLE 0
#define SERVO_MAX_ANGLE 180
#define SERVO_MIN_CHANGE 3

const static uint8_t SERVO_PINS[] = {9, 8, 7},
        POT_PINS[] = {A0, A1, A2};
const static uint32_t POT_MIN[] = {0, 0, 0},
        POT_MAX[] = {699, 699, 699};
const static uint8_t SERVOS_LEN = sizeof(SERVO_PINS) / sizeof(uint8_t);
const static uint8_t PINS_LEN = sizeof(POT_PINS) / sizeof(uint8_t);

static int *SERVOS_POS;
static Servo **SERVOS = nullptr;

void setup() {
#ifdef __DEBUG__
    Serial.begin(9600);
#endif
    SERVOS_POS = new int[SERVOS_LEN];
    SERVOS = new Servo *[SERVOS_LEN];
    for (uint8_t i = 0; i < SERVOS_LEN; i++) {
        SERVOS[i] = new Servo();
        SERVOS_POS[i] = 0;
    }
#ifdef __DEBUG__
    Serial.println("Initialized, starting.");
#endif
}


int analogRead(uint8_t pin, uint8_t samples, uint8_t delay_timeout = 4) {
    long values = analogRead(pin);
    for (uint8_t i = 1; i < samples; i++) {
        delay(delay_timeout);
        values += analogRead(pin);
    }
    return static_cast<int>(values / samples);
}

void loop() {
    if (SERVOS != nullptr) {
        for (uint8_t i = 0; i < SERVOS_LEN; i++) {
            const auto pot_value = analogRead(POT_PINS[i], 5);
            const auto angle = (int) max(0, min(map(pot_value,
                                                   POT_MIN[i], POT_MAX[i],
                                                   SERVO_MIN_ANGLE, SERVO_MAX_ANGLE), 180));

            if (abs(SERVOS_POS[i] - angle) >= SERVO_MIN_CHANGE) {
#ifdef __DEBUG__
                Serial.print("Pot v:");
                Serial.print(pot_value);
                Serial.print(", ABS ");
                Serial.print(abs(SERVOS_POS[i] - angle));
                Serial.print("/");
                Serial.print(SERVOS_POS[i] - angle);
                Serial.print(" Setting servo:");
                Serial.print(i + 1);
                Serial.print(" with pa:");
                Serial.print(SERVOS_POS[i]);
                Serial.print(" with a:");
                Serial.println(angle);
#endif
                SERVOS[i]->attach(SERVO_PINS[i]);
                for (uint8_t o = 0; o < 5; o++) {
                    SERVOS[i]->write(angle);
                    delay(25);
                }
                SERVOS_POS[i] = angle;
                SERVOS[i]->detach();
            }
        }
    }
    delay(15);
}