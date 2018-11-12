/*
 * TM1637.cpp
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

#include "TM1637.h"
#include <Arduino.h>
static int8_t TubeTab[] = {0x3f, 0x06, 0x5b, 0x4f,
                           0x66, 0x6d, 0x7d, 0x07,
                           0x7f, 0x6f}; //0~9
TM1637::TM1637(uint8_t Clk, uint8_t Data)
{
  Clkpin = Clk;
  Datapin = Data;
  pinMode(Clkpin, OUTPUT);
  pinMode(Datapin, OUTPUT);
}

int TM1637::writeByte(int8_t wr_data)
{
  uint8_t i, count1;
  for (i = 0; i < 8; i++) //sent 8bit data
  {
    digitalWrite(Clkpin, LOW);
    if (wr_data & 0x01)
      digitalWrite(Datapin, HIGH); //LSB first
    else
      digitalWrite(Datapin, LOW);
    wr_data >>= 1;
    digitalWrite(Clkpin, HIGH);
  }
  digitalWrite(Clkpin, LOW); //wait for the ACK
  digitalWrite(Datapin, HIGH);
  digitalWrite(Clkpin, HIGH);
  pinMode(Datapin, INPUT);

  bitDelay();
  uint8_t ack = digitalRead(Datapin);
  if (ack == 0)
  {
    pinMode(Datapin, OUTPUT);
    digitalWrite(Datapin, LOW);
  }
  bitDelay();
  pinMode(Datapin, OUTPUT);
  bitDelay();

  return ack;
}
//send start signal to TM1637
void TM1637::start(void)
{
  digitalWrite(Clkpin, HIGH); //send start signal to TM1637
  digitalWrite(Datapin, HIGH);
  digitalWrite(Datapin, LOW);
  digitalWrite(Clkpin, LOW);
}
//End of transmission
void TM1637::stop(void)
{
  digitalWrite(Clkpin, LOW);
  digitalWrite(Datapin, LOW);
  digitalWrite(Clkpin, HIGH);
  digitalWrite(Datapin, HIGH);
}
//display function.Write to full-screen.
void TM1637::display(int8_t DispData[], bool raw = false)
{
  int8_t SegData[4];
  uint8_t i;
  for (i = 0; i < 4; i++)
  {
    SegData[i] = DispData[i];
  }
  if (!raw)
  {
    coding(SegData);
  }
  start();                //start signal sent to TM1637 from MCU
  writeByte(ADDR_AUTO);   //
  stop();                 //
  start();                //
  writeByte(Cmd_SetAddr); //
  for (i = 0; i < 4; i++)
  {
    writeByte(SegData[i]); //
  }
  stop();                  //
  start();                 //
  writeByte(Cmd_DispCtrl); //
  stop();                  //
}
//******************************************
void TM1637::display(uint8_t BitAddr, int8_t DispData, bool raw = false)
{
  int8_t SegData = DispData;
  if (!raw)
  {
    SegData = coding(DispData);
  }
  start();                   //start signal sent to TM1637 from MCU
  writeByte(ADDR_FIXED);     //
  stop();                    //
  start();                   //
  writeByte(BitAddr | 0xc0); //
  writeByte(SegData);        //
  stop();                    //
  start();                   //
  writeByte(Cmd_DispCtrl);   //
  stop();                    //
}

void TM1637::clearDisplay(void)
{
  display(0x00, 0x00, true);
  display(0x01, 0x00, true);
  display(0x02, 0x00, true);
  display(0x03, 0x00, true);
}
//To take effect the next time it displays.
void TM1637::set(uint8_t brightness, uint8_t SetData, uint8_t SetAddr)
{
  Cmd_SetData = SetData;
  Cmd_SetAddr = SetAddr;
  Cmd_DispCtrl = 0x87 + brightness; //Set the brightness and it takes effect the next time it displays.
}

//Whether to light the clock point ":".
//To take effect the next time it displays.
void TM1637::point(boolean PointFlag)
{
  _PointFlag = PointFlag;
}
void TM1637::coding(int8_t DispData[])
{
  uint8_t PointData;
  if (_PointFlag == POINT_ON)
    PointData = 0x80;
  else
    PointData = 0;
  for (uint8_t i = 0; i < 4; i++)
  {
    DispData[i] = TubeTab[DispData[i]] + PointData;
  }
}
int8_t TM1637::coding(int8_t DispData)
{
  uint8_t PointData;
  if (_PointFlag == POINT_ON)
    PointData = 0x80;
  else
    PointData = 0;
  DispData = TubeTab[DispData] + PointData;
  return DispData;
}

void TM1637::bitDelay(void)
{
  delayMicroseconds(50);
}
