#include <Motor.h>

#ifdef __EEPROM__
#include <EEPROM.h>
#endif

static Motor *motor = nullptr;

static unsigned int position_abs(unsigned int a, unsigned int b) {
    return a >= b ? a - b : b - a;
}

Motor::Motor(uint8_t _pin1, uint8_t _pin2, uint8_t stop_diff, uint8_t min_change, bool reverse) :
        sensor((char) _pin1, (char) _pin2),
        sensor_pin_1(_pin1), sensor_pin_2(_pin2),
        pos_diff(stop_diff), min_change(min_change), reverse(reverse) {
    motor = this;
}

bool Motor::begin() {
#ifdef __EEPROM__
    EEPROM.get(ADDRESS_POSITION, position);
    EEPROM.get(ADDRESS_END_STOP_0, end_stop[0]);
    EEPROM.get(ADDRESS_END_STOP_1, end_stop[1]);
    EEPROM.get(ADDRESS_MODE, mode);
#endif
    LOG("m | pos:%d end_pos:%d-%d mode:%d", position, end_stop[0], end_stop[1], mode);
#ifdef __USENSOR__
    double sensor_value = sensor.measureDistanceCm();
    for (uint8_t i = 0; i < 5 && sensor_value < 0; i++) {
        delay(250);
        sensor_value = sensor.measureDistanceCm();
    }
    return sensor_value >= 0;
#else
    auto interrupt_routine = []() {
        motor->update_position(motor->sensor.process());
    };
    attachInterrupt((uint8_t) digitalPinToInterrupt(sensor_pin_1), interrupt_routine, CHANGE);
    attachInterrupt((uint8_t) digitalPinToInterrupt(sensor_pin_2), interrupt_routine, CHANGE);

    return true;
#endif
}

void Motor::off() {
    _off();
    LOG("m | off %d,%d", end_stop[0], end_stop[1]);
    state = OFF;
}

void Motor::dir_cw() {
    if (disabled || (get_position() >= end_stop[1] && mode == CALIBRATED)) {
        return;
    }
    LOG("m | cw");
    _dir_cw();
    state = CW;
}

void Motor::dir_ccw() {
    if (disabled || (get_position() <= end_stop[0] && mode != UNCALIBRATED)) {
        return;
    }
    LOG("m | ccw");
    _dir_ccw();
    state = CCW;
}

unsigned int Motor::get_position() const {
    return position;
}

void Motor::reset_position() {
    if (disabled) return;
#ifndef __USENSOR__
    this->position = 0u;
#endif
    this->end_stop[0] = get_position();

#ifdef __EEPROM__
    updateEEPROM(ADDRESS_END_STOP_0, this->end_stop[0]);
#ifndef __USENSOR__
    updateEEPROM(ADDRESS_POSITION, position);
#endif
#endif
    LOG("m | rst");
}

void Motor::set_position(unsigned int pos) {
    if (mode != CALIBRATED || position_abs(pos, get_position()) < min_change) {
        return;
    }
    next_position = pos;
    if (next_position < get_position()) {
        this->dir_ccw();
    } else {
        this->dir_cw();
    }
    LOG("m | pos:%d", pos);
}

#ifndef __USENSOR__

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

#endif

MotorState Motor::get_state() const {
    return this->state;
}

MotorMode Motor::get_mode() const {
    return this->mode;
}

void Motor::set_mode(MotorMode mode) {
    if (disabled) return;
    this->mode = mode;
#ifdef __EEPROM__
    updateEEPROM(ADDRESS_MODE, this->mode);
#endif
    LOG("m | mode:%d", mode);
}

void Motor::set_end_stop(unsigned int end_stop, unsigned int offset) {
    if (disabled) return;
    if (offset)
        this->position -= offset;
    this->end_stop[1] = end_stop - 2 * offset;
#ifdef __EEPROM__
    updateEEPROM(ADDRESS_END_STOP_1, this->end_stop[1]);
#endif
    LOG("m | end_pod:%d", end_stop);
}

void Motor::initPin(uint8_t pin, uint8_t val) {
    pinMode(pin, OUTPUT);
    digitalWrite(pin, val);
}


bool Motor::check_end_stops(const unsigned int end_stop_down,
                            const unsigned int end_stop_up) const {
    const MotorState state = get_state();
    const unsigned int position = get_position();
    return (state == CCW && position <= end_stop_down + pos_diff) ||
           (state == CW && position >= end_stop_up - pos_diff);
}

void Motor::cycle() {
#ifdef __USENSOR__
    double measurement_raw;

    if ((elapsed > SENSOR_TIMEOUT || position == 0) &&
        (measurement_raw = sensor.measureDistanceCm()) >= 0) {
        const unsigned int cur_position = (unsigned int) measurement_raw * 10,
                position_diff = position_abs(position, cur_position);

        if ((get_state() == OFF && position_diff > SENSOR_DELTA_OFF) ||
            (get_state() != OFF && position_diff > SENSOR_DELTA_ON)) {
            position_change += position_diff;
            position = cur_position;
        }

        elapsed = 0;
    }
#endif
    if (mode == UNCALIBRATED) {
        return;
    }

#if defined(__EEPROM__) && !defined(__USENSOR__)
    updateEEPROM(ADDRESS_POSITION, position);
#endif

    if (check_end_stops(end_stop[0], end_stop[1]) ||
        (next_position >= 0 && check_end_stops((unsigned int) (next_position),
                                               (unsigned int) (next_position)))) {
        off();
        LOG("m | s:%d p:%d np:%d", state, position, next_position);
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

unsigned int Motor::get_end_stop_low() {
    return end_stop[0];
}

unsigned int Motor::get_end_stop_high() {
    return end_stop[1];
}
