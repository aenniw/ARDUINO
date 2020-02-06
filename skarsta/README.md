# SKARSTA - motorized table

![](https://www.ikea.com/gb/en/images/products/skarsta-desk-sit-stand-white__0324864_pe518141_s4.jpg)

## Introduction

Quite some time ago, I have bought one of [Skarsta](https://www.ikea.com/us/en/catalog/products/S29084966/) tables. As its electric alternative [Bekant](https://www.ikea.com/us/en/catalog/products/S59022528/) seemed to me overpriced and it doesn't have many preset_values for custom positions.
Design of the case and its mechanical parts (mainly motor and encoder) was updated multiple times during this time.
First version was powered by [FIXA Screwdriver](https://www.ikea.com/us/en/catalog/products/60196103/). This was great, as I was reusing all of its electronics. However, it has drawbacks, as it could not handle heavier loads of stuff on the table and PSU that was supplied, could be used only for recharging, which resulted to dead batteries sooner than expected.
After testing quite a few DC motors from Ebay, Aliexpress and Banggood, I stuck with [DC-12V-180RPM Motor](https://www.banggood.com/DC-12V-180RPM-Geared-Motor-High-Torque-Gear-Reducer-Motor-p-1068573.html?p=7W02096203810201609O&stayold=1&curwarehouse=CN), that satisfies my needs at the moment. But basically, you can use any motor that you have... All you have to do is update case sizing for that motor. Other electronics can remain the same...

## Features

- display showing current position and feedback
- 3 customizable height positions with auto home
- calibration of MIN and MAX table position to avoid destroying mechanism and limiting travel range of table

<details>
  <summary>Demo</summary>
<a href="https://drive.google.com/uc?export=view&id=11fxb0fec8Fuy7tDqFeZYRDHFgvnCaMHv"><img src="https://drive.google.com/uc?export=view&id=1hdM9_hMtFimvwPmZIGsFbmUTbFtn0tWU" style="width: 500px; max-width: 100%; height: auto" title="Click for the larger version." /></a>
</details>

<details>
  <summary>Calibration/Usage</summary>
<a href="https://drive.google.com/uc?export=view&id=1DPuMu6nJTlpt_S-GxPTESTXoy6SDZpPV"><img src="https://drive.google.com/uc?export=view&id=1b0MzVhLmEIejq30hajqHqj84zYMOo-d1" style="width: 500px; max-width: 100%; height: auto" title="Click for the larger version." /></a>
</details>

- While display is flashing with dashes, set table to lowest position
- Press "tilde" button for 3s or until message '-rst' shows to confirm lowest position and afterwards set table to highest position
- Press "tilde" button for 3s or until message '-rst' shows once more to confirm max position
- Afterwards you can use table with min/max soft. limits and custom positions
- To set new position just press one of 1, 2, 3 buttons until display shows "set" message
- To "goto" preset position just press 1, 2, 3 buttons
- To reset min/max bounds and custom positions, press "tilde" once more as previously it was calibrated

### How-to

- **_NOTE_** your table needs to be leveled and manual adjustment shouldn't feel jerky, as it may influence on _mod_ functionality
- print all parts from [Thingverse](https://www.thingiverse.com/thing:3081839) or download them directly from [models](models)
  - **_RELAY VERSION_** - all parts were printed in 0.2 mm layer height but 0.3 should be ok for [case-non-modular.stl](models/relay/case-non-modular.stl), for modular parts [case-modular-aio.stl](models/relay/case-modular-aio.stl), [case-modular-cover.stl](models/relay/case-modular-cover.stl) and [case-modular-frame.stl](models/relay/case-modular-frame.stl) 0.2 mm clearance is used so layer height 0.2 mm or lower is recommended
  - **_H-BRIDGE VERSION_** - all parts were printed in 0.2 mm layer height but 0.3 should be ok for [case-non-modular.stl](models/h-bridge/case-non-modular.stl), for modular parts [case-modular-aio.stl](models/h-bridge/case-modular-aio.stl), [case-modular-cover.stl](models/h-bridge/case-modular-cover.stl) and [case-modular-frame.stl](models/h-bridge/case-modular-frame.stl) 0.2 mm clearance is used so layer height 0.2 mm or lower is recommended
  - all parts except for [buttons.stl](models/buttons.stl), [encoder-pulley.stl](models/encoder-pulley.stl), [keypad-connector.stl](models/keypad-connector.stl) and [table-frame-tie.stl](models/table-frame-tie.stl) were printed with supports, but it should work without them...
  - [motor-pulley.stl](models/motor-pulley.stl) needs to be printed with PLA with 100% infill or you can buy one of [Shaft-Coupling 6-7mm](https://www.ebay.com/itm/2-3-4-5-6-7-8mm-Aluminum-Flexible-Shaft-Coupling-Rigid-Coupler-Motor-Connector/291882575832?ssPageName=STRK%3AMEBIDX%3AIT&var=590884618019&_trksid=p2057872.m2749.l2649) and combine it with [GT2_Skarsta_shaft.stl](https://www.thingiverse.com/download:5633328) so that it can handle more torque. **Shaft-Coupling 6-7mm** and **GT2_Skarsta_shaft.stl** can be attached together with [Super-glue](https://www.banggood.com/Suleve-M243-50mL-Threadlocker-Screw-Lock-Glue-Multipe-Surface-Medium-Strength-Anaerobic-Adhesive-p-1214087.html?p=8H180110948532015010&custlinkid=17674&cur_warehouse=CN)
  - [panel-cover-bridge.stl](models/panel-cover-bridge.stl) has a tweak for printing without support, but you need clear the first layer on the holes
  - [motor-pulley.stl](models/motor-pulley.stl) should be printed with thicker walls or with 100% infill, also print few of these as this part will get permanently damaged when accidentally hit min/max height of table
  - based on your printer accuracy, may be needed pre-drilling of screw holes
- get electronic parts:
  - [Arduino-Nano](https://www.ebay.com/itm/1PCS-mini-USB-Nano-V3-0-ATmega328P-CH340G-5V-16M-micro-controller-Arduino/191773759569?hash=item2ca69b5851:g:aHAAAOSwLN5WiNjA)
  - [Rotary encoder](https://www.ebay.com/itm/1PCS-KY-040-Rotary-Encoder-Module-Brick-Sensor-Development-For-Arduino-CA/141975888668?hash=item210e6c231c:g:6aIAAOSwq5lTmbF7)
  - **_RELAY VERSION_** - [Relay 1ch](https://www.ebay.com/itm/1-2-5-10PCS-5V-One-1-Channel-Relay-Module-Board-Shield-For-Arduino-PIC-AVR-ARM-M/302126861988?hash=item46582a0ea4%3Am%3AmO-uqxnlvpXMQXX-eRxgBwA&var=600944064917)
  - **_RELAY VERSION_** - [Relay 2ch](https://www.ebay.com/itm/5V-Two-2-Channel-Relay-Module-With-optocoupler-For-PIC-AVR-DSP-ARM-Arduino-NEW/281683101414?epid=734905713&hash=item41959f06e6:g:uBQAAOSwhOxVSLJu)
  - **_H-BRIDGE VERSION_** - [H-Bridge](https://www.ebay.com/p/Double-Bts7960b-DC-43a-Stepper-Motor-Driver-H-bridge-PWM-for-Arduino-Smart-Car/1062875951)
  - [GT2 160mm belt](https://www.ebay.com/itm/2GT-3D-Printer-Belt-Closed-Loop-Rubber-GT2-Timing-Belt-Length-110mm-852mm/132454520950?hash=item1ed6e79876%3Am%3Am1bUMmOd4qzqWTbtUS6QyGQ&var=431798850623)
  - [Power Switch](https://www.ebay.com/itm/10Pcs-12V-2-Pin-Car-Boat-Round-Dot-Light-ON-OFF-Rocker-Toggle-Switch-Tool-Black/382170389677?hash=item58fb215cad:g:D~QAAOSwHb9ZcAnX)
  - [Power connector](https://www.ebay.com/itm/10pcs-DC-005-Power-Supply-Jack-Socket-Female-PCB-Mount-Connector-5-5mm-x-2-1mm/263403309838?hash=item3d540f8b0e:g:tMgAAOSwc2FaQFNw)
  - [Step-Down module](https://www.ebay.com/itm/1PCS-DC-DC-LM2596-power-Supply-Buck-Converter-step-down-module-NEW-CA/142147326064?hash=item2118a41070:g:L1kAAOSwvzRXzOBg)
  - [Display](https://www.ebay.com/itm/0-56-TM1637-4Bit-Digital-LED-7-Segment-Clock-Tube-Display-For-Arduino-White/172971714554?hash=item2845eae7fa:g:ISAAAOSweZJaOkDh)
  - [Display buttons - 12x12x7.3mm](https://www.ebay.com/itm/50pcs-12x12x7-3mm-Vertical-Key-switch-Micro-button-4-Pin-Tactile-Switch-M3c/324010722417?_trkparms=ispr%3D1&hash=item4b708b4471:g:0H4AAOSwSn5ZewPm&enc=AQAEAAACUBPxNw%2BVj6nta7CKEs3N0qUKUAaQntkh%2BYOyR0TXDTtFVDhZP%2FAHn8%2FQnXrKqUszq%2B%2F4bxLwGFuQ3MsmCQrUYX2kX2UzuOTc1jLyYvX0TwdgSrsBNk7c%2F2yx8p3ONRp27SImgsWHY8oKyQtdm2dTdFPeI1n7U0lGPQksSd3K2Mdn1C2Ti%2FExBb4Z8Y5u%2FQ9X6OISBvN1tMzHqDZHgz4%2BFnwHu7mMD7vYhgzGlbH8hNDJ0StF6rHXk3AMoVqKPEcVXCSBY0KO4QyXZaDeAPgl74IWqwN3eRyEI%2BlKR0bUG204fQVVDdm3qlmEAUAZWpnhfDoKxI6q%2B2d2P97xBCtxlAxADkK2Ol62eFsZRCyOH2FhTXfTbkVWruSLB3CVXO6gEmnLrDHVBa%2FSBDNpXzy3blrkuw8uMRWkm5OtBkHqCNZU2wCvkpSGTaQpdx2ub%2BM0QSe38C50h52AnkCTaU1CJcv5T6u7Bb5sn%2FM8SnIHPTntNwtiOJ8z9KRG43tpkgpR05qZUwklZXwQia1KhxRVKZQRYX0lpVs9ntMbq7%2B5x3iiOGlU5ohxGTk0SZzG9rctFRX6Ths4ffzJsCAtzugHVZDtUwbDh0QV7VYBNPCm9Ssu0T%2F67z%2BrLIlbLEJYjVd32tFsGduG2jZvKiV4XMdXRYKxC1edf6VdoPBfjGP2XRnipFXxW5Z%2BEW6%2BpggnE9HqOHVrIba3zhbyVXxzuC9TcSXnstgKUWsS80H1M7RNmphrjpx0R8aRXqslWTeH4oSiTmr8EwIoO%2Fw%2F3tYx8mGkiL4%3D&checksum=3240107224178b71b42bf5484953997898b0631424a2)
  - [12V motor](https://www.banggood.com/DC-12V-180RPM-Geared-Motor-High-Torque-Gear-Reducer-Motor-p-1068573.html?p=7W02096203810201609O&stayold=1&curwarehouse=CN)
  - [PSU 12V3A - minimal](https://www.meanwell-web.com/en-gb/ac-dc-industrial-desktop-adaptor-output-12vdc-at-gst36e12--p1j)
  - [PSU 12V5A - recommended](https://www.meanwell-web.com/en-gb/ac-dc-industrial-desktop-adaptor-with-3-pin-iec320-gs60a12--p1j) (or any other kind os power source like ATX-PSU etc. that can deliver at least 12V)
  - [Fuse-mount](https://www.aliexpress.com/item/5-pcs-5-20mm-glass-fuse-holders-5x20-insurance-tube-socket-fuse-holder-for-5-20/32897554363.html?spm=2114.search0104.3.9.666c7894B6qsRn&ws_ab_test=searchweb0_0,searchweb201602_2_10065_10068_319_317_10696_453_10084_454_10083_433_10618_431_10304_10307_10820_10821_537_10302_536_10902_10059_10884_10887_100031_321_322_10103,searchweb201603_51,ppcSwitch_0&algo_expid=6030efca-06db-4a68-8c27-2d1068849aac-1&algo_pvid=6030efca-06db-4a68-8c27-2d1068849aac&transAbTest=ae803_3)
  - [Fuse 3A/4A](https://www.aliexpress.com/item/10PCS-5-20mm-Fast-Quick-Blow-Glass-Tube-Fuse-Assorted-Kit-Fast-Blow-Glass-Fuses-250V/32881363210.html?spm=2114.search0104.3.1.6b6031c3dtFTgP&ws_ab_test=searchweb0_0,searchweb201602_2_10065_10068_319_317_10696_453_10084_454_10083_433_10618_431_10304_10307_10820_10821_537_10302_536_10902_10059_10884_10887_100031_321_322_10103,searchweb201603_51,ppcSwitch_0&algo_expid=20a2b960-5b5f-4dcc-82f3-d109f11fb347-0&algo_pvid=20a2b960-5b5f-4dcc-82f3-d109f11fb347&transAbTest=ae803_3)
  - [Screws](https://www.ebay.com/itm/Hex-Socket-Bolts-Countersunk-Flat-Head-Screws-M2-M3-M4-M5-M6-Black-Alloy-Steel/183214890165?hash=item2aa8755cb5%3Am%3AmRNLwqyXcB6KXHipo6jIAWA&var=690492992291) M4-16mm for control panel and M3-20mm for motor box (may differ due to printer accuracy)
- Build & Flash Arduino-Nano via platformio
- Cut off 100mm from the end of Skarsta handle or use [Hex-Key 6mm](https://www.ebay.com/itm/1-5mm-24mm-ALLEN-BALL-POINT-END-LONG-ARM-HEX-KEY-WRENCH-METRIC-ALLEN-KEY/182563068986) which will be used to connect motor to table

#### Assembling

##### Wiring

###### **_RELAY VERSION_**

![](./img/wiring-relay.png)

###### **_H-BRIDGE VERSION_**

![](./img/wiring-h-bridge.png)

##### **_RELAY VERSION_** Motor case

- Add Arduino-Nano to case, insert it into notches a slowly push usb connector down. It should be quite tight fit
  <details>
    <summary>Arduino-Nano</summary>
    <a href="https://drive.google.com/uc?export=view&id=132wOltgyG5gLfcHMTsRL4sOw1rBVbGGI"><img src="https://drive.google.com/uc?export=view&id=132wOltgyG5gLfcHMTsRL4sOw1rBVbGGI" style="width: 500px; max-width: 100%; height: auto" title="Click for the larger version." /></a>
  </details>
- Wire up power connector, power switch, buck converter and relays
- Fix power connector with super/hot glue in case
  <details>
    <summary>Power connector + switch</summary>
    <a href="https://drive.google.com/uc?export=view&id=166TC9dmI48ojHFoScJtCKCrcVCN16JBk"><img src="https://drive.google.com/uc?export=view&id=166TC9dmI48ojHFoScJtCKCrcVCN16JBk" style="width: 500px; max-width: 100%; height: auto" title="Click for the larger version." /></a>
  </details>
- Add cables to buck converter output and mount it slowly to the case, it should be tight fit
  <details>
    <summary>Buck-converter</summary>
    <a href="https://drive.google.com/uc?export=view&id=1MY1QLGhSeExeis4uxmTsKUTAkFWPeI9L"><img src="https://drive.google.com/uc?export=view&id=1MY1QLGhSeExeis4uxmTsKUTAkFWPeI9L" style="width: 500px; max-width: 100%; height: auto" title="Click for the larger version." /></a>
  </details>
- Connect motor to relays and mount it to case, fix it with 6 screws
- Cleanup wiring and secure relays to case with few screws
  <details>
    <summary>Motor + relays</summary>
    <a href="https://drive.google.com/uc?export=view&id=1QHBbjOuYCsB3xog_ySKO66kFvDqK6Fbp"><img src="https://drive.google.com/uc?export=view&id=1QHBbjOuYCsB3xog_ySKO66kFvDqK6Fbp" style="width: 500px; max-width: 100%; height: auto" title="Click for the larger version." /></a>
  </details>
- Add Rotary encoder next to motor and fix it with screws or nut
  <details>
    <summary>Rotary encoder</summary>
    <a href="https://drive.google.com/uc?export=view&id=1GGPZeZlSmgoTHLFkNLEzZYqxAW9sUbMC"><img src="https://drive.google.com/uc?export=view&id=1GGPZeZlSmgoTHLFkNLEzZYqxAW9sUbMC" style="width: 500px; max-width: 100%; height: auto" title="Click for the larger version." /></a>
  </details>
- Insert nut & screw into motor pulley and secure it on motor shaft
- Add timing belt with encoder pulley onto encoder, readjust encoder position if belt is loosen or too tight
  <details>
    <summary>Motor/Encoder pulleys</summary>
    <a href="https://drive.google.com/uc?export=view&id=1q8UN07lRM9irNA-o0cjjysNviAEeggWU"><img src="https://drive.google.com/uc?export=view&id=1q8UN07lRM9irNA-o0cjjysNviAEeggWU" style="width: 500px; max-width: 100%; height: auto" title="Click for the larger version." /></a>
  </details>
- Wire up rest of the electronics as shown in the wiring diagram
- Fix dupont cables for display + keypad with super/hot glue into cable connector part and afterwards fix it with super/hot glue to case if needed
  <details>
    <summary>Finished motor case</summary>
    <a href="https://drive.google.com/uc?export=view&id=19tCSOthFfihupELphLvazGWEjptixe2d"><img src="https://drive.google.com/uc?export=view&id=19tCSOthFfihupELphLvazGWEjptixe2d" style="width: 500px; max-width: 100%; height: auto" title="Click for the larger version." /></a>
  </details>
- Add hexagonal rod and slowly insert it into table, if needed adjust hole for rod hole with original lever
- Fix mounted motor case with 7 screws to table
  <details>
    <summary>Mounted motor case</summary>
    <a href="https://drive.google.com/uc?export=view&id=1Mlf-zk6XFL4rruclsFIqbEq0DQg2OCfx"><img src="https://drive.google.com/uc?export=view&id=1Mlf-zk6XFL4rruclsFIqbEq0DQg2OCfx" style="width: 500px; max-width: 100%; height: auto" title="Click for the larger version." /></a>
  </details>

##### **_H-BRIDGE VERSION_** Motor case

- Add Arduino-Nano, buck converter and h-bridge to case cover plate and fix them with screws, if needed use [screw-spacer.stl](models/h-bridge/screw-spacer.stl) for buck converter
- Wire all components on case cover plate together based on wiring diagram (optionally 2 [Female 8-pin sockets](https://www.ebay.com/itm/10pcs-8-Pin-Female-Tall-Stackable-Header-Connector-Socket-For-Arduino-Shield-ZY/253903399521?hash=item3b1dd26a61:g:iKYAAOSwqVJaxCg2:rk:4:pf:1&frcectupt=true) can be inserted cover plate slot for _GND_ and _V5_)
  <details>
    <summary>Mounted case cover plate</summary>
  <a href="https://drive.google.com/uc?export=view&id=1YWug2k6Anf5VfqV33lh-GuqVLCNM8rbB"><img src="https://drive.google.com/uc?export=view&id=1YWug2k6Anf5VfqV33lh-GuqVLCNM8rbB" style="width: 500px; max-width: 100%; height: auto" title="Click for the larger version." /></a>
  </details>
- Add motor, PSU switch, PSU and keypad connector to case frame
- Add Rotary encoder next to motor and fix it with screws or nut
- Insert nut & screw into motor pulley and secure it on motor shaft
- Add timing belt with encoder pulley onto encoder, readjust encoder position if belt is loosen or too tight
  <details>
    <summary>Mounted case frame</summary>
  <a href="https://drive.google.com/uc?export=view&id=1Caumk2_G1FUvc5cQmAM8cCj6xImWTPGc"><img src="https://drive.google.com/uc?export=view&id=1Caumk2_G1FUvc5cQmAM8cCj6xImWTPGc" style="width: 500px; max-width: 100%; height: auto" title="Click for the larger version." /></a>
  </details>
- Wire up rest of the electronics as shown in the wiring diagram
- Fix dupont cables for display + keypad with super/hot glue into cable connector part and afterwards fix it with super/hot glue to case if needed
  <details>
    <summary>Case top view</summary>
  <a href="https://drive.google.com/uc?export=view&id=1CnfOpbx6Frux5znjWxagWpygVWQ5PAPc"><img src="https://drive.google.com/uc?export=view&id=1CnfOpbx6Frux5znjWxagWpygVWQ5PAPc" style="width: 500px; max-width: 100%; height: auto" title="Click for the larger version." /></a>
  </details>
- Insert 3 screws to case where cover plate overlaps frame and afterwards secure cover plate with [screw-hollow.stl](models/h-bridge/screw-hollow.stl) these will hold case parts together and prevent screws from falling out while you can still tight them through inner hole
  <details>
    <summary>Case bottom view</summary>
  <a href="https://drive.google.com/uc?export=view&id=1pt37iSBKFuvvMNw2Ou8L7yq4VtgisVa5"><img src="https://drive.google.com/uc?export=view&id=1pt37iSBKFuvvMNw2Ou8L7yq4VtgisVa5" style="width: 500px; max-width: 100%; height: auto" title="Click for the larger version." /></a>
  </details>
- Add hexagonal rod and slowly insert it into table, if needed adjust hole for rod hole with original lever
- Fix mounted motor case with 7 screws to table

##### Display + Keypad case

- Paint button caps with paint or nail polish
- Insert button caps to tactile switches
  <details>
    <summary>Tactile switches</summary>
    <a href="https://drive.google.com/uc?export=view&id=1t-OP-l0pg0Zmd32IxDXx131u2P1lAcoV"><img src="https://drive.google.com/uc?export=view&id=1t-OP-l0pg0Zmd32IxDXx131u2P1lAcoV" style="width: 500px; max-width: 100%; height: auto" title="Click for the larger version." /></a>
  </details>
- Insert them to panel top case part and fix them with super/hot glue or with their pins
  <details>
    <summary>Front case panel</summary>
    <a href="https://drive.google.com/uc?export=view&id=10cKNxXXogKxYKBh4oIO3JDHDGB05OmQT"><img src="https://drive.google.com/uc?export=view&id=10cKNxXXogKxYKBh4oIO3JDHDGB05OmQT" style="width: 500px; max-width: 100%; height: auto" title="Click for the larger version." /></a>
  </details>
- Insert 7-segment display to panel top part
  <details>
    <summary>Front case panel with display</summary>
    <a href="https://drive.google.com/uc?export=view&id=1It6w3IHo9cuEwheeEhL4n8JV32JYSgZr"><img src="https://drive.google.com/uc?export=view&id=1It6w3IHo9cuEwheeEhL4n8JV32JYSgZr" style="width: 500px; max-width: 100%; height: auto" title="Click for the larger version." /></a>
  </details>
- Pass wires through panel bottom case part and secure them few centimeters below the end with zip tie
- Wire up all the buttons and display with cables
  <details>
    <summary>Wired case panel</summary>
    <a href="https://drive.google.com/uc?export=view&id=1vIWpAW9SODAJarWg87MDfvPzOkGBGSsY"><img src="https://drive.google.com/uc?export=view&id=1vIWpAW9SODAJarWg87MDfvPzOkGBGSsY" style="width: 500px; max-width: 100%; height: auto" title="Click for the larger version." /></a>
  </details>
- Fix top and bottom case parts with 6 screws
  <details>
    <summary>Finished Display + Keypad case</summary>
    <a href="https://drive.google.com/uc?export=view&id=1TNIRP2qKsRAH4POK3g0ZD4hCG--lAydb"><img src="https://drive.google.com/uc?export=view&id=1TNIRP2qKsRAH4POK3g0ZD4hCG--lAydb" style="width: 500px; max-width: 100%; height: auto" title="Click for the larger version." /></a>
  </details>
- Connect cables to motor case and mount it with 3 screws to table
- Cleanup the cable management with few table-frame-ties
  <details>
    <summary>Mounted Display + Keypad case</summary>
    <a href="https://drive.google.com/uc?export=view&id=1aDSQ8JeYmuPXdMajL1Xrt5WNS_1qEvJo"><img src="https://drive.google.com/uc?export=view&id=1aDSQ8JeYmuPXdMajL1Xrt5WNS_1qEvJo" style="width: 500px; max-width: 100%; height: auto" title="Click for the larger version." /></a>
    <a href="https://drive.google.com/uc?export=view&id=15VFWSnQ_u37xM2864LjUWOSjwABPXVya"><img src="https://drive.google.com/uc?export=view&id=15VFWSnQ_u37xM2864LjUWOSjwABPXVya" style="width: 500px; max-width: 100%; height: auto" title="Click for the larger version." /></a>
  </details>

### Error codes && handling

- if any Error message bellow is showed on display it is needed to manually power down/up table to be able to operate once again,
  after any of error pops out it is advised to closely inspect table if the problem reoccurs if so open an issue

  | _Error code_ |         _Description_         |
  | ------------ | :---------------------------: |
  | Err 1        | Motor stuck and cannot rotate |
  | Err 2        |  Motor started unexpectedly   |

### Library Credentials

[ENCODER](https://github.com/buxtronix/arduino/tree/master/libraries/Rotary)
[TM1637](https://github.com/Seeed-Studio/Grove_4Digital_Display)
[InterruptLib](https://github.com/GreyGnome/PinChangeInt)

### Thanks to

[iLLiac4](https://github.com/iLLiac4), See his mods on [Thingiverse](https://www.thingiverse.com/thing:3227567)
