#pragma once

#define SENSOR_PIN0 2
#define SENSOR_PIN1 3

#define DISPLAY_PIN_CLK     4
#define DISPLAY_PIN_DIO     5
#define FADE_TIMEOUT        60000

#define BUTTON_DOWN     A0 // -> 14
#define BUTTON_UP       A1 // -> 15
#define BUTTON_P0       A2 // -> 16
#define BUTTON_P1       A3 // -> 17
#define BUTTON_P2       A4 // -> 18
#define BUTTON_RST      A5 // -> 19

/**
 * __WATCHDOG__         if watchdog is enabled
 * WATCHDOG_TIMEOUT     represents timeout in millis for watchdog to perform its checks
 * WATCHDOG_TOLERANCE   represents number of failed check in row needed to trigger an error,
 *                      so the reaction time will be (WATCHDOG_TIMEOUT * WATCHDOG_TOLERANCE) ms
 *
 * WATCHDOG_DEADLOCK_CHANGE     represents rotation steps threshold for detecting if motor is stuck and cannot move
 *                              so if rotation change is greater then WATCHDOG_DEADLOCK_CHANGE for WATCHDOG_TOLERANCE times Error 1 will be triggered
 * WATCHDOG_OTHER_CHANGE        represents rotation steps threshold for detecting if motor was started mainly due to programing errors,
 *                              so if rotation change is greater then WATCHDOG_OTHER_CHANGE for WATCHDOG_TOLERANCE times Error 2 will be triggered
 * WATCHDOG_OTHER_SLEEP         represents timeout in seconds after which Error 2 check is not performed, 0 value no timeout is applied
 */
#ifdef __USENSOR__
    #define WATCHDOG_TIMEOUT        300
#else
    #define WATCHDOG_TIMEOUT        100
#endif
#define WATCHDOG_DEADLOCK_CHANGE    6
#define WATCHDOG_OTHER_CHANGE       6
#ifndef WATCHDOG_OTHER_SLEEP
    #define WATCHDOG_OTHER_SLEEP    0
#endif
#ifdef __H_BRIDGE_MOTOR__
    #define WATCHDOG_TOLERANCE      5
#else
    #define WATCHDOG_TOLERANCE      3
#endif

#define STOP_POS_DIFF       1   // Offset distance for preset end stop
#define MINIMUM_POS_CHANGE  8   // Minimum distance for presets to be invoked

#define R_EN    7
#define L_EN    8
#define R_PWM   9
#define L_PWM   10

#define REVERSE_POLARITY   false
#define DIRECTION_RELAY     6
#define POWER_RELAY         7

#define SERIAL_COM_RX   12      // Ignored if HW serial is used
#define SERIAL_COM_TX   11      // Ignored if HW serial is used
#define SERIAL_COM_BAUD 9600