# SKARSTA - motorized table

<p align="center">
    <img src="https://www.ikea.com/us/en/images/products/skarsta-desk-sit-stand-white__0777623_PE758665_S4.JPG"/>
</p>

## Introduction

Quite some time ago, I have bought one of [Skarsta](https://www.ikea.com/us/en/p/skarsta-desk-sit-stand-white-s89324812/) tables. As its electric alternative [Bekant](https://www.ikea.com/us/en/p/bekant-desk-sit-stand-black-stained-ash-veneer-black-s29282221/) seemed to me overpriced and it doesn't have many presets for custom positions.
After testing quite a few DC motors from Ebay, Aliexpress and Banggood, I stuck with [DC-12V-180RPM Motor](https://www.banggood.com/DC-12V-180RPM-Geared-Motor-High-Torque-Gear-Reducer-Motor-p-1068573.html?p=7W02096203810201609O&stayold=1&curwarehouse=CN), that satisfies my needs at the moment. But basically, you can use any motor that you have... All you have to do is update case sizing for that motor. Other electronics can remain the same...

## Features

- display showing current position and feedback
- 3 customizable height positions with auto home
- calibration of MIN and MAX table position to avoid destroying mechanism and limiting travel range of table

<p align="center">
    <a href="https://www.youtube.com/watch?v=pk-jIIXeZvs">
        <img src="https://img.youtube.com/vi/pk-jIIXeZvs/0.jpg"/>
    </a>
</p>

<p align="center">
    <a href="https://www.youtube.com/watch?v=a_JpCyb1K0Y">
        <img src="https://img.youtube.com/vi/a_JpCyb1K0Y/0.jpg"/>
    </a>
</p>

- While display is flashing with dashes, set table to lowest position
- Press "tilde" button for 3s or until message '-rst' shows to confirm lowest position and afterwards set table to highest position
- Press "tilde" button for 3s or until message '-rst' shows once more to confirm max position
- Afterwards you can use table with min/max soft. limits and custom positions
- To set new position just press one of 1, 2, 3 buttons until display shows "set" message
- To "goto" preset position just press 1, 2, 3 buttons
- To reset min/max bounds and custom positions, press "tilde" once more as previously it was calibrated

### How-to

- **_NOTE_** your table needs to be leveled and manual adjustment shouldn't feel jerky, as it may influence on _mod_ functionality

#### Electronic cases

- All in one designs (deprecated)
  - [case_bridge-v1](./docs/case_bridge-v1.md)
  - [case_relays-v1](./docs/case_relays-v1.md)
  - for shaft cut off 100mm from the end of `Skarsta` handle or use [Hex-Key 6mm](https://www.ebay.com/itm/1-5mm-24mm-ALLEN-BALL-POINT-END-LONG-ARM-HEX-KEY-WRENCH-METRIC-ALLEN-KEY/182563068986) which will be used to connect motor to table (deprecated)

- Modullar designs
  - Electronics
    - [case_bridge-v2](./docs/case_bridge-v2.md)
    - [case_relays-v2](./docs/case_relays-v2.md)
  - Motor
    - [case_motor-v2](./docs/case_motor-v2.md)
  - Sensors
    - [sensor-rot](./docs/sensor-rot.md) - relative position
    - [sensor-hall](./docs/sensor-hall.md) - relative position
    - [sensor-sonar](./docs/sensor-sonar.md) - absolute position

#### Display + Keypad cases

- [panel-v1](./docs/panel-v1.md)
- [panel-v2](./docs/panel-v2.md)

### FAQ

- if any Error message bellow is showed on display it is needed to manually power down/up table to be able to operate once again,
  after any of error pops out it is advised to closely inspect table if the problem reoccurs if so open an issue

  | _Error code_ |         _Description_         |
  | ------------ | :---------------------------: |
  | Err 1        | Motor stuck and cannot rotate |
  | Err 2        |  Motor started unexpectedly   |
  | Err 3        |         Wiring issue          |

- in case of reversed polarity on motor update `#define REVERSE_POLARITY   false` to `true` in [configuration.h](./src/configuration.h) 

### Library Credentials

[Rotary](https://github.com/brianlow/Rotary)
[HC-SR04](https://github.com/Martinsos/arduino-lib-hc-sr04)
[TM1637](https://github.com/Seeed-Studio/Grove_4Digital_Display)
[PolymorphicButtons](https://github.com/JCWentzel/PolymorphicButtons)

### Thanks to

[iLLiac4](https://github.com/iLLiac4), See his mods on [Thingiverse](https://www.thingiverse.com/thing:3227567)
