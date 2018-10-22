#include "Service.h"
#include "limits.h"

unsigned long get_period(const unsigned long last, const unsigned long next)
{
    if (last > next) //overflow
    {
        return (ULONG_MAX - last) + next;
    }
    return next - last;
}
