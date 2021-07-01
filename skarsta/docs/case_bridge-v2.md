## Overview

<p align="center">
  <img src="img/case_bridge-v1/wiring.png"/>
</p>

## Printing

- case frames:
  - [ecase-bridge.stl](../models/ecase/ecase-bridge.stl) - print with `0.2mm` or `0.3mm` profile with support enforcers on keypad and sensor holes
  - [emount.stl](../models/ecase/emount.stl) - print with `0.2mm` or `0.3mm` profile
  
## Shopping list

| Item                                                                                                                                                | Price   |
| --------------------------------------------------------------------------------------------------------------------------------------------------: | :-----: |
| [Arduino-Nano](https://www.ebay.com/itm/2-5-10PCS-USB-Nano-V3-0-ATmega328-16M-5V-Micro-controller-CH340G-Board-Arduino/173636038739)                | ~$2,57  |
| [BTS7960B - H-Bridge](https://www.ebay.com/itm/BTS7960B-43A-Double-DC-Stepper-Motor-Driver-H-Bridge-PWM-Arduino-Smart-Car-New/284070305999)         | ~$6,85  |
| [Switch-On/Off](https://www.ebay.com/itm/10Pcs-12V-2-Pin-Car-Boat-Round-Dot-Light-ON-OFF-Rocker-Toggle-Switch-Tool-Black/382170389677)              | ~$1,54  |
| [PSU-Jack](https://www.ebay.com/itm/10x-DC-Power-Panel-Mount-Female-Socket-Connector-Jack-Plug-5-5x2-1mm/332050246178)                              | ~$3.20  |
| [LM2596-step-down](https://www.ebay.com/itm/LM2596-Step-Down-Module-DC-3V-40V-to1-5v-35V-3-3V-5V-12V-3A-Voltage-Regulator-US/162648939028)          | ~$1.44  |
| [KF2510 - 2.54mm - 10P](https://www.aliexpress.com/item/4000229636156.html?spm=a2g0s.9042311.0.0.426d4c4d4E2JyY)                                    | ~1.97   |
| [KF2510 - 2.54mm - 4P](https://www.aliexpress.com/item/4000229636156.html?spm=a2g0s.9042311.0.0.426d4c4d4E2JyY)                                     | ~$1.39  |
| [PSH08-02P / PFH08-02P](https://www.ebay.com/itm/10-Pairs-6-2mm-2-Pin-Male-Female-JST-SM-Housing-Crimp-Terminal-Connector/362591186288)             | ~$5,94  |
| [PSU 12V3A - minimal](https://www.meanwell-web.com/en-gb/ac-dc-industrial-desktop-adaptor-output-12vdc-at-gst36e12--p1j)                            | ~$16,41 |
| [PSU 12V5A - recommended](https://www.meanwell-web.com/en-gb/ac-dc-industrial-desktop-adaptor-with-3-pin-iec320-gs60a12--p1j)                       | ~$18,78 |
| [Fuse-mount](https://www.aliexpress.com/item/32897554363.html)                                                                                      | ~$0.85  |
| [Fuse 3A/4A](https://www.aliexpress.com/item/10PCS-5-20mm-Fast-Quick-Blow-Glass-Tube-Fuse-Assorted-Kit-Fast-Blow-Glass-Fuses-250V/32881363210.html) | ~$0.69  |
| [M4 - wood screw](https://www.ebay.com/itm/Self-Drilling-Drywall-Wood-Screws-M4-Bugle-Head-Coarse-Thread-Zinc-Plated/273011742134) x 3              | ~$1.10  |
| [M3-6mm](https://www.ebay.com/itm/10-20-50-100x-M2-M3-M4-M5-Stainless-Steel-Hex-Bolt-Socket-Cap-Screws-Head-DIN912/173028404303) x 4                | ~$1.62  |
| SUM                                                                                                                                                 | ~$54,63 |
| Weighted SUM                                                                                                                                        | ~$55,25 |

## Assembly

- Add Arduino-Nano to case, insert it into notches a slowly push usb connector down. It should be quite tight fit
- Wire up motor power connector with `H-bridge` and secure it to case with 4 screws, fix the power connector with super glue
  <details>
    <summary>Arduino + H-bridge</summary>
    <p align="center">
      <img src="img/case_bridge-v2/electronics-1.jpg"/>
    </p>
  </details>
- Wire up buck converter and secure it to case with 2 screws
  <details>
    <summary>Buck converter</summary>
    <p align="center">
      <img src="img/case_bridge-v2/electronics-2.jpg"/>
    </p>
  </details>
- Mount power switch, fuse holder and power connecter onto frame
- Wire up power switch, fuse holder and power connecter to the rest of the components
  <details>
    <summary>Connectors - back</summary>
    <p align="center">
      <img src="img/case_bridge-v2/electronics-3.jpg"/>
    </p>
  </details>
- Prepare panel and sensor connectors
  <details>
    <summary>Connectors - front</summary>
    <p align="center">
      <img src="img/case_bridge-v2/connectors.jpg"/>
    </p>
  </details>
- Press fit panel and sensor connectors to case and fix them with super glue
  <details>
    <summary>Connectors - mounted</summary>
    <p align="center">
      <img src="img/case_bridge-v2/electronics-4.jpg"/>
    </p>
  </details>
- Wire up rest of the electronics as shown in the wiring diagram
- Mount case holder onto table with 3 wood screws
  <details>
    <summary>Mount</summary>
    <p align="center">
      <img src="img/case_bridge-v2/mount.jpg"/>
    </p>
  </details>
- Slide case onto mount rails and fix it in plase with 1 screw
  <details>
    <summary>Mounted</summary>
    <p align="center">
      <img src="img/case_bridge-v2/case.jpg"/>
    </p>
  </details>