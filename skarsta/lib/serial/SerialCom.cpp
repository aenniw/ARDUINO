#include <SerialCom.h>

bool SerialCom::begin() {
    serial.begin(baud);
    return true;
}

void SerialCom::cycle() {
    String line = get_command_line();
    if (line.length() > 0) {

        char command = line.charAt(0);
        line.remove(0, 1);

        switch (command) {
            case '%': {
                float percent = min(100, max(0, line.toFloat())) / 100;
                unsigned int position = round(
                        motor->get_end_stop_low() + (motor->get_end_stop_high() - motor->get_end_stop_low()) * percent
                );
                motor->set_position(position);
                break;
            }
            case '#': {
                long preset = line.toInt() - 1;
                if (preset >= 0 && preset < 3) {
                    motor->set_position(calibrator->get_preset(preset));
                }
                break;
            }
            case '?':
                send_motor_position();
                break;
            case 's':
                motor->off();
                break;
            case 'u':
                motor->dir_cw();
                break;
            case 'd':
                motor->dir_ccw();
                break;
            default:
                break;
        }
    }

    static unsigned int pos_watch = 0u;
    if (pos_watch != motor->get_position() && elapsed > 500) {
        pos_watch = send_motor_position();
    }
}

unsigned int SerialCom::send_motor_position() {
    unsigned int position = motor->get_position();
    serial.print(position);
    serial.print("/");
    serial.println(motor->get_end_stop_high() - motor->get_end_stop_low());

    elapsed = 0;
    return position;
}

String SerialCom::get_command_line() {
    static String line = "";

    while (serial.available()) {
        delay(3);

        int c = serial.read();
        if (c == '\n') {
            String command(line);
            line = "";
            command.trim();
            return command;
        } else if (c >= 0) {
            line += (char) c;
        }
    }
    return "";
}
