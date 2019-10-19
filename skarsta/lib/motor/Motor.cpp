#include <Motor.h>

#ifdef __EEPROM__
#include <EEPROM.h>
#endif

static Motor *motor = nullptr;

static unsigned int position_abs(unsigned int a, unsigned int b) {
    return a >= b ? a - b : b - a;
}

Motor::Motor(uint8_t _pin1, uint8_t _pin2) :
        encoder((char) _pin1, (char) _pin2), encoder_pin_1(_pin1), encoder_pin_2(_pin2) {
    motor = this;
}

void Motor::begin() {
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

    auto interrupt_routine = []() {
        motor->update_position(motor->encoder.process());
    };
    attachInterrupt((uint8_t) digitalPinToInterrupt(encoder_pin_1), interrupt_routine, CHANGE); // set interrupt
    attachInterrupt((uint8_t) digitalPinToInterrupt(encoder_pin_2), interrupt_routine, CHANGE); // set interrupt
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

void Motor::update_position(const unsigned char result) {
    position_change++;
    if (mode == UNCALIBRATED) {
        return;
    }

    if (result == DIR_CW) {
        position++;
    } else if (result == DIR_CCW && position != 0) {
        position--;
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

void Motor::cycle(unsigned long now) {
    if (mode == UNCALIBRATED) {
        return;
    }

#ifdef __EEPROM__
    updateEEPROM(ADDRESS_POSITION, position);
#endif

    if ((get_state() == CCW && position <= STOP_POS_DIFF) ||
        (get_state() == CW && position >= end_stop - STOP_POS_DIFF) ||
        (next_position >= 0 && position_abs(position, (unsigned int) next_position) <= STOP_POS_DIFF)) {
        off();
        next_position = -1;
    }
};

void Motor::disable() {
    disabled = true;
    off();
}

unsigned int Motor::get_position_change() {
    unsigned int cur_change = position_change;
    position_change = 0;
    return cur_change;
}
