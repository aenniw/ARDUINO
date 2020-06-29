#include "Calibrator.h"

unsigned int preset_values[3] = {0u};

Calibrator::Calibrator(Motor *motor) {
    this->motor = motor;

    for (uint8_t i = 0; i < 3; i++) {
#ifdef __EEPROM__
        EEPROM.get(ADDRESS_PRESETS[i], preset_values[i]);
        LOG("c | set preset:%d v:%d", i, preset_values[i]);
#endif
    }
}

void Calibrator::uncalibrated() {
    for (int i = 0; i < 3; i++) {
        preset_values[i] = 0u;
#ifdef __EEPROM__
        updateEEPROM(ADDRESS_PRESETS[i], preset_values[i]);
#endif
    }
    motor->set_mode(UNCALIBRATED);
    motor->set_end_stop(~0u);
    motor->reset_position();
}

void Calibrator::semi_calibrated() {
    motor->set_mode(SEMICALIBRATED);
    motor->reset_position();
}

void Calibrator::calibrated(unsigned int offset) {
    motor->set_mode(CALIBRATED);
    motor->set_end_stop(motor->get_position(), offset);
    if (offset)
        motor->set_position(motor->get_position() / 2);
}

void Calibrator::calibrate() {
    auto_calibrating = false;
    switch (motor->get_mode()) {
        case UNCALIBRATED:
            semi_calibrated();
            break;
        case SEMICALIBRATED:
            calibrated();
            break;
        default:
            uncalibrated();
            break;
    }
}

void Calibrator::auto_calibrate() {
#ifndef __USENSOR__
    uncalibrated();
    auto_calibrating = true;
    spin_up = millis();
    motor->dir_ccw();
    LOG("c | down");
#endif
}

void Calibrator::set_preset(uint8_t i) {
    if (motor->get_mode() != CALIBRATED)
        return;
    preset_values[i] = motor->get_position();
#ifdef __EEPROM__
    updateEEPROM(ADDRESS_PRESETS[i], preset_values[i]);
#endif
    LOG("c | set preset:%d v:%d", i, preset_values[i]);
}

unsigned int Calibrator::get_preset(uint8_t i) {
    return preset_values[i];
}

void Calibrator::cycle() {
    if (!auto_calibrating) {
        return;
    }

    if (spin_up && elapsed > SPIN_DELAY) {
        spin_up = false;
    }

    if (motor->get_state() == OFF) {
        switch (motor->get_mode()) {
            case UNCALIBRATED:
                semi_calibrated();
                elapsed = 0;
                spin_up = true;
                motor->dir_cw();
                LOG("c | up");
                break;
            case SEMICALIBRATED:
                calibrated(END_STOP_OFFSET);
                LOG("c | offset");
                break;
            case CALIBRATED:
                auto_calibrating = false;
                LOG("c | fin");
                break;
        }
    }
}

bool Calibrator::trip(ErrorType t) {
    if (!auto_calibrating || t != STOPPED)
        return false;

    if (!spin_up) {
        motor->off();
        LOG("c | trip");
    }

    return true;
}

void Calibrator::reset() {}