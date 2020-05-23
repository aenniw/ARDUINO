#ifndef SKARSTA_CONFIGURATION_H
#define SKARSTA_CONFIGURATION_H

#define SENSOR_PIN0 2
#define SENSOR_PIN1 3

#define DISPLAY_PIN_CLK     4
#define DISPLAY_PIN_DIO     5
#define FADE_TIMEOUT        60000

#define BUTTON_DOWN     A0
#define BUTTON_UP       A1
#define BUTTON_P0       A2
#define BUTTON_P1       A3
#define BUTTON_P2       A4
#define BUTTON_RST      A5

/**
 * __WATCHDOG__         if watchdog is enabled
 * WATCHDOG_TIMEOUT     represents timeout in millis for watchdog to perform its checks
 * WATCHDOG_TOLERANCE   represents number of failed check in row needed to trigger an error,
 *                      so the reaction time will be (WATCHDOG_TIMEOUT * WATCHDOG_TOLERANCE) ms
 *
 * WATCHDOG_DEADLOCK_CHANGE     represents rotation steps threshold for detecting if motor is stuck and cannot move
 *                              so if rotation change is greater then WATCHDOG_DEADLOCK_CHANGE for WATCHDOG_TOLERANCE times Error 1 will be trigerred
 * WATCHDOG_OTHER_CHANGE        represents rotation steps threshold for detecting if motor was started mainly due to programing errors,
 *                              so if rotation change is greater then WATCHDOG_OTHER_CHANGE for WATCHDOG_TOLERANCE times Error 2 will be trigerred
 */
#ifdef __USENSOR__
    #define WATCHDOG_TIMEOUT        250
#else
    #define WATCHDOG_TIMEOUT        100
#endif
#define WATCHDOG_DEADLOCK_CHANGE    2
#define WATCHDOG_OTHER_CHANGE       5
#ifdef __H_BRIDGE_MOTOR__
    #define WATCHDOG_TOLERANCE      5
#else
    #define WATCHDOG_TOLERANCE      3
#endif

#define R_EN    7
#define L_EN    8
#define R_PWM   9
#define L_PWM   10

#define DIRECTION_RELAY     6
#define POWER_RELAY         7

#endif