#ifdef __EEPROM__
#include <EEPROM.h>
#endif

#include <Motor.h>
#include <Rotary.h>

static Motor *motor = nullptr;
static Rotary *encoder = nullptr;

Motor::Motor(uint8_t _pin1, uint8_t _pin2, uint8_t _pin3, uint8_t _pin4)
{
    this->power_pin = _pin3;
    this->dir_pin = _pin4;

#ifdef __EEPROM__

    EEPROM.get(ADDRESS_POSITION, position);
    EEPROM.get(ADDRESS_END_STOP, end_stop);
    EEPROM.get(ADDRESS_MODE, mode);

#endif

    encoder = new Rotary(_pin1, _pin2);
    motor = this;

    auto interrupt_routine = []() {
        motor->update_position(encoder->process());
    };
    attachInterrupt((uint8_t)digitalPinToInterrupt(_pin1), interrupt_routine, CHANGE); // set interrupt
    attachInterrupt((uint8_t)digitalPinToInterrupt(_pin2), interrupt_routine, CHANGE); // set interrupt
    pinMode(this->dir_pin, OUTPUT);
    pinMode(this->power_pin, OUTPUT);
    digitalWrite(this->dir_pin, LOW);
    digitalWrite(this->power_pin, HIGH);
}

void Motor::off()
{
    if (state == OFF)
    {
        next_state = state;
        return;
    }

#ifdef __DEBUG__
    Serial.println("m off");
#endif
    digitalWrite(power_pin, HIGH);
    state = OFF;
    next_state = state;
}

void Motor::dir_cw()
{
    if (state == CW || (position >= end_stop && mode == CALIBRATED))
    {
        next_state = state;
        return;
    }

#ifdef __DEBUG__
    Serial.println("m cw");
#endif
    digitalWrite(dir_pin, HIGH);
    delay(DIRECTION_CHANGE_DELAY);
    digitalWrite(power_pin, LOW);
    state = CW;
    next_state = state;
}

void Motor::dir_ccw()
{
    if (state == CCW || (position <= 0 && mode != UNCALIBRATED))
    {
        next_state = state;
        return;
    }

#ifdef __DEBUG__
    Serial.println("m ccw");
#endif
    digitalWrite(dir_pin, LOW);
    delay(DIRECTION_CHANGE_DELAY);
    digitalWrite(power_pin, LOW);
    state = CCW;
    next_state = state;
}

unsigned int Motor::get_position()
{
    return position;
}

void Motor::reset_position()
{
    this->position = 0u;
#ifdef __EEPROM__
    EEPROM.update(ADDRESS_POSITION, position);
#endif
#ifdef __DEBUG__
    Serial.println("m rst");
#endif
}

void Motor::set_position(unsigned int pos)
{
    if (mode != CALIBRATED)
    {
        return;
    }
    next_position = pos;
    if (next_position < position)
    {
        this->set_state(CCW);
    }
    else
    {
        this->set_state(CW);
    }
#ifdef __DEBUG__
    Serial.print("m pos:");
    Serial.println(pos);
#endif
}

void Motor::update_position(const unsigned char result)
{
    if (mode == UNCALIBRATED)
    {
        return;
    }

    if (result == DIR_CW)
    {
        position++;
    }
    else if (result == DIR_CCW && position != 0)
    {
        position--;
    }

#ifdef __EEPROM__
    if (state == OFF)
        EEPROM.update(ADDRESS_POSITION, position);
#endif

    if (position == 0 || position >= end_stop)
    {
        if ((position == 0 && get_state() == CCW) ||
            (position >= end_stop && get_state() == CW))
        {
            off();
        }
    }
    if (next_position >= 0 && next_position == position)
    {
        off();
        next_position = -1;
    }
}

MotorState Motor::get_state()
{
    return this->state;
}

void Motor::set_state(MotorState state)
{
    this->next_state = state;
#ifdef __DEBUG__
    Serial.print("m state: ");
    Serial.println(state);
#endif
}

MotorMode Motor::get_mode()
{
    return this->mode;
}

void Motor::set_mode(MotorMode mode)
{
    this->mode = mode;
#ifdef __EEPROM__
    EEPROM.update(ADDRESS_MODE, this->mode);
#endif
#ifdef __DEBUG__
    Serial.print("m mode: ");
    Serial.println(mode);
#endif
}

void Motor::set_end_stop(unsigned int end_stop)
{
    this->end_stop = end_stop;
#ifdef __EEPROM__
    EEPROM.update(ADDRESS_END_STOP, this->end_stop);
#endif
#ifdef __DEBUG__
    Serial.print("m estop:");
    Serial.println(end_stop);
#endif
}

void Motor::cycle()
{
    if (next_state != state)
    {
        switch (next_state)
        {
        case OFF:
            this->off();
            break;
        case CCW:
            this->dir_ccw();
            break;
        case CW:
            this->dir_cw();
            break;
        }
    }
};

Motor::~Motor()
{
    delete encoder;
}
