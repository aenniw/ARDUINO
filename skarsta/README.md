# SKARSTA - motorized table

![](https://www.ikea.com/gb/en/images/products/skarsta-desk-sit-stand-white__0324864_pe518141_s4.jpg)

## Introduction

Quite ago I have bought one of [Skarsta](https://www.ikea.com/us/en/catalog/products/S29084966/) as its electric alternative [Bekant](https://www.ikea.com/us/en/catalog/products/S59022528/) seemed to me overpriced as it doesn't many presets for custom positions.
Design of case and its mechanical parts mainly motor and encoder was updated multiple times during the time.
First version was powered by [FIXA Screwdriver](https://www.ikea.com/us/en/catalog/products/60196103/) this was great as I was reusing all of its electronics. However it have drawbacks as it could not handle heavier loads and PSU that was supplied can be used only for recharging which resulted to dead batteries sooner that expected.
After testing quite a few DC motors from Ebay, Aliexpres and Banggood I stuck with [DC-12V-180RPM Motor](https://www.banggood.com/DC-12V-180RPM-Geared-Motor-High-Torque-Gear-Reducer-Motor-p-1068573.html?p=7W02096203810201609O&stayold=1&curwarehouse=CN) that satisfies my needs at the moment, but basically you can use any motor that you have... all you have to do is update case for that motor other electronics can remain the same...

## Features

- display showing current position and feedback
- 3 customizable height position with auto home
- calibration of MIN and MAX table position to avoid destroying mechanism and limiting travel range of table

### How-to

- print all parts from [Thingverse](https://www.thingiverse.com/thing:3081839) or download them directly from [models](models)
- get electronic parts (Total Cost ~25€):
  [Arduino nano](https://www.ebay.com/itm/1PCS-mini-USB-Nano-V3-0-ATmega328P-CH340G-5V-16M-micro-controller-Arduino/191773759569?hash=item2ca69b5851:g:aHAAAOSwLN5WiNjA),
  [Rotary encoder](https://www.ebay.com/itm/1PCS-KY-040-Rotary-Encoder-Module-Brick-Sensor-Development-For-Arduino-CA/141975888668?hash=item210e6c231c:g:6aIAAOSwq5lTmbF7),
  [Relay 1ch](https://www.ebay.com/itm/1-2-5-10PCS-5V-One-1-Channel-Relay-Module-Board-Shield-For-Arduino-PIC-AVR-ARM-M/302126861988?hash=item46582a0ea4%3Am%3AmO-uqxnlvpXMQXX-eRxgBwA&var=600944064917),
  [Relay 2ch](https://www.ebay.com/itm/5V-Two-2-Channel-Relay-Module-With-optocoupler-For-PIC-AVR-DSP-ARM-Arduino-NEW/281683101414?epid=734905713&hash=item41959f06e6:g:uBQAAOSwhOxVSLJu),
  [GT2 160mm belt](https://www.ebay.com/itm/2GT-3D-Printer-Belt-Closed-Loop-Rubber-GT2-Timing-Belt-Length-110mm-852mm/132454520950?hash=item1ed6e79876%3Am%3Am1bUMmOd4qzqWTbtUS6QyGQ&var=431798850623),
  [Power Switch](https://www.ebay.com/itm/10Pcs-12V-2-Pin-Car-Boat-Round-Dot-Light-ON-OFF-Rocker-Toggle-Switch-Tool-Black/382170389677?hash=item58fb215cad:g:D~QAAOSwHb9ZcAnX),
  [Power connector](https://www.ebay.com/itm/10pcs-DC-005-Power-Supply-Jack-Socket-Female-PCB-Mount-Connector-5-5mm-x-2-1mm/263403309838?hash=item3d540f8b0e:g:tMgAAOSwc2FaQFNw),
  [StepDown module](https://www.ebay.com/itm/1PCS-DC-DC-LM2596-power-Supply-Buck-Converter-step-down-module-NEW-CA/142147326064?hash=item2118a41070:g:L1kAAOSwvzRXzOBg),
  [Display](https://www.ebay.com/itm/0-56-TM1637-4Bit-Digital-LED-7-Segment-Clock-Tube-Display-For-Arduino-White/172971714554?hash=item2845eae7fa:g:ISAAAOSweZJaOkDh),
  [Display buttons](https://www.ebay.com/itm/20Set-Momentary-Tactile-Push-Button-Touch-Micro-Switch-4P-PCB-Caps12x12x7-3mm-XW/113218977191?hash=item1a5c607da7:g:ay4AAOSwkVxa8RE4),
  [12V motor](https://www.banggood.com/DC-12V-180RPM-Geared-Motor-High-Torque-Gear-Reducer-Motor-p-1068573.html?p=7W02096203810201609O&stayold=1&curwarehouse=CN),
  [PSU 12V3A - minimal](https://www.ebay.com/itm/EU-Plug-1A-2A-3A-5A-Power-Supply-Adapter-100V-240V-to-12V-Lighting-Transformer/322850236046?hash=item4b2b5fa68e%3Am%3Amjgjm0i78jYJH9-2bkZdCyg&var=511979243408),
  [PSU 12V5A - recommended](https://www.ebay.com/itm/EU-Plug-1A-2A-3A-5A-Power-Supply-Adapter-100V-240V-to-12V-Lighting-Transformer/322850236046?hash=item4b2b5fa68e%3Am%3Amjgjm0i78jYJH9-2bkZdCyg&var=511979243408),
  [M2 Screws](https://www.ebay.com/itm/Hex-Socket-Bolts-Countersunk-Flat-Head-Screws-M2-M3-M4-M5-M6-Black-Alloy-Steel/183214890165?hash=item2aa8755cb5%3Am%3AmRNLwqyXcB6KXHipo6jIAWA&var=690492992291)
- Build & Flash Arduino nano via platformio
- Cut off 100mm from the end of Skarsta handle which will be used to connect motor to table
- Assemble all parts together and mount them to table

### References

![](https://cdn.thingiverse.com/renders/84/36/45/45/12/885c656599cac7bea0320a0cf7080532_preview_featured.jpg)
![](https://cdn.thingiverse.com/renders/49/da/49/3c/ec/377d4a664dd0c668cbe15b86c6c594ee_preview_featured.jpg)

### Library Credentials

[TM1637](https://github.com/bremme/arduino-tm1637),
[ENCODER](https://github.com/buxtronix/arduino/tree/master/libraries/Rotary)
