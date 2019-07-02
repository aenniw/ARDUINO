#ifdef __EEPROM__

#include <EEPROM.h>

#endif

#include <Motor.h>

static Motor *motor = nullptr;

static unsigned int position_abs(unsigned int a, unsigned int b) {
    return a >= b ? a - b : b - a;
}

Motor::Motor(Stepper *stepper) {
#ifdef __EEPROM__
    EEPROM.get(ADDRESS_POSITION, position);
    EEPROM.get(ADDRESS_END_STOP, end_stop);
    EEPROM.get(ADDRESS_MODE, mode);
#endif

#ifdef __DEBUG__
    Serial.print("m pos: ");
    Serial.println(position);
    Serial.print("m end_pos: ");
    Serial.println(end_stop);
    Serial.print("m mode:");
    Serial.println(mode);
#endif

    motor = this;
    stepper->on_step([]() {
        if (motor != nullptr) {
            motor->update_position();
        }
    });
}

void Motor::off() {
#ifdef __EEPROM__
    if (mode != UNCALIBRATED)
        updateEEPROM(ADDRESS_POSITION, position);
#endif
    _off();
#ifdef __DEBUG__
    Serial.print("m off ");
    Serial.print(end_stop);
    Serial.println();
#endif
    state = OFF;
}

void Motor::dir_cw() {
    if (disabled || (position >= end_stop && mode == CALIBRATED)) {
        return;
    }

#ifdef __DEBUG__
    Serial.println("m cw");
#endif
    _dir_cw();
    state = CW;
}

void Motor::dir_ccw() {
    if (disabled || (position <= 0 && mode != UNCALIBRATED)) {
        return;
    }

#ifdef __DEBUG__
    Serial.println("m ccw");
#endif
    _dir_ccw();
    state = CCW;
}

unsigned int Motor::get_position() {
    return position;
}

void Motor::reset_position() {
    if (disabled) return;
    this->position = 0u;
#ifdef __EEPROM__
    updateEEPROM(ADDRESS_POSITION, position);
#endif
#ifdef __DEBUG__
    Serial.println("m rst");
#endif
}

void Motor::set_position(unsigned int pos) {
    if (mode != CALIBRATED || position_abs(pos, position) < MINIMUM_POS_CHANGE) {
        return;
    }
    next_position = pos;
    if (next_position < position) {
        this->dir_ccw();
    } else {
        this->dir_cw();
    }
#ifdef __DEBUG__
    Serial.print("m pos:");
    Serial.println(pos);
#endif
}

void Motor::update_position() {
    position_change++;
    if (mode == UNCALIBRATED) {
        return;
    }

    if (state != OFF) {
        last_state = state;
    }
    if (state == CW || last_state == CW) {
        position++;
    } else if ((state == CCW || last_state == CCW) && position != 0) {
        position--;
    }

#ifdef __EEPROM__
    updateEEPROM(ADDRESS_POSITION, position);
#endif

    if (position == 0 || position >= end_stop) {
        if ((position == 0 && get_state() == CCW) ||
            (position >= end_stop && get_state() == CW)) {
            off();
        }
    }
    if (next_position >= 0 && next_position == position) {
        off();
        next_position = -1;
    }
}

MotorState Motor::get_state() {
    return this->state;
}

MotorMode Motor::get_mode() {
    return this->mode;
}

void Motor::set_mode(MotorMode mode) {
    if (disabled) return;
    this->mode = mode;
#ifdef __EEPROM__
    updateEEPROM(ADDRESS_MODE, this->mode);
#endif
#ifdef __DEBUG__
    Serial.print("m mode: ");
    Serial.println(mode);
#endif
}

void Motor::set_end_stop(unsigned int end_stop) {
    if (disabled) return;
    this->end_stop = end_stop;
#ifdef __EEPROM__
    updateEEPROM(ADDRESS_END_STOP, this->end_stop);
#endif
#ifdef __DEBUG__
    Serial.print("m end_pos: ");
    Serial.println(end_stop);
#endif
}

void Motor::initPin(uint8_t pin, uint8_t val) {
    pinMode(pin, OUTPUT);
    digitalWrite(pin, val);
}

void Motor::cycle() {};

void Motor::disable() {
    disabled = true;
    off();
}

unsigned int Motor::get_position_change() {
    unsigned int cur_change = position_change;
    position_change = 0;
    return cur_change;
}
