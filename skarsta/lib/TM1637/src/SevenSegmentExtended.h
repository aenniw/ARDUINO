/*
  SevenSegmentTM1637 - class to control a 4 digit seven segment display with a TM1636 or TM1637 driver IC
  Created by Bram Harmsen, September 25, 2015
  Released into the public domain.
  Licence: GNU GENERAL PUBLIC LICENSE V2.0

  # Changelog

  v1.0.0  25-10-2015
  * First release

*/

#ifndef SevenSegmentExtended_H
#define SevenSegmentExtended_H

#if ARDUINO >= 100
 #include <Arduino.h>
#else
 #include <WProgram.h>
#endif

#include "SevenSegmentTM1637.h"

class SevenSegmentExtended : public SevenSegmentTM1637 {

public:
SevenSegmentExtended(uint8_t pinClk, uint8_t pinDIO);
/* Prints given time to the display
@param [in] hour        hours or minutes
@param [in] min         minutes or seconds
*/
void    printTime(uint8_t hour, uint8_t min, bool blink = false);
/* Prints given time to the display
@param [in] t           time given as an int, e.g. 1643 prints 16:43
*/
void    printTime(uint16_t t, bool blink);
/* Print two one or two digit numbers to the display
* Prints a number to the left and right of the display
*
@param [in] leftCounter   the number on the left side of the display
@param [in] rightcounter  the numnber on the right side of the display
@param [in] zeroPadding   optional: pad counters with zero
*/
void    printDualCounter(int8_t leftCounter, int8_t rightCounter, bool zeroPadding = false);

};
#endif
