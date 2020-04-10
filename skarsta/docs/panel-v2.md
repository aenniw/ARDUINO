## Overview

<p align="center">
  <img src="https://drive.google.com/uc?export=view&id=1tUJG2tDDYfkTXWqMySgCs1FwpKBsLuZM"/>
</p>

## Printing

- [buttons.stl](../models/panel-v2/buttons.stl) - print with `0.15mm` profile, can be aligned with [buttons-inset.stl](../models/panel-v2/buttons-inset.stl) for dual-color print
- [panel-cover.stl](../models/panel-v2/panel-cover.stl) - print with `0.2mm` profile, can be aligned with [panel-inset.stl](../models/panel-v2/panel-inset.stl) for dual-color print
- [panel-bottom.stl](../models/panel-v2/panel-bottom.stl) - print with `0.2mm` profile

## Shopping list

| Item |   Price    |
| --: | :-: |
| [0.56'' TM1637 - 7 Segment display](https://www.ebay.com/itm/0-56-TM1637-4Bit-Digital-LED-7-Segment-Clock-Tube-Display-For-Arduino-White/172971714554?hash=item2845eae7fa:g:ISAAAOSweZJaOkDh) | $1.69 |
| [Tactile switches - 12x12x7.3mm](https://www.ebay.com/itm/20-500Pcs-Square-Cap-12mm-Tactile-Momentary-Push-Button-Switches-12x12x7-3mm/183319341164?hash=item2aaeaf286c:m:mG2oTMyKYmNmt5AVOSwsUYA) | $1.04 |
| [KF2510 2.54mm - 10 Pin connector set](https://www.ebay.com/itm/KF2510-2-54MM-Connector-Sets-Housing-Header-Crimps-Straight-90-2-3-4-5-10-Pin/223503280182?hash=item3409d56836:m:mNmSC5ELz_KfQSRRljflK-A) | $2.37 |
| SUM | $5.1 |
| Weighted SUM | $2.239 |

## Assembly

- Solder wires to to tactile switches and insert button caps on them
  <details>
    <summary>Tactile switches</summary>
    <p align="center">
      <img src="https://drive.google.com/uc?export=view&id=1NdkupouZzGTi3ndcAF86yFrV8r3Vo_3i"/>
    </p>
  </details>

- Insert them to top panel part and test if buttons clicks without any issue (cleanup holes if needed)
- Add super glue underneath each switch for stability
  <details>
    <summary>Front case panel</summary>
    <p align="center">
      <img src="https://drive.google.com/uc?export=view&id=1_oLoSaGsYvG6G8GuQIlrQMEGsYCcVUbr"/>
    </p>
  </details>

- Insert 7-segment display to top panel part
- Wire up all the buttons and display with connector
- Add super glue to connector and fix it to panel back hole
- Fix top and bottom case parts with 3*M3x15mm + 3*M3x10mm screws
  <details>
    <summary>Panel - screwed</summary>
    <p align="center">
      <img src="https://drive.google.com/uc?export=view&id=1LcG0IjXb3J7B-gdt-wPgCUTo8fIonSwu"/>
    </p>
  </details>
