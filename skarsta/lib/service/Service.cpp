#include "Service.h"
#include <Arduino.h>
#include <limits.h>

unsigned long get_period(const unsigned long last, const unsigned long next) {
    if (last > next) //overflow
    {
        return (ULONG_MAX - last) + next;
    }
    return next - last;
}

void setPwmFrequency(int pin, int divisor) {
    byte mode;
    if (pin == 5 || pin == 6 || pin == 9 || pin == 10) {
        switch (divisor) {
            case 1:
                mode = 0x01;
                break;
            case 8:
                mode = 0x02;
                break;
            case 64:
                mode = 0x03;
                break;
            case 256:
                mode = 0x04;
                break;
            case 1024:
                mode = 0x05;
                break;
            default:
                return;
        }
        if (pin == 5 || pin == 6) {
            TCCR0B = TCCR0B & 0b11111000 | mode;
        } else {
            TCCR1B = TCCR1B & 0b11111000 | mode;
        }
    } else if (pin == 3 || pin == 11) {
        switch (divisor) {
            case 1:
                mode = 0x01;
                break;
            case 8:
                mode = 0x02;
                break;
            case 32:
                mode = 0x03;
                break;
            case 64:
                mode = 0x04;
                break;
            case 128:
                mode = 0x05;
                break;
            case 256:
                mode = 0x06;
                break;
            case 1024:
                mode = 0x07;
                break;
            default:
                return;
        }
        TCCR2B = TCCR2B & 0b11111000 | mode;
    }
}