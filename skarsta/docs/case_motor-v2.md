## Overview

<p align="center">
  <img src="img/case_motor-v2/mounted.jpg"/>
</p>

## Printing

- case frames:
  - [motor-cover.stl](../models/motor/motor-cover.stl) print with `0.3mm` profile
  - [motor-lock.stl](../models/motor/motor-lock.stl) print with `0.3mm` profile
  - [motor-mount.stl](../models/motor/motor-mount.stl) print with `0.3mm` profile, with supports
  - [motor-sleeve.stl](../models/motor/motor-sleeve.stl) print with `0.2mm` or `0.3mm` profile, ideally with `FLEX` filament
  - [shaft-template.stl](../models/shaft-template.stl) optional, template for cutting shaft print with `0.2mm` profile
  
## Shopping list

| Item                                                                                                                                                | Price   |
| --------------------------------------------------------------------------------------------------------------------------------------------------: | :-----: |
| [DC motor - 12V](https://www.banggood.com/DC-12V-180RPM-Geared-Motor-High-Torque-Gear-Reducer-Motor-p-1068573.html)                                 | ~$14,29 |
| [PSH08-02P / PFH08-02P](https://www.aliexpress.com/item/32885777326.html)                                                                           | ~$5,94  |
| [Shaft coupler 6mm-10mm](https://www.ebay.com/itm/Coupling-Shaft-Motor-Coupler-4mm-5mm-6mm-7mm-8mm-10mm-45-steel-Connector-/322250560957)           | ~$2.69  |
| [M4 - wood screw](https://www.ebay.com/itm/Self-Drilling-Drywall-Wood-Screws-M4-Bugle-Head-Coarse-Thread-Zinc-Plated/273011742134) x 4              | ~$1.50  |
| [M3-6mm](https://www.ebay.com/itm/10-20-50-100x-M2-M3-M4-M5-Stainless-Steel-Hex-Bolt-Socket-Cap-Screws-Head-DIN912/173028404303) x 12               | ~$1.62  |
| SUM                                                                                                                                                 | ~$26,04 |
| Weighted SUM                                                                                                                                        | ~$26,04 |

## Assembly

- Cut the provided `Skarsta` handle to needed size, use the provided template for cutting (paint needs to be removed from the thicker part of shaft either with sandpaper or razor knife)
  <details>
    <summary>Motor shaft</summary>
    <p align="center">
      <img src="img/case_motor-v2/shaft.jpg"/>
    </p>
  </details>
- Insert `motor-sleeve` on the motor and fix it with `6xM3-6mm` screws
  <details>
    <summary>Motor sleeve</summary>
    <p align="center">
      <img src="img/case_motor-v2/sleeve.jpg"/>
    </p>
  </details>
- Wire up the `PSH08-02P` connector to the motor and fix it with superglue to the `motor-cover`  
- Mount the `motor-mount` underneath the table with `6xM4` wood screws
  <details>
    <summary>Motor mount</summary>
    <p align="center">
      <img src="img/case_motor-v2/mount-1.jpg"/>
    </p>
  </details>
- Fix the `shaft coupler` to the shortened shaft and insert it to the table
- Insert the motor between the `motor-mount` and table all the way in, if needed adjust the shaft position
- Fix the motor to the `shaft-coupler`, if needed rotate the shaft
  <details>
    <summary>Motor mounted</summary>
    <p align="center">
      <img src="img/case_motor-v2/mount-2.jpg"/>
    </p>
  </details>
- Insert the `motor-lock` between the `motor-mount` and table all the way in and fix it with `2xM3-6mm` screws
  <details>
    <summary>Motor locked</summary>
    <p align="center">
      <img src="img/case_motor-v2/mount-3.jpg"/>
    </p>
  </details>
- Cover the motor with `motor-cover` with `4xM3-6mm` screws (move the cable to the side of the cover)
  <details>
    <summary>Mounted</summary>
    <p align="center">
      <img src="img/case_motor-v2/mounted.jpg"/>
    </p>
  </details>
