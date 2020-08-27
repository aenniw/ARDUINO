#pragma once

#if defined(ARDUINO_AVR_NANO)

#define DISPLAY_BCK     6
#define DISPLAY_SCK     13
#define DISPLAY_MOSI    11
#define DISPLAY_CS      10
#define DISPLAY_DC      9
#define GATE_PIN        2
#define MOTOR_PWM       9
#define ROTARY_OK       A3
#define ROTARY_CLK      A1
#define ROTARY_DT       A2

#elif defined(ARDUINO_AVR_PRO)

#define DISPLAY_BCK     6
#define DISPLAY_SCK     13
#define DISPLAY_MOSI    11
#define DISPLAY_CS      10
#define DISPLAY_DC      9
#define GATE_PIN        2
#define MOTOR_PWM       5
#define ROTARY_OK       A3
#define ROTARY_CLK      A1
#define ROTARY_DT       A2

#elif defined(ARDUINO_AVR_MEGA2560)

#define DISPLAY_BCK     12
#define DISPLAY_SCK     13
#define DISPLAY_MOSI    11
#define DISPLAY_CS      10
#define DISPLAY_DC      9
#define GATE_PIN        7
#define MOTOR_PWM       6
#define ROTARY_OK       5
#define ROTARY_CLK      14
#define ROTARY_DT       15

#endif