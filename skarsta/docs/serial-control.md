## Overview

[p0thi](https://github.com/p0thi) created a new service that adds the functionality to control the table with serial communication.

It also sends the current position of the table to the serial connection every time it changes, but at a maximum rate of every `500ms`.
The `SERIAL_COM_TX`/`SERIAL_COM_RX` and `SERIAL_COM_BAUD` can be configured in the [configuration.h](../src/configuration.h).
By default, software serial is used via build flag `__SERIAL_COM__`, however hardware serial can be used also via `__SERIAL_COM_HW__`. 

### Serial Input

Every command to the table has to be a new line. So in my case I add `\r\n` to every command string I send to the table.
The first character of the command signals the type of command.

It accepts the following commands:

- %\<number\>: (e.g. `%75`)
  This sets the table position to \<number\> percent. The value is cramped between 0 and 100. The number can be a float.
- \#\<number\>: (e.g. `#2`)
  Move the table to the \<number\> preset. Accepted integers are 1, 2 and 3.
- ?:
  This requests the service to send the current table position over the serial connection without changing it.
- s:
  This stops the motor.
- u:
  This starts the motor to spin cw until stopped.
- d:
  This starts the motor to spin ccw until stopped.

### Serial Output

The output to the serial connection of the table is always the current position. It will send every time the table position changes but twice per second at most.
It sends it in the following format:
\<number1\>\/<number2\> (e.g. `782/2187`). \<number1\> is the current position and \<number2\> is the maximum range of the table (end_stop[0] - end_stop[1]).

## Use Case

To control table with a `Wemos D1 mini` chip over WiFi. This chip uses [ESPHome](https://esphome.io/) and is integrated in [Home Assistant](https://www.home-assistant.io/) installation.
Example `skarsta.yaml` config looks like:

```yaml
esphome:
  name: skarsta
  platform: ESP8266
  board: d1_mini
  includes:
    - uart_read_line_sensor.h

wifi:
  ssid: <SSID>
  password: <PASSWORD>

  # Enable fallback hotspot (captive portal) in case wifi connection fails
  ap:
    ssid: "Skarsta Fallback Hotspot"
    password: "supersecretpassword"

captive_portal:

# Enable logging
logger:
  baud_rate: 9600

# Enable Home Assistant API
api:
  password: <API_PASSWORD>

ota:
  password: <OTA_PASSWORD>
  
uart:
  id: uart_bus
  tx_pin: D0
  rx_pin: D5
  baud_rate: 9600

switch:
  - platform: uart
    name: "Skarsta Preset 1"
    data: "#1\r\n"
  - platform: uart
    name: "Skarsta Preset 2"
    data: "#2\r\n"
  - platform: uart
    name: "Skarsta Preset 3"
    data: "#3\r\n"

text_sensor:
- platform: custom
  lambda: |-
    auto my_custom_sensor = new UartReadLineSensor(id(uart_bus));
    App.register_component(my_custom_sensor);
    return {my_custom_sensor};
  text_sensors:
    id: "uart_readline"
    
cover:
  - platform: template
    name: "Skarsta Manuell"
    lambda: |-
      String str = String(id(uart_readline).state.c_str());
      int index = str.indexOf('/');
      String first = str.substring(0, index);
      String second = str.substring(index + 1);
      return first.toFloat() / second.toFloat();
    has_position: true
    open_action:
      - uart.write: "u\r\n"
    close_action:
      - uart.write: "d\r\n"
    stop_action:
      - uart.write: "s\r\n"
    position_action:
      - uart.write: !lambda
          int percent = round(pos * 100);

          char numstr[21];
          std::string s = "%" + std::string(itoa(percent, numstr, 10)) + std::string("\r\n");

          std::vector<uint8_t> myVector(s.begin(), s.end());
          return myVector;
```

The `UartReadLineSensor` used in `skarsta.yaml` is in a file called `uart_read_line_sensor.h` in `ESPHome` directory and has the following contents:

```cpp
#include "esphome.h"

class UartReadLineSensor : public Component, public UARTDevice, public TextSensor {
 public:
  UartReadLineSensor(UARTComponent *parent) : UARTDevice(parent) {}

  void setup() override {
    publish_state("0/100");

    std::string s = "\r\n?\r\n";
    std::vector<uint8_t> myVector(s.begin(), s.end());

    for(const auto& value: myVector) {
      write(value);
    }
  }

  int readline(int readch, char *buffer, int len)
  {
    static int pos = 0;
    int rpos;

    if (readch > 0) {
      switch (readch) {
        case '\n': // Ignore new-lines
          break;
        case '\r': // Return on CR
          rpos = pos;
          pos = 0;  // Reset position index ready for next time
          return rpos;
        default:
          if (pos < len-1) {
            buffer[pos++] = readch;
            buffer[pos] = 0;
          }
      }
    }
    // No end of line has been found, so return -1.
    return -1;
  }

  void loop() override {
    const int max_line_length = 80;
    static char buffer[max_line_length];
    if (available() && readline(read(), buffer, max_line_length) > 0) {
      publish_state(buffer);
    }
  }
};
```