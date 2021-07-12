#include <SerialCom.h>

SerialCom::SerialCom(Motor *motor, Calibrator *calibrator, uint8_t ixPin, uint8_t txPin) :
ixPin(ixPin), txPin(txPin){
    this->motor = motor;
    this->calibrator = calibrator;
}

bool SerialCom::begin() {
    if (NULL != mySerial) {
        delete mySerial;
    }
    mySerial = new SoftwareSerial(ixPin, txPin);
    mySerial->begin(9600);

    return true;
}

void SerialCom::disable() {
    disabled = true;
}

void SerialCom::cycle() {
    String line = read_line();
    if (line.length() > 0) {

        char command = line.charAt(0);
        line.remove(0, 1);

        switch (command)
        {
        case '%':
            {
                float percent = min(100, max(0, line.toFloat())) / 100;
                int position = round(motor->get_end_stop_low() + (motor->get_end_stop_high() - motor->get_end_stop_low()) * percent);
                if (motor->get_mode() != CALIBRATED)
                    return;
                set_motor(position);
            } break;
        case '#':
            {
                long number = line.toInt();

                switch (number)
                {
                case 1:
                case 2:
                case 3:
                    {
                        unsigned int preset = calibrator->get_preset(number - 1);
                        goto_preset(preset);
                    } break;
                }
            } break;
        case '?':
            send_motor_position(); break;
        case 's':
            motor->off(); break;
        case 'u':
            motor->dir_cw(); break;
        case 'd':
            motor->dir_ccw(); break;
            
        default:
            {
            } break;
        }
    }

    if (pos_watch != motor->get_position() && millis() - last_update > 500) {
        send_motor_position();
    }
}

void SerialCom::send_motor_position() {
    pos_watch = motor->get_position();
    mySerial->print(pos_watch);
    mySerial->print("/");
    mySerial->println(motor->get_end_stop_high() - motor->get_end_stop_low());

   last_update = millis();
}

void SerialCom::goto_preset(unsigned int preset) {
    if (motor->get_mode() != CALIBRATED)
        return;
    set_motor(preset);
}

void SerialCom::set_motor(unsigned int pos) {
    if (pos != motor->get_next_position()) {
        motor->set_position(pos);
    }
}

String SerialCom::read_line() {
    while (mySerial->available()) {
        delay(3);
        if (mySerial->available() > 0) {
            char c = mySerial->read();
            if (c != '\n') {
                readString += c;
            }
            else {
                String result(readString);
                readString = "";
                result.trim();
                return result;
            }
        }
    }
    return "";
}
