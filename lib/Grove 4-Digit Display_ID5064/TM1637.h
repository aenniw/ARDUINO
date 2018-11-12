/*
 * TM1637.h
 * A library for the 4 digit display
 *
 * Copyright (c) 2012 seeed technology inc.
 * Website    : www.seeed.cc
 * Author     : Frankie.Chu
 * Create Time: 9 April,2012
 * Change Log :
 *
 * The MIT License (MIT)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#ifndef TM1637_h
#define TM1637_h
#include <inttypes.h>
#include <Arduino.h>
//************definitions for TM1637*********************
#define ADDR_AUTO 0x40
#define ADDR_FIXED 0x44

#define STARTADDR 0xc0
/**** definitions for the clock point of the digit tube *******/
#define POINT_ON 1
#define POINT_OFF 0
/**************definitions for brightness***********************/
#define BRIGHT_LOW 1
#define BRIGHT_MEDIUM 3
#define BRIGHT_HIGH 8

class TM1637
{
public:
  uint8_t Cmd_SetData;
  uint8_t Cmd_SetAddr;
  uint8_t Cmd_DispCtrl;
  boolean _PointFlag; //_PointFlag=1:the clock point on
  TM1637(uint8_t, uint8_t);
  void display(int8_t DispData[], bool raw = false);
  void display(uint8_t BitAddr, int8_t DispData, bool raw = false);
  void clearDisplay(void);
  void set(uint8_t = BRIGHT_HIGH, uint8_t = 0x40, uint8_t = 0xc0); //To take effect the next time it displays.
  void point(boolean PointFlag);                                   //whether to light the clock point ":".To take effect the next time it displays.

protected:
  void start(void);              //send start bits
  void stop(void);               //send stop bits
  int writeByte(int8_t wr_data); //write 8bit data to tm1637
  void coding(int8_t DispData[]);
  int8_t coding(int8_t DispData);
  void bitDelay(void);

private:
  uint8_t Clkpin;
  uint8_t Datapin;
};
#endif
